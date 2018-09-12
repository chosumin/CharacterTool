#include "stdafx.h"
#include "cActorColliders.h"
#include "./Collider/cColliderFactory.h"
#include "./Collider/cCollider.h"
#include "./Model/ModelPart/cModelMesh.h"
#include "./Model/ModelPart/cModelBone.h"

cActorColliders::cActorColliders(weak_ptr<cActor> actor)
	:_actor(actor)
{
}

cActorColliders::~cActorColliders()
{
}

void cActorColliders::Update()
{
	for (auto&& collider : _attackColliders)
	{
		auto colPtr = collider.lock();
		if (colPtr)
			colPtr->Update();
	}

	for (auto&& collider : _damageColliders)
	{
		auto colPtr = collider.lock();
		if (colPtr)
			colPtr->Update();
	}
}

void cActorColliders::Render()
{
	for (auto&& collider : _attackColliders)
	{
		auto colPtr = collider.lock();
		if (colPtr)
			colPtr->Render();
	}
	
	for (auto&& collider : _damageColliders)
	{
		auto colPtr = collider.lock();
		if (colPtr)
			colPtr->Render();
	}
}

void cActorColliders::AddCollider(eColliderType type,
								  eColliderShape shape,
								  weak_ptr<cModelBone>& bone,
								  const D3DXMATRIX & matrix)
{
	auto bonePtr = bone.lock();

	//애니메이티드 본
	shared_ptr<cCollider> col = cColliderFactory::Create(type, shape, bonePtr->GetAnimatedTransform(), matrix);

	bonePtr->AddCollider(col);

	if (type == eColliderType::ATTACK)
		_attackColliders.emplace_back(col);
	else if (type == eColliderType::DAMAGE)
		_damageColliders.emplace_back(col);
}

void cActorColliders::DeleteCollider()
{
	for (auto iter = _attackColliders.begin(); iter != _attackColliders.end();)
	{
		if (iter->expired())
			iter = _attackColliders.erase(iter);
		else
			iter++;
	}

	for (auto iter = _damageColliders.begin(); iter != _damageColliders.end();)
	{
		if (iter->expired())
			iter = _damageColliders.erase(iter);
		else
			iter++;
	}
}

bool cActorColliders::Attack(weak_ptr<cActorColliders> colliders)
{
	auto opColPtr = colliders.lock();
	if (!opColPtr)
		return false;

	auto& damageCols = opColPtr->GetDamageColliders();
	for (auto&& damageCol : damageCols)
	{
		for (auto attackCol : _attackColliders)
		{
			if (attackCol.lock()->IntersectsWith(damageCol.lock()))
				return true;
		}
	}

	return false;
}
