#include "stdafx.h"
#include "cToolWindow.h"
#include "./GameObject/cActor.h"
#include "./Message/cEntityManager.h"
#include "./Message/cMessageDispatcher.h"
#include "./UI/Gizmo/cGizmo.h"

#include "./Transform/cTransformTool.h"
#include "./Model/cModelTool.h"
#include "./Collider/cColliderTool.h"
#include "./Animation/cAnimTool.h"
#include "./Helper/Json.h"

UI::cToolWindow::cToolWindow()
{
	auto modelTool = make_shared<cModelTool>();
	_tools.emplace_back(make_shared<cTransformTool>());
	_tools.emplace_back(modelTool);
	_tools.emplace_back(make_shared<cColliderTool>(modelTool));
	_tools.emplace_back(make_shared<cAnimTool>(modelTool));
}

UI::cToolWindow::~cToolWindow()
{
	cEntityManager::Get()->RemoveEntity(GetID());
}

void UI::cToolWindow::Init()
{
	FunctionInitialize();
	cEntityManager::Get()->RegisterEntity(eIdGroup::CharacterTool, shared_from_this());

	for (auto&& tool : _tools)
		tool->Init();
}

void UI::cToolWindow::Update()
{
	//fixme : 툴을 모두 업데이트 렌더할지, 아니면 선택된 툴만 업데이트 렌더할지
	for (auto&& tool : _tools)
		tool->Update();
}

void UI::cToolWindow::Render()
{
	for (auto&& tool : _tools)
		tool->Render();
}

void UI::cToolWindow::PostRender()
{
	bool flag = true;
	ImGui::Begin("Hierarchy", &flag, ImGuiWindowFlags_HorizontalScrollbar);
	{
		for (UINT i = 0; i < _tools.size(); i++)
			_tools[i]->ShowHierarchy(i + 1);
	}
	ImGui::End();

	bool flag2 = true;
	ImGui::Begin("Inspector");
	{
		if (_selectedTool.expired() == false)
			_selectedTool.lock()->ShowInspector();
	}
	ImGui::End();
}

void UI::cToolWindow::HandleMessage(const sTelegram & msg)
{
	if (msgMap[msg.message])
		msgMap[msg.message](msg);
}

void UI::cToolWindow::FunctionInitialize()
{
	msgMap[eMessageType::RECIEVE_ACTOR] = [this](const sTelegram& msg)
	{
		_actor = *(weak_ptr<cActor>*)(msg.extraInfo);

		for (auto&& tool : _tools)
			tool->SetActor(_actor);
	};

	msgMap[eMessageType::RECIEVE_TOOL] = [this](const sTelegram& msg)
	{
		_selectedTool = *(weak_ptr<cTool>*)(msg.extraInfo);
	};

	msgMap[eMessageType::SAVE_ACTOR] = [this](const sTelegram& msg)
	{
		Json::Value *root = static_cast<Json::Value*>(msg.extraInfo);
		for (auto&& tool : _tools)
		{
			tool->SaveJson(*root);
		}
	};

	msgMap[eMessageType::LOAD_ACTOR] = [this](const sTelegram& msg)
	{
		_actor = *(weak_ptr<cActor>*)(msg.extraInfo);

		for(auto&& tool : _tools)
			tool->SetActor(_actor);

		for (auto&& tool : _tools)
			tool->LoadJson();
	};
}