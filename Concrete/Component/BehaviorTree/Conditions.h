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

	// cTask��(��) ���� ��ӵ�
	virtual std::unique_ptr<cTask> Clone() const override;
	virtual eState Run() override;
	virtual void RenderInfo() override;
private:
	weak_ptr<cBlackboard> _blackboard;

	string _boolParam;
	bool _boolean;

	//todo : �ۼ�Ƽ�� �ȿ� ���ϴ���
	//todo : �ܼ� bool ��
	//todo : �� ��, ū�� ������ ������
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

	// cTask��(��) ���� ��ӵ�
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