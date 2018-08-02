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
	cGizmo::Get()->Update();
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

		//������ ���� �ڽ��� ����
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
	//todo : �����ϸ��� ����ǵ���
	_actor.lock()->GetTransform().lock()->Scaling;
	Json::Value transform;
	Json::SetValue(transform, "Scale", )
	root["Transform"]
	Json::SetValue(root,)
	
}

void UI::cTransformTool::LoadJson(Json::Value& root)
{
}
