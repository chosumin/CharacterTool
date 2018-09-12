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

	// cTask��(��) ���� ��ӵ�
	virtual std::unique_ptr<cTask> Clone() const override;
	virtual eState Run() override;
	virtual void RenderInfo() override;

	// iFile ���
	virtual void LoadJson(Json::Value& root) override;
	virtual void SaveJson(Json::Value& root) override;
private:
	//Ÿ�ٰ��� ���� ����
	void SetTargetDirection(shared_ptr<cBlackboard>& blackboardPtr);
	void EndCombo(shared_ptr<cBlackboard>& blackboardPtr);

	//�浹 �̺�Ʈ
	void Collide(const shared_ptr<cAnimClip>& clipPtr, const shared_ptr<cAnimator>& animatorPtr);

	//ImGui �̺�Ʈ ������ �޺� �ڽ� ����
	void EventFrameComboBox(int& index, const string& title,
							wstring& name);
private:
	weak_ptr<cActor> _actor;
	weak_ptr<cAnimClip> _clip;
	float _blendTime;
	string _selectedClip;

	int _attackNumber; //���� ��ȣ

	wstring _nameComboFrame; //�޺� ������ �̸�
	wstring _nameCollisionFrame; //�浹 ������ �̸�

	bool _succeedCollision;
};