#include "stdafx.h"
#include "cCollisionManager.h"
#include "./GameObject/cActor.h"
#include "./Collider/cActorColliders.h"
#include "./Collider/cCollider.h"

cCollisionManager::cCollisionManager()
{
}

cCollisionManager::~cCollisionManager()
{
}

void cCollisionManager::Update()
{
	auto playerPtr = _player.lock();
	auto enemyPtr = _enemy.lock();

	if (playerPtr && enemyPtr)
	{
		auto&& enemyVec = enemyPtr->GetColliders().lock()->GetDamageColliders();
		for (auto&& enemyCol : enemyVec)
		{
			playerPtr->Collide(enemyCol);
		}

		auto&& playerVec = playerPtr->GetColliders().lock()->GetDamageColliders();
		for (auto&& playerCol : playerVec)
		{
			enemyPtr->Collide(playerCol);
		}
	}
}

void cCollisionManager::HandleMessage(const sTelegram & msg)
{
}

void cCollisionManager::FunctionInitialize()
{
}
