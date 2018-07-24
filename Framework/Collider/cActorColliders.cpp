#include "stdafx.h"
#include "cActorColliders.h"
#include "./Collider/cColliderFactory.h"
#include "./Collider/cCollider.h"
#include "./Model/ModelPart/cModelMesh.h"

cActorColliders::cActorColliders(weak_ptr<cActor> actor)
	:_actor(actor)
{
}

cActorColliders::~cActorColliders()
{
}

void cActorColliders::Update()
{
	for (auto iter = _colliders.begin(); iter != _colliders.end();)
	{
		auto colPtr = iter->lock();
		if (!colPtr)
			iter = _colliders.erase(iter);
		else
		{
			colPtr->Update();
			iter++;
		}
	}
}

void cActorColliders::Render()
{
	for (auto iter = _colliders.begin(); iter != _colliders.end();iter++)
	{
		auto colPtr = iter->lock();
		colPtr->Render();
	}
}

void cActorColliders::AddCollider(bool attack, eColliderShape shape,
								  weak_ptr<cModelMesh> mesh,
								  const D3DXMATRIX & matrix)
{
	auto meshPtr = mesh.lock();

	auto type = attack ? eColliderType::ATTACK : eColliderType::DAMAGE;
	shared_ptr<cCollider> col = cColliderFactory::Create(type, shape, meshPtr->GetParentTransform(), matrix);

	meshPtr->SetCollider(col);

	AddCollider(col);
}

void cActorColliders::AddCollider(weak_ptr<cCollider> collider)
{
	_colliders.emplace_back(collider);
}

vector<weak_ptr<cCollider>>& cActorColliders::GetColliders()
{
	return _colliders;
}