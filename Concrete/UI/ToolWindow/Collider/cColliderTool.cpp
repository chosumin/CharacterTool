#include "stdafx.h"
#include "cColliderTool.h"
#include "../Model/cModelTool.h"
#include "./Model/cModel.h"
#include "./Model/ModelPart/cModelMesh.h"
#include "./GameObject/cActor.h"
#include "./UI/Gizmo/cGizmo.h"
#include "./Collider/cActorColliders.h"
#include "./Collider/cCollider.h"
#include "./Helper/cString.h"
#include "./Helper/cMath.h"

UI::cColliderTool::cColliderTool(weak_ptr<cModelTool> modelTool)
	:_modelTool(modelTool),
	_selectAttack(true)
{
}

UI::cColliderTool::~cColliderTool()
{
}

void UI::cColliderTool::Init()
{
	_modelTool.lock()->RegisterObserver(iObserver::downcasted_shared_from_this<cColliderTool>());
	cTool::Init();
}

void UI::cColliderTool::Update()
{
	auto actorPtr = _actor.lock();
	if(actorPtr)
		_colliders = actorPtr->GetColliders();
}

void UI::cColliderTool::Render()
{
}

void UI::cColliderTool::ShowHierarchy(int i)
{
	if (ImGui::TreeNode(to_string(i).c_str()))
	{
		SelectCollider();
		ImGui::TreePop();
	}
	else
		SelectCollider();
}

void UI::cColliderTool::SelectCollider()
{
	ImGui::SameLine();
	if (ImGui::Selectable("Collider"))
	{
		if (Alert())
			return;

		SendMe();
		cDebug::Log("Collider Selected!");
	}
}

void UI::cColliderTool::ShowInspector()
{
	if (ImGui::CollapsingHeader("Transform"))
		cGizmo::Get()->PostRender();

	if (ImGui::CollapsingHeader("Collider Header"))
		ShowAddWindowInspector();

	int index = 1;
	if (ImGui::CollapsingHeader("Attack Colliders"))
		ShowColliderInspector(eColliderType::ATTACK, index);

	if (ImGui::CollapsingHeader("Damage Colliders"))
		ShowColliderInspector(eColliderType::DAMAGE, index);
}

void UI::cColliderTool::ShowAddWindowInspector()
{
	//선택된 본
	ImGui::Text("Mesh : "); ImGui::SameLine();
	auto mesh = _selectedMesh.lock();
	string meshName = mesh ? cString::String(mesh->GetName()) : "None";
	ImGui::Text(meshName.c_str());

	//선택된 모양
	ImGui::Text("Shape : "); ImGui::SameLine();

	//hack : 충돌체 늘어날수록 숫자 늘려줘야함
	ImGui::Combo("", &_shapeNumber, &cColliderFactory::GetList()[0], 3);

	//공격, 피격 선택
	ImGui::Checkbox("Offensive", &_selectAttack);

	if (ImGui::Button("Add"))
	{
		if (!MeshAlert())
			AddCollider();
	}
}

void UI::cColliderTool::AddCollider()
{
	eColliderShape shape = static_cast<eColliderShape>(_shapeNumber);

	auto colliders = _actor.lock()->GetColliders().lock();
	colliders->AddCollider(_selectAttack,
						   shape,
						   _selectedMesh.lock(),
						   cMath::MATRIX_IDENTITY);

	cDebug::Log("Collider Added!");
}

void UI::cColliderTool::ShowColliderInspector(eColliderType type, int& index)
{
	auto meshes = _model.lock()->GetMeshes();
	for (auto&& mesh : meshes)
	{
		auto colPtr = mesh->GetCollider().lock();
		if (!colPtr)
			continue;

		if (colPtr->GetType() == type)
		{
			if (ImGui::Selectable(("No." + to_string(index++) + " Collider").c_str(), true, 0, ImVec2(150, 20)))
			{
				auto transform = colPtr->GetLocalTransform();
				auto world = colPtr->GetWorldTransform();
				cGizmo::Get()->AddTransform(transform, world);
			}

			ImGui::SameLine();
			if(ImGui::Button(("Delete " + to_string(index - 1)).c_str()))
			{
				//삭제
				mesh->SetCollider(weak_ptr<cCollider>());
				continue;
			}

			ImGui::Text("Mesh : %s", cString::String(mesh->GetName()).c_str());

			auto shape = cColliderFactory::GetList()[static_cast<int>(colPtr->GetShape())];
			ImGui::Text("Shape : %s", shape);

			ImGui::Separator();
		}
	}
}

void UI::cColliderTool::SelectBone(weak_ptr<cModelBone> bone)
{
}

void UI::cColliderTool::SelectMesh(weak_ptr<cModelMesh> mesh)
{
	_selectedMesh = mesh;
}

void UI::cColliderTool::ChangeModel(weak_ptr<cModel> newModel)
{
	_model = newModel;
	_selectedMesh.reset();
}

bool UI::cColliderTool::Alert()
{
	if (AlertActor())
		return true;

	if (_model.expired())
	{
		cDebug::Log("Add Model First!");
		return true;
	}

	return false;
}

bool UI::cColliderTool::MeshAlert()
{
	if (Alert())
		return true;

	if (_selectedMesh.expired())
	{
		cDebug::Log("Select Mesh First!");
		return true;
	}

	return false;
}