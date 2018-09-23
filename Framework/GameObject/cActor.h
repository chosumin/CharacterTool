#pragma once
#include "./Message/cBaseGameEntity.h"

struct sTransform;
class cCollider;
class cModel;
class cActorColliders;
class cAnimator;
class cBehaviorTree;
class cBlackboard;
class cActor : 
	public cBaseGameEntity
{
private:
	friend class cActorFactory;
public:
	~cActor();

	void Init();
	void Update();
	void Render();
	void RenderShadow();

	//�ൿƮ�� ������ ������Ʈ ������Ʈ
	void TestUpdate();

	void ClearEnemies();
	void SetEnemy(const weak_ptr<cActor> & actors);

	void StartScene();
	void StopScene();

	void GetPosition(D3DXVECTOR3& position);
	void SetPosition(D3DXVECTOR3& position);

	bool Collide(const weak_ptr<class cCollider> & collider);
	bool Attack();
	void Damage(const weak_ptr<cActor> & opponent);
public:
	// cBaseGameEntity��(��) ���� ��ӵ�
	virtual void HandleMessage(const sTelegram & msg) override;
	virtual void FunctionInitialize() override;
public:
	//���� �̸�
	const wstring& GetName() { return _name; }
	void SetName(wstring&& name) { _name = name; }

	//��
	const shared_ptr<cModel> & GetModel() const;
	void SetModel(weak_ptr<cModel> model);

	//Ʈ������
	const shared_ptr<struct sTransform> & GetTransform() const
	{
		return _transform;
	}

	//�浹ü
	const shared_ptr<cActorColliders> & GetColliders() const
	{
		return _colliders;
	}

	//�ִϸ��̼�
	const shared_ptr<cAnimator> & GetAnimator() const
	{
		return _animator;
	}

	//�ൿƮ��
	const shared_ptr<cBehaviorTree> & GetBehaviorTree() const
	{
		return _behaviorTree;
	}
	
	//������
	shared_ptr<cBlackboard> & GetBlackboard() 
	{ 
		return _blackboard;
	}
private:
	cActor();
private:
	wstring _name;
	shared_ptr<sTransform> _transform;
	shared_ptr<cModel> _model;
	shared_ptr<cAnimator> _animator;
	shared_ptr<cBlackboard> _blackboard;
	shared_ptr<cBehaviorTree> _behaviorTree;
	shared_ptr<cActorColliders> _colliders;
};