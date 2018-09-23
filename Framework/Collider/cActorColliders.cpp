#include "stdafx.h"
#include "cActorColliders.h"
#include "./GameObject/cActor.h"
#include "./Collider/cColliderFactory.h"
#include "./Collider/cCollider.h"
#include "./Collider/cRayCollider.h"
#include "./Model/ModelPart/cModelMesh.h"
#include "./Model/ModelPart/cModelBone.h"
#include "./Command/cInputHandler.h"

cActorColliders::cActorColliders(weak_ptr<cActor> actor)
	:_actor(actor)
{
	D3DXMATRIX matrix;
	D3DXMatrixScaling(&matrix, 100, 100, 100);
	_pickChecker = cColliderFactory::Create(eColliderType::NONE, eColliderShape::SPHERE, _actor.lock()->GetTransform(), matrix);
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
								  const shared_ptr<cModelBone>& bone,
								  const D3DXMATRIX & matrix)
{
	//애니메이티드 본
	shared_ptr<cCollider> col = cColliderFactory::Create(type, shape, bone->GetAnimatedTransform(), matrix);

	bone->AddCollider(col);

	if (type == eColliderType::COMBO_ATTACK)
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
			if (attackCol.lock()->Contains(damageCol.lock()) == eContainmentType::Intersects)
				return true;
		}
	}

	return false;
}

bool cActorColliders::Collide(const weak_ptr<cCollider>& collider)
{
	auto colPtr = collider.lock();
	if (!colPtr)
		return false;

	for (auto&& dmgCol : _damageColliders)
	{
		auto dmgColPtr = dmgCol.lock();
		if (!dmgColPtr)
			continue;

		//충돌
		if (dmgColPtr->Contains(colPtr) != eContainmentType::Disjoint)
			return true;
	}

	return false;
}
