#include "stdafx.h"
#include "cActorManager.h"
#include "./GameObject/cActor.h"
#include "./BehaviorTree/cBehaviorTree.h"
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
	bool open = true;
	
	ImGuiWindowFlags flag;
	flag = ImGuiWindowFlags_NoTitleBar;
	flag |= ImGuiWindowFlags_NoMove;
	//flag |= ImGuiWindowFlags_NoResize;
	ImGui::SetNextWindowBgAlpha(0.3f);

	auto pos = ImVec2(300.0f,
					  25.0f);
	ImGui::SetNextWindowPos(pos);
	ImGui::Begin("ActorManager",&open, flag);
	{
		//¾À ½ÃÀÛ ¹öÆ°
		if (ImGui::Button("Start"))
		{
			if (_actor)
			{
				auto treePtr = _actor->GetBehaviorTree().lock();
				if (treePtr)
					treePtr->Run();
			}
		}
		ImGui::SameLine();
		//¾À Á¾·á ¹öÆ°
		if (ImGui::Button("Stop"))
		{

		}
	}
	ImGui::End();
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
		case eMessageType::LOAD_ACTOR:
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
