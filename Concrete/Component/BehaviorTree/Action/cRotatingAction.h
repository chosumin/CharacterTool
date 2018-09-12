#pragma once
#include "./BehaviorTree/BaseTasks.h"

class cActor;
class cRotatingAction : public cTask
{
public:
	cRotatingAction(const weak_ptr<cActor> & actor,
					const weak_ptr<cBehaviorTree> & tree,
					const ImVec2& position,
					weak_ptr<cTask> parent = weak_ptr<cTask>());
	~cRotatingAction();

	// cTask을(를) 통해 상속됨
	virtual std::unique_ptr<cTask> Clone() const override;
	virtual eState Run() override;
	virtual void RenderInfo() override;

	// iFile 상속
	virtual void LoadJson(Json::Value& root) override;
	virtual void SaveJson(Json::Value& root) override;
private:
	void RotateTarget();
	void RotateKeyboard();
	void RotateAngle();

	//@brief : 현재 방향을 다음 방향으로 틱 회전
	void Rotate(OUT shared_ptr<sTransform> transformPtr,
				const D3DXVECTOR3& actorDirection,
				const D3DXVECTOR3& targetDirection);
private:
	weak_ptr<cActor> _actor;
	weak_ptr<sTransform> _transform;

	//최대 속도 이상이면 바로 회전하기
	float _rotationSpeed;
	float _maxSpeed;

	//타겟 방향 회전, 키보드 방향 회전, 카메라 방향 회전, 일정 각도 회전
	enum eRotationType
	{
		TARGET, KEYBOARD, CONSTANT
	}_rotationType;

	float _rotationAngle;
	float _remaining; //남은 각도
};
