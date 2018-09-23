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

	//행동트리 제외한 컴포넌트 업데이트
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
	// cBaseGameEntity을(를) 통해 상속됨
	virtual void HandleMessage(const sTelegram & msg) override;
	virtual void FunctionInitialize() override;
public:
	//액터 이름
	const wstring& GetName() { return _name; }
	void SetName(wstring&& name) { _name = name; }

	//모델
	const shared_ptr<cModel> & GetModel() const;
	void SetModel(weak_ptr<cModel> model);

	//트랜스폼
	const shared_ptr<struct sTransform> & GetTransform() const
	{
		return _transform;
	}

	//충돌체
	const shared_ptr<cActorColliders> & GetColliders() const
	{
		return _colliders;
	}

	//애니메이션
	const shared_ptr<cAnimator> & GetAnimator() const
	{
		return _animator;
	}

	//행동트리
	const shared_ptr<cBehaviorTree> & GetBehaviorTree() const
	{
		return _behaviorTree;
	}
	
	//블랙보드
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