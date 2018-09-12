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

	// cTask��(��) ���� ��ӵ�
	virtual std::unique_ptr<cTask> Clone() const override;
	virtual eState Run() override;
	virtual void RenderInfo() override;

	// iFile ���
	virtual void LoadJson(Json::Value& root) override;
	virtual void SaveJson(Json::Value& root) override;
private:
	void RotateTarget();
	void RotateKeyboard();
	void RotateAngle();

	//@brief : ���� ������ ���� �������� ƽ ȸ��
	void Rotate(OUT shared_ptr<sTransform> transformPtr,
				const D3DXVECTOR3& actorDirection,
				const D3DXVECTOR3& targetDirection);
private:
	weak_ptr<cActor> _actor;
	weak_ptr<sTransform> _transform;

	//�ִ� �ӵ� �̻��̸� �ٷ� ȸ���ϱ�
	float _rotationSpeed;
	float _maxSpeed;

	//Ÿ�� ���� ȸ��, Ű���� ���� ȸ��, ī�޶� ���� ȸ��, ���� ���� ȸ��
	enum eRotationType
	{
		TARGET, KEYBOARD, CONSTANT
	}_rotationType;

	float _rotationAngle;
	float _remaining; //���� ����
};
