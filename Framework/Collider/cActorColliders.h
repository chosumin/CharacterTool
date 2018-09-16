#pragma once

enum class eColliderShape;
struct sTransform;
class cActor;
class cCollider;
class cModelBone;
class cActorColliders
{
public:
	cActorColliders(weak_ptr<cActor> actor);
	~cActorColliders();

	void Update();
	void Render();

	void AddCollider(eColliderType type, eColliderShape shape,
					 weak_ptr<cModelBone>& bone,
					 const D3DXMATRIX& matrix);
	void DeleteCollider();

	vector<weak_ptr<cCollider>>& GetAttackColliders()
	{
		return _attackColliders;
	}
	vector<weak_ptr<cCollider>>& GetDamageColliders()
	{
		return _damageColliders;
	}

	//test : 충돌 테스트, 상대에게 공격함
	bool Attack(weak_ptr<cActorColliders> colliders);
	bool Collide(const weak_ptr<cCollider> & collider);
private:
	weak_ptr<cActor> _actor;

	vector<weak_ptr<cCollider>> _attackColliders;
	vector<weak_ptr<cCollider>> _damageColliders;

	shared_ptr<cCollider> _pickChecker;
};