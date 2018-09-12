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
		auto playerColPtr = playerPtr->GetColliders().lock();
		auto enemyColPtr = enemyPtr->GetColliders().lock();

	}
}

void cCollisionManager::HandleMessage(const sTelegram & msg)
{
}

void cCollisionManager::FunctionInitialize()
{
}
