#pragma once
#include "./BehaviorTree/BaseTasks.h"

class cActor;
class cBlackboard;
class cMovingAction : public cTask
{
public:
	cMovingAction(const weak_ptr<cActor> & actor,
				  const weak_ptr<cBehaviorTree> & tree,
				  const ImVec2& position,
				  weak_ptr<cTask> parent = weak_ptr<cTask>());
	~cMovingAction();

	// cTask을(를) 통해 상속됨
	virtual std::unique_ptr<cTask> Clone() const override;
	virtual eState Run() override;
	virtual void RenderInfo() override;

	// iFile 상속
	virtual void LoadJson(Json::Value& root) override;
	virtual void SaveJson(Json::Value& root) override;
private:
	void MoveToTarget();
	void RotateAndMove(OUT shared_ptr<sTransform> & transformPtr,
					   const D3DXVECTOR3& actorDirection,
					   const D3DXVECTOR3& targetDirection);
	void MoveContantDistance();

	void Move();
private:
	weak_ptr<cActor> _actor;
	weak_ptr<cBlackboard> _blackboard;
	weak_ptr<sTransform> _transform;
	float _speed;

	enum eMovingType
	{
		CONTINUE, TO_TARGET, CONSTANT
	}_type;

	enum eDirection
	{
		FRONT, TARGET_REVERSE
	}_direction;

	float _distance;
	float _remaining; //남은 거리
	float _rotationSpeed;
};
