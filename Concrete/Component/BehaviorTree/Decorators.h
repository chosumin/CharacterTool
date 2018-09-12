#pragma once
#include "./BehaviorTree/BaseTasks.h"

class cDecorator : public cTask
{
public:
	cDecorator(const weak_ptr<cBehaviorTree>& tree,
					   const ImVec2& position,
					   weak_ptr<cTask> parent = weak_ptr<cTask>());
	~cDecorator();

	// cTask��(��) ���� ��ӵ�
	virtual void RenderInfo() override;

	virtual void AddChild(std::shared_ptr<cTask> child);
	virtual TaskList* GetChildren();

	// iFile ���
	virtual void LoadJson(Json::Value& root) override;
	virtual void SaveJson(Json::Value& root) override;
protected:
	TaskList _child;
};

/******************************************************************/

class cInverter : public cDecorator
{
public:
	cInverter(const weak_ptr<cBehaviorTree>& tree,
			   const ImVec2& position,
			   weak_ptr<cTask> parent = weak_ptr<cTask>());
	~cInverter();

	// cDecorator��(��) ���� ��ӵ�
	virtual std::unique_ptr<cTask> Clone() const override;

	virtual eState Run() override;
};

/******************************************************************/

class cSucceeder : public cDecorator
{
public:
	cSucceeder(const weak_ptr<cBehaviorTree>& tree,
			  const ImVec2& position,
			  weak_ptr<cTask> parent = weak_ptr<cTask>());
	~cSucceeder();

	// cDecorator��(��) ���� ��ӵ�
	virtual std::unique_ptr<cTask> Clone() const override;

	virtual eState Run() override;
};