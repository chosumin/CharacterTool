#pragma once
#include "./Interface/iCloneable.h"
#include "./Interface/iFile.h"

static ImVec4 ActionTextColor = ImVec4(250, 250, 0, 1);

enum class eTaskState
{
	NONE, DELETE_ALL, DELETE_ONLY_ME, DELETE_ONLY_SUBNODES
};

class cBehaviorTree;
class cTask :
	public iCloneable<cTask>,
	public iFile,
	public enable_shared_from_this<cTask>
{
public:
	enum class eState
	{
		NONE, FAILURE, SUCCESS, RUNNING
	};
public:
	using Task = std::shared_ptr<cTask>;
	using TaskList = std::vector<Task>;

	cTask(weak_ptr<cBehaviorTree> tree, string name,
		  const ImVec2& position = ImVec2(0, 0),
		  const weak_ptr<cTask>& parent = weak_ptr<cTask>());

	virtual ~cTask() {}

	//None ���·� ����
	//@param : ���� ���µ� �ʱ�ȭ�ϴ��� ����
	void SetInitState(const bool& includeRunning);

	//��ȸ�ϸ� ����Ǵ� �Լ�
	virtual eState Run() = 0;

	//��� ����â ���
	virtual void RenderInfo() = 0;

	//��� �޴�â ���
	//���� �޴� Ŭ���� true ��ȯ, ������ false
	bool RenderMenu();

	virtual void AddChild(std::shared_ptr<cTask> child) {}

	//test : �ൿƮ�� �׽�Ʈ
	void RenderName()
	{
		if (_state == eState::SUCCESS || _state == eState::RUNNING)
		{
			cDebug::Log("%s", _taskName.c_str());
			if (GetChildren())
			{
				for (auto&& child : *GetChildren())
				{
					child->RenderName();
				}
			}
		}
	}
public:
	/*******************
		Getter Setter
	********************/
	virtual TaskList* GetChildren();

	const string & GetName() const { return _taskName; }

	const ImVec2& GetPosition() { return _pos; }
	void SetPosition(ImVec2&& pos) { _pos = pos; }

	const ImVec2& GetSize() { return _size; }
	void SetSize(ImVec2&& size) { _size = size; }

	eTaskState GetState() const { return _delete; }

	void SetBehaviorTree(const weak_ptr<cBehaviorTree> & tree)
	{ _tree = tree; }

	eState GetRunningState() { return _state; }
	
	weak_ptr<cTask> GetParent() const;
	void SetParent(const weak_ptr<cTask> & parent)
	{ _parent = parent; }

	bool GetHide() const { return _hide; }
protected:
	void LoadDefaultInfo(Json::Value& task);
	void SaveDefaultInfo(Json::Value& task);
protected:
	eState _state;
	weak_ptr<cTask> _parent;
	string _taskName;
	ImVec2 _pos;

	weak_ptr<cBehaviorTree> _tree;

	bool _hide; //���� üũ
private:
	ImVec2 _size;
	eTaskState _delete;
};

class cCompositeTask : public cTask
{
public:
	cCompositeTask(weak_ptr<cBehaviorTree> tree,
				   const ImVec2& position = ImVec2(0, 0),
				   weak_ptr<cTask> parent = weak_ptr<cTask>());
	~cCompositeTask();
	
	virtual eState Run() override;

	virtual TaskList* GetChildren() override;
	virtual void RenderInfo() override;

	virtual void AddChild(std::shared_ptr<cTask> child) override;

	//iCloneable ���
	virtual std::unique_ptr<cTask> Clone() const override;
	
	//iFile ���
	virtual void LoadJson(Json::Value& root) override;
	virtual void SaveJson(Json::Value& root) override;
protected:
	int _type; //selector, sequence, parallel
private:
	TaskList _children;

	static const char* ITEMS[3];

	char _summury[255]; //����
};

class cRootTask : public cCompositeTask
{
public:
	cRootTask();
	~cRootTask();
};