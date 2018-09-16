#include "stdafx.h"
#include "cUI.h"
#include "./UI/Gizmo/cGizmo.h"
#include "./UI/MenuBar/cActorMenu.h"
#include "./UI/MenuBar/cCameraMenu.h"
#include "./UI/ToolWindow/cToolWindow.h"

cUI::cUI(weak_ptr<sGlobalVariable> global)
	:_global(global)
{
	_toolWindow = make_shared<UI::cToolWindow>();
	_actorMenu = make_shared<UI::cActorMenu>();
	_cameraMenu = make_shared<UI::cCameraMenu>();
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
	_cameraMenu->Init();
}

void cUI::Update()
{
	cGizmo::Get()->Update();
	//_toolWindow->Update();
	_actorMenu->Update();
	_cameraMenu->Update();
}

void cUI::Render()
{
	_toolWindow->Render();
}

void cUI::PostRender()
{
	_actorMenu->PostRender();
	_cameraMenu->PostRender();
	_toolWindow->PostRender();
}