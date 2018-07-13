#include "stdafx.h"
#include "cUI.h"
#include "./UI/Gizmo/cGizmo.h"
#include "./UI/MenuBar/cActorMenu.h"
#include "./UI/ToolWindow/cToolWindow.h"

cUI::cUI(weak_ptr<sGlobalVariable> global)
	:_global(global)
{
	_toolWindow = make_shared<UI::cToolWindow>();
	_actorMenu = make_shared<UI::cActorMenu>();
}

cUI::~cUI()
{
	cGizmo::Delete();
}

void cUI::Init()
{
	cGizmo::Get()->SetGlobalVariable(_global);
	_toolWindow->Init();
	_actorMenu->Init();
}

void cUI::Update()
{
	_toolWindow->Update();
	_actorMenu->Update();
}

void cUI::Render()
{
	_toolWindow->Render();
}

void cUI::PostRender()
{
	_toolWindow->PostRender();
	_actorMenu->PostRender();
}