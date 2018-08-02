#include "stdafx.h"
#include "cActorMenu.h"
#include "./GameObject/cActor.h"
#include "./Message/cEntityManager.h"
#include "./Message/cMessageDispatcher.h"
#include "./GameObject/cActorBuilder.h"

UI::cActorMenu::cActorMenu()
	:_actorName("None Actor")
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
				SaveActor();

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	bool flag = true;
	ImGui::Begin("Hierarchy", &flag, ImGuiWindowFlags_HorizontalScrollbar);
	{
		ImGui::TextColored(ImVec4(1, 1, 0, 1), _actorName.c_str());
	}
	ImGui::End();
}

void UI::cActorMenu::HandleMessage(const sTelegram & msg)
{
	switch (msg.message)
	{
		case eMessageType::RECIEVE_ACTOR:
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
	cMessageDispatcher::Get()->DispatchMessages(0, GetID(), receivers, eMessageType::RECIEVE_ACTOR, (void*)&actor);

	_actorName = "New Actor";

	cDebug::Log("Created New Actor!");
}

void UI::cActorMenu::SaveActor()
{
	//툴에게 저장 메시지 보냄
	vector<UINT> receivers;
	cEntityManager::Get()->GetIdentityGroup(eIdGroup::CharacterTool, receivers);

	cMessageDispatcher::Get()->DispatchMessages(0, GetID(), receivers, eMessageType::RECIEVE_ACTOR, nullptr);

	cDebug::Log(("Save Success! : " + _actorName).c_str());
}

bool UI::cActorMenu::AlertActor()
{
	return false;
}
