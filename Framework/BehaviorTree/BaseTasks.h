#pragma once
#include <memory>
#include <list>
#include <typeinfo>

class cTask
{
public:
	cTask(string name) : _taskName(name) {}

	virtual ~cTask() {}
	virtual bool Run() = 0;

	string GetName() const
	{
		return _taskName;
	}
protected:
	string _taskName;
};

class cCompositeTask : public cTask
{
public:
	using Task = std::unique_ptr<cTask>;
	using TaskList = std::list<Task>;
	
	cCompositeTask(string name);
	~cCompositeTask();
	const TaskList& GetChildren() const;

	void AddChild(std::unique_ptr<cTask> child);
private:
	TaskList children;
};

class cSelector : public cCompositeTask
{
public:
	cSelector();

	virtual bool Run() override;
};

class cSequence : public cCompositeTask
{
public:
	cSequence();

	virtual bool Run() override;
};