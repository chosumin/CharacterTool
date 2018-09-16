#include "../../stdafx.h"
#include "cEntityManager.h"
#include "./Concrete/Message/eIdGroup.h"

const UINT cEntityManager::idBits = 20;

void cEntityManager::GetGroupId(IN const UINT id, UINT* pGroup, UINT* pId) const
{
	if (pGroup != nullptr)
		*pGroup = (id & 0xFFF00000) >> idBits;

	if (pId != NULL)
		*pId = (id & 0x000FFFFF);
}

weak_ptr<cBaseGameEntity> cEntityManager::GetEntityFromId(UINT id) const
{
	auto iter = _entityMap.find(id);

	//assert(iter != _entityMap.end() && "ID가 존재하지 않습니다!");

	//만약 삭제된 id를 가져오면 nullptr 처리
	if (iter == _entityMap.end())
		return weak_ptr<cBaseGameEntity>();

	return iter->second;
}

UINT cEntityManager::GetId(const weak_ptr<cBaseGameEntity> entity) const
{
	for (auto i : _entityMap)
	{
		if (i.second.lock() == entity.lock())
			return i.first;
	}

	return -1;
}

void cEntityManager::GetIdentityGroup(eIdGroup group, vector<shared_ptr<cBaseGameEntity>>& entities)
{
	entities.clear();
	for(auto i : _entityMap)
	{
		UINT tempGroup;
		GetGroupId(i.first, &tempGroup, NULL);

		//fixme : 수정 예정
		if (tempGroup == (UINT)group)
			entities.emplace_back(i.second);
	}
}

void cEntityManager::GetIdentityGroup(eIdGroup group, vector<UINT>& entities)
{
	entities.clear();
	for (auto i : _entityMap)
	{
		UINT tempGroup;
		GetGroupId(i.first, &tempGroup, NULL);

		if (tempGroup == (UINT)group)
			entities.push_back(i.first);
	}
}

void cEntityManager::RegisterEntity(const eIdGroup group, weak_ptr<cBaseGameEntity> object)
{
	UINT maximum = 0;
	for (auto i : _entityMap)
	{
		UINT id = i.first;
		UINT tempGroup, tempId;
		GetGroupId(id, &tempGroup, &tempId);

		if (tempGroup == (UINT)group)
			maximum = max(maximum, tempId);
	}

	UINT newId = maximum + 1;
	newId = ((UINT)group << idBits) | newId;

	object.lock()->id = newId;

	_entityMap[newId] = object;
}

void cEntityManager::RemoveEntity(UINT id)
{
	auto iter = _entityMap.find(id);
	assert(iter != _entityMap.end() && "삭제하려는 id가 존재하지 않습니다!");

	_entityMap.erase(iter);
}

void cEntityManager::Clear()
{
	_entityMap.clear();
}

cEntityManager::cEntityManager()
{
}

cEntityManager::~cEntityManager()
{
}
