#include "stdafx.h"
#include "cTool.h"
#include "./Message/cEntityManager.h"
#include "./Message/cMessageDispatcher.h"

UI::cTool::cTool()
{
}

UI::cTool::~cTool()
{
	cEntityManager::Get()->RemoveEntity(GetID());
}

void UI::cTool::Init()
{
	cEntityManager::Get()->RegisterEntity(eIdGroup::CharacterTool, cBaseGameEntity::downcasted_shared_from_this<cTool>());
}

void UI::cTool::HandleMessage(const sTelegram & msg)
{
}

void UI::cTool::FunctionInitialize()
{
}

void UI::cTool::SetActor(weak_ptr<cActor> actor)
{
	_actor = actor;
}

bool UI::cTool::AlertActor()
{
	bool expired = _actor.expired();
	if (expired)
		cDebug::Log("Add Actor First!");

	return expired;
}

void UI::cTool::SendMe()
{
	vector<UINT> receivers;
	cEntityManager::Get()->GetIdentityGroup(eIdGroup::CharacterTool, receivers);

	cMessageDispatcher::Get()->DispatchMessages(0, GetID(), receivers, eMessageType::RECIEVE_TOOL, (void*)&shared_from_this());
}
