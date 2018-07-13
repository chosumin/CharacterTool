#include "stdafx.h"
#include "cTransformTool.h"
#include "./GameObject/cActor.h"
#include "./UI/Gizmo/cGizmo.h"

UI::cTransformTool::cTransformTool()
{
}

UI::cTransformTool::~cTransformTool()
{
}

void UI::cTransformTool::Init()
{
	cTool::Init();
}

void UI::cTransformTool::Update()
{
	cGizmo::Get()->Update();
}

void UI::cTransformTool::Render()
{
	cGizmo::Get()->Render();
}

void UI::cTransformTool::ShowHierarchy()
{
	if (ImGui::TreeNode(""))
		ImGui::TreePop();
	ImGui::SameLine();

	if (ImGui::Selectable("Transform"))
	{
		cDebug::Log("Transform Selected!");
		ShowTransform();

		//윈도우 툴에 자신을 전달
		SendMe();
	}
}

void UI::cTransformTool::ShowInspector()
{
	if (ImGui::CollapsingHeader("Transform"))
		cGizmo::Get()->PostRender();
}

void UI::cTransformTool::ShowTransform()
{
	if (AlertActor())
		return;

	auto actorPtr = _actor.lock();
	if (actorPtr->GetTransform().expired())
		return;

	cGizmo::Get()->AddTransform(actorPtr->GetTransform());
}
