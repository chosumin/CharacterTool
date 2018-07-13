#include "stdafx.h"
#include "cToolWindow.h"
#include "./GameObject/cActor.h"
#include "./Message/cEntityManager.h"
#include "./Message/cMessageDispatcher.h"
#include "./UI/Gizmo/cGizmo.h"

#include "./Transform/cTransformTool.h"
#include "./Model/cModelTool.h"

UI::cToolWindow::cToolWindow()
{
	_tools.emplace_back(make_shared<cTransformTool>());
	_tools.emplace_back(make_shared<cModelTool>());
}

UI::cToolWindow::~cToolWindow()
{
	cEntityManager::Get()->RemoveEntity(GetID());
}

void UI::cToolWindow::Init()
{
	cEntityManager::Get()->RegisterEntity(eIdGroup::CharacterTool, shared_from_this());

	for (auto&& tool : _tools)
		tool->Init();
}

void UI::cToolWindow::Update()
{
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
	ImGui::Begin("Hierarchy");
	{	
		for (auto&& tool : _tools)
			tool->ShowHierarchy();
	}
	ImGui::End();

	ImGui::Begin("Inspector");
	{
		if (_selectedTool.expired() == false)
			_selectedTool.lock()->ShowInspector();
	}
	ImGui::End();
}

void UI::cToolWindow::HandleMessage(const sTelegram & msg)
{
	switch (msg.message)
	{
		case eMessageType::RecieveActor:
			RecieveActor(msg);
		break;
		case eMessageType::RecieveTool:
			RecieveTool(msg);
		break;
	}
}

void UI::cToolWindow::FunctionInitialize()
{
}

void UI::cToolWindow::RecieveActor(const sTelegram & msg)
{
	_actor = *(weak_ptr<cActor>*)(msg.extraInfo);

	for (auto&& tool : _tools)
		tool->SetActor(_actor);
}

void UI::cToolWindow::RecieveTool(const sTelegram & msg)
{
	_selectedTool = *(weak_ptr<cTool>*)(msg.extraInfo);
}
