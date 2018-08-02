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

	void AddCollider(bool attack,
 eColliderShape shape, weak_ptr<cModelBone> bone,
					 const D3DXMATRIX& matrix);
	void AddCollider(weak_ptr<cCollider> collider);

	vector<weak_ptr<cCollider>>& GetColliders();
private:
	weak_ptr<cActor> _actor;
	vector<weak_ptr<cCollider>> _colliders;
};