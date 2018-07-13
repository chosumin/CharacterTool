#include "stdafx.h"
#include "cActorMenu.h"
#include "./GameObject/cActor.h"
#include "./Message/cEntityManager.h"
#include "./Message/cMessageDispatcher.h"
#include "./GameObject/cActorBuilder.h"

UI::cActorMenu::cActorMenu()
{
}

UI::cActorMenu::~cActorMenu()
{
	cEntityManager::Get()->RemoveEntity(GetID());
}

void UI::cActorMenu::Init()
{
	cEntityManager::Get()->RegisterEntity(eIdGroup::CharacterTool, shared_from_this());
}

void UI::cActorMenu::Update()
{
}

void UI::cActorMenu::PostRender()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Actor"))
		{
			if (ImGui::MenuItem("New Actor", ""))
				NewActor();

			if (ImGui::MenuItem("Load Actor", ""))
			{

			}

			if (ImGui::MenuItem("Save Actor", ""))
			{

			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void UI::cActorMenu::HandleMessage(const sTelegram & msg)
{
	switch (msg.message)
	{
		case eMessageType::RecieveActor:
		{
			_actor = *(weak_ptr<cActor>*)(msg.extraInfo);
		}
		break;
	}
}

void UI::cActorMenu::FunctionInitialize()
{
}

void UI::cActorMenu::NewActor()
{
	cActorBuilder builder;
	auto actor = builder.CreateTransform().CreateActor();
	
	vector<UINT> receivers;
	cEntityManager::Get()->GetIdentityGroup(eIdGroup::CharacterTool, receivers);

	//새 액터를 관여된 객체들에게 전달
	cMessageDispatcher::Get()->DispatchMessages(0, GetID(), receivers, eMessageType::RecieveActor, (void*)&actor);

	cDebug::Log("Created New Actor!");
}
