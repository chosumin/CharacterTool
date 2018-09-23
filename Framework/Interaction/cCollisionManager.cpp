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
	/*auto playerPtr = _player.lock();
	auto enemyPtr = _enemy.lock();

	if (playerPtr && enemyPtr)
	{
		auto&& enemyVec = enemyPtr->GetColliders()->GetDamageColliders();
		for (auto&& enemyCol : enemyVec)
		{
			playerPtr->Collide(enemyCol);
		}

		auto&& playerVec = playerPtr->GetColliders()->GetDamageColliders();
		for (auto&& playerCol : playerVec)
		{
			enemyPtr->Collide(playerCol);
		}
	}*/

	UINT i = 0;
	UINT maximum = _actors.size();
	if (maximum <= 1)
		return;

	while (i < maximum - 1)
	{
		for (UINT temp = 1; temp < maximum - i; temp++)
		{
			auto actor1Ptr = _actors[i].lock();
			auto actor2Ptr = _actors[i + temp].lock();
			if (actor1Ptr && actor2Ptr)
			{
				auto&& actor1Vec = actor1Ptr->GetColliders()->GetDamageColliders();
				for (auto&& actor1Col : actor1Vec)
				{
					actor2Ptr->Collide(actor1Col);
				}

				auto&& actor2Vec = actor2Ptr->GetColliders()->GetDamageColliders();
				for (auto&& actor2Col : actor2Vec)
				{
					actor1Ptr->Collide(actor2Col);
				}
			}
		}
		++i;
	}
}

void cCollisionManager::HandleMessage(const sTelegram & msg)
{
}

void cCollisionManager::FunctionInitialize()
{
}
