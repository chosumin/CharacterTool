#pragma once
#include "./Interface/iCloneable.h"

enum class eTaskState
{
	NONE, DELETE_ALL, DELETE_ONLY_ME, DELETE_ONLY_SUBNODES
};

class cTask : public iCloneable<cTask>
{
public:
	using Task = std::shared_ptr<cTask>;
	using TaskList = std::vector<Task>;

	cTask(string name, ImVec2& position = ImVec2(0,0));

	virtual ~cTask() {}
	virtual bool Run() = 0;

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

	const ImVec4& GetColor() { return _color; }
	void SetColor(ImVec4&& color) { _color = color; }

	eTaskState GetState() const { return _delete; }
protected:
	string _taskName;
	ImVec2 _pos;
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
	
	virtual TaskList* GetChildren() override;
	virtual void RenderInfo() override;

	virtual void AddChild(std::shared_ptr<cTask> child) override;

	// cTask을(를) 통해 상속됨
	virtual std::unique_ptr<cTask> Clone() const override;
private:
	TaskList _children;
};

class cSelector : public cCompositeTask
{
public:
	cSelector(ImVec2& position = ImVec2(0, 0));
	cSelector(string name);

	virtual bool Run() override;
};

class cSequence : public cCompositeTask
{
public:
	cSequence(ImVec2& position = ImVec2(0, 0));

	virtual bool Run() override;
};