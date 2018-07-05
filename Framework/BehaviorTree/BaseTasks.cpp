#include "stdafx.h"
#include "BaseTasks.h"

cCompositeTask::cCompositeTask(string name)
	: cTask(name)
{
}

cCompositeTask::~cCompositeTask()
{
}

const cCompositeTask::TaskList & cCompositeTask::GetChildren() const
{
	return children;
}

void cCompositeTask::AddChild(std::unique_ptr<cTask> child)
{
	children.emplace_back(move(child));
}

/**********************************************************/

cSelector::cSelector()
	:cCompositeTask(typeid(this).name())
{
}

bool cSelector::Run()
{
	for (auto&& child : GetChildren())
	{
		if (child->Run())
			return true;
	}

	return false;
}

/**********************************************************/

cSequence::cSequence()
	:cCompositeTask(typeid(this).name())
{
}

bool cSequence::Run()
{
	for (auto&& child : GetChildren())
	{
		if (!child->Run())
			return false;
	}

	return true;
}
