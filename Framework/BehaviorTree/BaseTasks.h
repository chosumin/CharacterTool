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
	using Task = std::shared_ptr<cTask>;
	using TaskList = std::vector<Task>;

	cTask(weak_ptr<cBehaviorTree> tree, string name, const ImVec2& position = ImVec2(0,0));

	virtual ~cTask() {}

	//Ʈ�� ��ȸ�ϸ� ����Ǵ� �Լ�
	virtual bool Run() = 0;

	//�� �����Ӹ��� ȣ��Ǵ� �Լ�
	virtual void Update() = 0;

	//��� ����â ���
	virtual void RenderInfo() = 0;

	//��� �޴�â ���
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

	const ImVec4& GetColor() { return _color; }
	void SetColor(ImVec4&& color) { _color = color; }

	eTaskState GetState() const { return _delete; }

	void SetBehaviorTree(weak_ptr<cBehaviorTree> tree)
	{ _tree = tree; }
protected:
	string _taskName;
	ImVec2 _pos;

	weak_ptr<cBehaviorTree> _tree;
private:
	ImVec2 _size;
	ImVec4 _color;
	eTaskState _delete;
};

class cCompositeTask : public cTask
{
public:
	cCompositeTask(string name);
	~cCompositeTask();
	
	virtual void Update() override {}

	virtual TaskList* GetChildren() override;
	virtual void RenderInfo() override;

	virtual void AddChild(std::shared_ptr<cTask> child) override;

	// cTask��(��) ���� ��ӵ�
	virtual std::unique_ptr<cTask> Clone() const override;
private:
	TaskList _children;
};

class cSelector : public cCompositeTask
{
public:
	cSelector(const ImVec2& position = ImVec2(0, 0));
	cSelector(string name);

	virtual bool Run() override;
};

class cSequence : public cCompositeTask
{
public:
	cSequence(const ImVec2& position = ImVec2(0, 0));

	virtual bool Run() override;
};