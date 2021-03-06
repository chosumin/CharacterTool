#include "stdafx.h"
#include "cActorMenu.h"
#include "./GameObject/cActor.h"
#include "./GameObject/cActorFactory.h"
#include "./Helper/cPath.h"
#include "./Helper/cString.h"
#include "./Concrete/Message/eIdGroup.h"
#include "./Concrete/Message/eMessageType.h"
#include "./Model/cModel.h"

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
	cEntityManager::Get()->RegisterEntity(eIdGroup::CHARACTER_TOOL, shared_from_this());

	FunctionInitialize();
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
			{
				AlertActor();
				NewEditedActor();
			}
			if (ImGui::MenuItem("Load Actor", ""))
			{
				AlertActor();
				LoadEditedActor();
			}
			if (ImGui::MenuItem("Save Actor", ""))
				SaveEditedActor();

			ImGui::Separator();

			if (ImGui::MenuItem("Load Enemy", ""))
				LoadEnemy();

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	ImGui::Begin("Hierarchy", nullptr, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	{
		ImGui::TextColored(ImVec4(1, 1, 0, 1), _actorName.c_str());
	}
	ImGui::End();
}

void UI::cActorMenu::HandleMessage(const sTelegram & msg)
{
	if(msgMap[msg.message])
		msgMap[msg.message](msg);
}

void UI::cActorMenu::FunctionInitialize()
{
	msgMap[eMessageType::RECIEVE_ACTOR] = [this](const sTelegram& msg)
	{
		_actor = *(weak_ptr<cActor>*)(msg.extraInfo);
	};

	msgMap[eMessageType::LOAD_ACTOR] = [this](const sTelegram& msg)
	{
		_actor = *(weak_ptr<cActor>*)(msg.extraInfo);
	};
}

void UI::cActorMenu::NewEditedActor()
{
	cActorFactory factory;
	auto actor = factory.CreateActor();
	
	CreateActor(actor, eMessageType::RECIEVE_ACTOR);
}

void UI::cActorMenu::LoadEditedActor()
{
	cPath::OpenFileDialog(L"Open Actor", L"Actor\0*.actor", Content, [this](wstring path)
	{
		cActorFactory factory;
		auto actor = factory.CreateActor(path);

		CreateActor(actor, eMessageType::LOAD_ACTOR);

		_actorName = cString::String(_actor.lock()->GetName());
		cDebug::Log((_actorName + " has been created!").c_str());
	});
}

void UI::cActorMenu::CreateActor(weak_ptr<cActor> actor, eMessageType type)
{
	vector<UINT> receivers;
	cEntityManager::Get()->GetIdentityGroup(eIdGroup::CHARACTER_TOOL, receivers);

	//새 액터를 관여된 객체들에게 전달
	cMessageDispatcher::Get()->DispatchMessages(0, GetID(), receivers, type, (void*)&actor);
}

void UI::cActorMenu::SaveEditedActor()
{
	if (_actor.expired())
	{
		//todo : 메시지 박스
		cDebug::Log("Create Actor First!");
		return;
	}

	cPath::SaveFileDialog(L"Save Actor", L"Actor\0*.actor", Content, [this](wstring path)
	{
		Json::Value root;

		//툴에게 저장 메시지 보냄
		vector<UINT> receivers;
		cEntityManager::Get()->GetIdentityGroup(eIdGroup::CHARACTER_TOOL, receivers);

		cMessageDispatcher::Get()->DispatchMessages(0, GetID(), receivers, eMessageType::SAVE_ACTOR, &root);

		//저장 경로
		wstring temp = cPath::GetDirectoryName(path) + cPath::GetFileNameWithoutExtension(path);
		temp += L".actor";

		_actor.lock()->SetName(cPath::GetFileNameWithoutExtension(path));

		_actorName = cString::String(cPath::GetFileNameWithoutExtension(path));

		root["ActorName"] = _actorName;

		//저장
		Json::WriteData(temp, &root);
	});

	//todo : 메시지 박스 띄우기
	cDebug::Log(("Save Success! : " + _actorName).c_str());
}

void UI::cActorMenu::LoadEnemy()
{
	cPath::OpenFileDialog(L"Open Enemy", L"Actor\0*.actor", Content, [this](wstring path)
	{
		cActorFactory factory;
		auto actor = factory.CreateActor(path);

		CreateActor(actor, eMessageType::LOAD_ENEMY);
	});
}

bool UI::cActorMenu::AlertActor()
{
	if (_actor.expired() == false)
	{
		//todo : 액터가 존재하면 팝업창
		return true;
	}

	return false;
}
