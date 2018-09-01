#pragma once
#include "./BehaviorTree/BaseTasks.h"

class cActor;
class cBlackboard;

class cBoolCondition : public cTask
{
public:
	cBoolCondition(weak_ptr<cBlackboard> blackboard,
				   weak_ptr<cBehaviorTree> tree,
				   const ImVec2& position,
				   const weak_ptr<cTask>& parent = weak_ptr<cTask>());
	~cBoolCondition();

	// cTask을(를) 통해 상속됨
	virtual std::unique_ptr<cTask> Clone() const override;
	virtual eState Run() override;
	virtual void RenderInfo() override;
private:
	weak_ptr<cBlackboard> _blackboard;

	string _boolParam;
	bool _boolean;

	//todo : 퍼센티지 안에 속하는지
	//todo : 단순 bool 비교
	//todo : 값 비교, 큰지 작은지 같은지
};

/******************************************************************/

class cValueCompareCondition : public cTask
{
public:
	cValueCompareCondition(weak_ptr<cBlackboard> blackboard,
						   weak_ptr<cBehaviorTree> tree,
						   const ImVec2& position,
						   const weak_ptr<cTask>& parent = weak_ptr<cTask>());
	~cValueCompareCondition();

	// cTask을(를) 통해 상속됨
	virtual std::unique_ptr<cTask> Clone() const override;
	virtual eState Run() override;
	virtual void RenderInfo() override;
private:
	/*enum class eInequalitySign
	{
		GREATER_THAN_RIGHT, LESSER_THAN_RIGHT,
		EQUAL,
		EQUAL_GREATER_THAN_RIGHT, EQUAL_LESSER_THAN_RIGHT
	};
	eInequalitySign _sign;*/
	
	weak_ptr<cBlackboard> _blackboard;

	string _valueParam;

	D3DXVECTOR3 _compare;
};

/******************************************************************/

class cPercentageCondition : public cTask
{

};