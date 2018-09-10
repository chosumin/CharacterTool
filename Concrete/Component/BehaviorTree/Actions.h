#pragma once
#include "./BehaviorTree/BaseTasks.h"

class cActor;
class cBlackboard;
class cAnimClip;
struct sTransform;

class cDataSetter : public cTask
{
public:
	cDataSetter(const weak_ptr<cActor> & actor,
				const weak_ptr<cBehaviorTree>& tree,
				const ImVec2& position,
				weak_ptr<cTask> parent = weak_ptr<cTask>());
	~cDataSetter();

	// cTask��(��) ���� ��ӵ�
	virtual std::unique_ptr<cTask> Clone() const override;
	virtual eState Run() override;
	virtual void RenderInfo() override;

	// iFile ���
	virtual void LoadJson(Json::Value& root) override;
	virtual void SaveJson(Json::Value& root) override;
private:
	void ComboBox(string& data);
private:
	weak_ptr<cActor> _actor;
	weak_ptr<cBlackboard> _blackboard;

	enum class eDataType
	{
		BOOL, FLOAT, VECTOR3, INT
	}_type;

	string _paramName;

	bool _compareBool;
	float _compareFloat;
	D3DXVECTOR3 _compareVector3;
	int _compareInt;
};

class cMovingAction : public cTask
{
public:
	cMovingAction(const weak_ptr<cActor> & actor,
				  const weak_ptr<cBehaviorTree> & tree,
				  const ImVec2& position,
				  weak_ptr<cTask> parent = weak_ptr<cTask>());
	~cMovingAction();

	// cTask��(��) ���� ��ӵ�
	virtual std::unique_ptr<cTask> Clone() const override;
	virtual eState Run() override;
	virtual void RenderInfo() override;

	// iFile ���
	virtual void LoadJson(Json::Value& root) override;
	virtual void SaveJson(Json::Value& root) override;
private:
	weak_ptr<cActor> _actor;
	weak_ptr<sTransform> _transform;
	float _speed;

	//todo : Ÿ�ٱ��� �̵�, ���� ��ġ���� �̵�, ��� �̵�
};

/******************************************************************/

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
		TARGET, KEYBOARD, ANGLE
	}_rotationType;

	float _rotationAngle;
};

/******************************************************************/

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
	weak_ptr<cActor> _actor;
	weak_ptr<cAnimClip> _clip;
	float _blendTime;
	string _selectedClip;
	int _comboFrame; //�޺� ���� ������
	int _attackNumber; //���� ��ȣ
};