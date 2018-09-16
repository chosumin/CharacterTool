#pragma once
#include "./BehaviorTree/BaseTasks.h"

class cActor;
class cBlackboard;
class cAnimClip;
class cAnimator;
class cSingleAttackAction : public cTask
{
public:
	cSingleAttackAction(const weak_ptr<cActor> & actor,
					   const weak_ptr<cBehaviorTree> & bTree,
					   const ImVec2& position,
					   weak_ptr<cTask> parent = weak_ptr<cTask>());
	~cSingleAttackAction();

	// cTask을(를) 통해 상속됨
	virtual std::unique_ptr<cTask> Clone() const override;
	virtual eState Run() override;
	virtual void RenderInfo() override;

	// iFile 상속
	virtual void LoadJson(Json::Value& root) override;
	virtual void SaveJson(Json::Value& root) override;
private:
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

	wstring _nameCollisionFrame; //충돌 프레임 이름

	bool _succeedCollision;
};