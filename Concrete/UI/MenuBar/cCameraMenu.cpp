#include "stdafx.h"
#include "cCameraMenu.h"
#include "./GameObject/cActor.h"
#include "./GameObject/cActorFactory.h"
#include "./Helper/cPath.h"
#include "./Helper/cString.h"
#include "./Concrete/Message/eIdGroup.h"
#include "./Concrete/Message/eMessageType.h"

UI::cCameraMenu::cCameraMenu()
{
}

UI::cCameraMenu::~cCameraMenu()
{
	cEntityManager::Get()->RemoveEntity(GetID());
}

void UI::cCameraMenu::Init()
{
	cEntityManager::Get()->RegisterEntity(eIdGroup::CHARACTER_TOOL, shared_from_this());

	FunctionInitialize();
}

void UI::cCameraMenu::Update()
{
}

void UI::cCameraMenu::PostRender()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Camera"))
		{
			//자유 시점
			if (ImGui::MenuItem("Free Point", ""))
			{
				vector<UINT> receivers;
				cEntityManager::Get()->GetIdentityGroup(eIdGroup::CHARACTER_TOOL, receivers);

				//새 액터를 관여된 객체들에게 전달
				cMessageDispatcher::Get()->DispatchMessages(0, GetID(), receivers, eMessageType::SET_FREE_POINT, nullptr);

				cDebug::Log("Set Free Point Camera!");
			}

			//고정 시점
			if (ImGui::MenuItem("Edited Actor Third Point", ""))
			{
				vector<UINT> receivers;
				cEntityManager::Get()->GetIdentityGroup(eIdGroup::CHARACTER_TOOL, receivers);

				//새 액터를 관여된 객체들에게 전달
				cMessageDispatcher::Get()->DispatchMessages(0, GetID(), receivers, eMessageType::SET_EDITED_ACTOR_THIRD_POINT, nullptr);

				cDebug::Log("Set Edited Actor Camera!");
			}

			//고정 시점
			if (ImGui::MenuItem("Enemy Actor Third Point", ""))
			{
				vector<UINT> receivers;
				cEntityManager::Get()->GetIdentityGroup(eIdGroup::CHARACTER_TOOL, receivers);

				//새 액터를 관여된 객체들에게 전달
				cMessageDispatcher::Get()->DispatchMessages(0, GetID(), receivers, eMessageType::SET_ENEMY_ACTOR_THIRD_POINT, nullptr);

				cDebug::Log("Set Enemy Actor Camera!");
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void UI::cCameraMenu::HandleMessage(const sTelegram & msg)
{
	if (msgMap[msg.message])
		msgMap[msg.message](msg);
}

void UI::cCameraMenu::FunctionInitialize()
{
}