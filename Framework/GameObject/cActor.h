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

	//행동트리 제외한 컴포넌트 업데이트
	void TestUpdate();

	void SetEnemy(const weak_ptr<cActor>& actor);

	void StartScene();
	void StopScene(const D3DXVECTOR3 & position);

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
	weak_ptr<class cModel> GetModel() const;
	void SetModel(weak_ptr<cModel> model);

	//트랜스폼
	weak_ptr<struct sTransform> GetTransform() const;

	//충돌체
	weak_ptr<cActorColliders> GetColliders() const;

	//애니메이션
	weak_ptr<cAnimator> GetAnimator() const;

	//행동트리
	weak_ptr<cBehaviorTree> GetBehaviorTree() const;
	
	//블랙보드
	weak_ptr<cBlackboard> GetBlackboard() const 
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