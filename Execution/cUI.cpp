#include "stdafx.h"
#include "cUI.h"
#include "./UI/Inspector/Transform/cGizmo.h"
#include "./Model/cModelAnimPlayer.h"

cUI::cUI(weak_ptr<sGlobalVariable> global)
	:_global(global)
{
}

cUI::~cUI()
{
	cGizmo::Delete();
	//cModelAnimPlayer::Delete();
}

void cUI::Init()
{
	//todo : 애님플레이어 메시지로 처리
	cGizmo::Get()->SetGlobalVariable(_global);
}

void cUI::Update()
{
	cGizmo::Get()->Update();
	//cModelAnimPlayer::Get()->Update();
}

void cUI::Render()
{
	//cModelAnimPlayer::Get()->Render();
	cGizmo::Get()->Render();
}

void cUI::PostRender()
{
	ImGui::Begin("Inspector");
	{
		ImGui::Separator();
		cGizmo::Get()->PostRender();
		ImGui::Separator();
		//cModelAnimPlayer::Get()->PostRender();
		//ImGui::Separator();
	}
	ImGui::End();
}