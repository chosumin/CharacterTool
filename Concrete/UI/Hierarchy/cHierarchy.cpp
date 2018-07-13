#include "stdafx.h"
#include "cHierarchy.h"
#include "cModelHierarchy.h"
#include "./GameObject/cActor.h"
#include "./Message/cEntityManager.h"
#include "./Message/cMessageDispatcher.h"

cHierarchy::cHierarchy()
{
}

cHierarchy::~cHierarchy()
{
	cEntityManager::Get()->RemoveEntity(GetID());
}

void cHierarchy::Init()
{
	_bOpen = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

	cEntityManager::Get()->RegisterEntity(eIdGroup::CharacterTool, shared_from_this());

	_modelHierarchy = make_unique<cModelHierarchy>(_actor, _bOpen);
}

void cHierarchy::Update()
{
}

void cHierarchy::Render()
{
}

void cHierarchy::PostRender()
{
	ImGui::Begin("Hierarchy");
	{
		_modelHierarchy->PostRender();
	}
	ImGui::End();
	
}

void cHierarchy::HandleMessage(const sTelegram & msg)
{
	switch (msg.message)
	{
		case eMessageType::RecieveActor:
		{
			_actor = *(weak_ptr<cActor>*)(msg.extraInfo);
			_modelHierarchy->SetActor(_actor);
		}
		break;
		case eMessageType::recievetool
	}
}

void cHierarchy::FunctionInitialize()
{
}
