#include "stdafx.h"
#include "cActorGroup.h"
#include "./Message/cEntityManager.h"

cActorGroup::cActorGroup(eIdGroup groupType)
	: _groupType(groupType)
{
}

cActorGroup::~cActorGroup()
{
	cEntityManager::Get()->RemoveEntity(GetID());
}

void cActorGroup::Init()
{
	cEntityManager::Get()->RegisterEntity(_groupType, downcasted_shared_from_this<cActorGroup>());
}

void cActorGroup::Update()
{
}

void cActorGroup::Render()
{
}
