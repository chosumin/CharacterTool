#pragma once
#include "./Interface/iCloneable.h"

enum class eTaskState
{
	NONE, DELETE_ALL, DELETE_ONLY_ME, DELETE_ONLY_SUBNODES
};

class cBehaviorTree;
class cTask :
	public iCloneable<cTask>,
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
		  const ImVec2& position = ImVec2(0,0),
		  const weak_ptr<cTask>& parent = weak_ptr<cTask>());

	virtual ~cTask() {}

	//None 상태로 변경
	//@param : 러닝 상태도 초기화하는지 여부
	void SetInitState(const bool& includeRunning);

	//순회하며 실행되는 함수
	virtual eState Run() = 0;

	//노드 정보창 출력
	virtual void RenderInfo() = 0;

	//노드 메뉴창 출력
	bool RenderMenu();

	virtual void AddChild(std::shared_ptr<cTask> child) {}
public:
	/*******************
		Getter Setter
	********************/
	virtual TaskList* GetChildren();

	string GetName() const { return _taskName; }

	const ImVec2& GetPosition() { return _pos; }
	void SetPosition(ImVec2&& pos) { _pos = pos; }

	const ImVec2& GetSize() { return _size; }
	void SetSize(ImVec2&& size) { _size = size; }

	eTaskState GetState() const { return _delete; }

	void SetBehaviorTree(weak_ptr<cBehaviorTree> tree)
	{ _tree = tree; }

	eState GetRunningState() { return _state; }
	
	weak_ptr<cTask> GetParent() const;
	void SetParent(weak_ptr<cTask> parent) { _parent = parent; }
protected:
	eState _state;
	weak_ptr<cTask> _parent;
	string _taskName;
	ImVec2 _pos;

	weak_ptr<cBehaviorTree> _tree;
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

	// cTask을(를) 통해 상속됨
	virtual std::unique_ptr<cTask> Clone() const override;
protected:
	int _type;
private:
	TaskList _children;

	static const char* ITEMS[3];
};

class cRootTask : public cCompositeTask
{
public:
	cRootTask();
	~cRootTask();
};