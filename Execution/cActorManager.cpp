#include "stdafx.h"
#include "cActorManager.h"

#include "./GameObject/cActor.h"

#include "./Message/cEntityManager.h"
#include "./Message/cMessageDispatcher.h"

cActorManager::cActorManager(weak_ptr<sGlobalVariable> global)
	:_global(global)
{
}

cActorManager::~cActorManager()
{
	cEntityManager::Get()->RemoveEntity(GetID());
}

void cActorManager::Init()
{
	cEntityManager::Get()->RegisterEntity(eIdGroup::CharacterTool, shared_from_this());
}

void cActorManager::Update()
{
	if (_actor)
		_actor->Update();
}

void cActorManager::Render()
{
	if (_actor)
		_actor->Render();
}

void cActorManager::PostRender()
{
}

void cActorManager::HandleMessage(const sTelegram & msg)
{
	switch (msg.message)
	{
		case eMessageType::RECIEVE_ACTOR:
		{
			auto actor = (weak_ptr<cActor>*)(msg.extraInfo);
			_actor = actor->lock();
		}
		break;
	}
}

void cActorManager::FunctionInitialize()
{
}
