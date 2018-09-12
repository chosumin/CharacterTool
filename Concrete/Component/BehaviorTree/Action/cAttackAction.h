#pragma once
#include "./BehaviorTree/BaseTasks.h"

class cActor;
class cBlackboard;
class cAnimClip;
class cAnimator;
class cAttackAction : public cTask
{
public:
	cAttackAction(const weak_ptr<cActor> & actor,
				  const weak_ptr<cBehaviorTree> & bTree,
				  const ImVec2& position,
				  weak_ptr<cTask> parent = weak_ptr<cTask>());
	~cAttackAction();

	// cTask을(를) 통해 상속됨
	virtual std::unique_ptr<cTask> Clone() const override;
	virtual eState Run() override;
	virtual void RenderInfo() override;

	// iFile 상속
	virtual void LoadJson(Json::Value& root) override;
	virtual void SaveJson(Json::Value& root) override;
private:
	//타겟과의 방향 세팅
	void SetTargetDirection(shared_ptr<cBlackboard>& blackboardPtr);
	void EndCombo(shared_ptr<cBlackboard>& blackboardPtr);

	//충돌 이벤트
	void Collide(const shared_ptr<cAnimClip>& clipPtr, const shared_ptr<cAnimator>& animatorPtr);

	//ImGui 이벤트 프레임 콤보 박스 띄우기
	void EventFrameComboBox(int& index, const string& title,
							wstring& name);
private:
	weak_ptr<cActor> _actor;
	weak_ptr<cAnimClip> _clip;
	float _blendTime;
	string _selectedClip;

	int _attackNumber; //공격 번호

	wstring _nameComboFrame; //콤보 프레임 이름
	wstring _nameCollisionFrame; //충돌 프레임 이름

	bool _succeedCollision;
};