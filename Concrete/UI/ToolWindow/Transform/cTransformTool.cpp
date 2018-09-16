#include "stdafx.h"
#include "cTransformTool.h"
#include "./GameObject/cActor.h"
#include "./UI/Gizmo/cGizmo.h"
#include "./Helper/Json.h"
#include "./Transform/sTransform.h"

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
}

void UI::cTransformTool::Render()
{
	cGizmo::Get()->Render();
}

void UI::cTransformTool::ShowHierarchy(int i)
{
	if (ImGui::TreeNode(to_string(i).c_str()))
		ImGui::TreePop();
	ImGui::SameLine();

	if (ImGui::Selectable("Transform"))
	{
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

	cDebug::Log("Transform Selected!");
	cGizmo::Get()->AddTransform(actorPtr->GetTransform());
}

void UI::cTransformTool::SaveJson(Json::Value& root)
{
	//스케일링만 저장되도록
	auto& scale = _actor.lock()->GetTransform().lock()->Scaling;
	Json::Value transform;
	Json::SetValue(transform, "Scale", scale);
	root["Transform"] = transform;
}

void UI::cTransformTool::LoadJson()
{
	//DO NOTHING
}
