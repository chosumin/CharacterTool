#include "stdafx.h"
#include "cColliderTool.h"
#include "../Model/cModelTool.h"
#include "./Model/cModel.h"
#include "./Model/ModelPart/cModelMesh.h"
#include "./Model/ModelPart/cModelBone.h"
#include "./GameObject/cActor.h"
#include "./Transform/sTransform.h"
#include "./UI/Gizmo/cGizmo.h"
#include "./Collider/cActorColliders.h"
#include "./Collider/cCollider.h"
#include "./Helper/cString.h"
#include "./Helper/cMath.h"

UI::cColliderTool::cColliderTool(weak_ptr<cModelTool> modelTool)
	: _modelTool(modelTool)
	, _selectAttack(true)
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

	//����� �浹ü ����
	auto collidersPtr = _colliders.lock();
	if (collidersPtr)
	{
		collidersPtr->DeleteCollider();
	}
}

void UI::cColliderTool::Render()
{
	//���Ϳ����� �ݶ��̴� ������ X, ���⼭ ���� �ݶ��̴� ������ ó��
	auto colPtr = _colliders.lock();
	if (colPtr)
		colPtr->Render();
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
		ShowColliderInspector(eColliderType::COMBO_ATTACK, index);

	if (ImGui::CollapsingHeader("Damage Colliders"))
		ShowColliderInspector(eColliderType::DAMAGE, index);
}

void UI::cColliderTool::ShowAddWindowInspector()
{
	//���õ� ��
	ImGui::Text("Bone : "); ImGui::SameLine();
	auto bone = _selectedBone.lock();
	string boneName = bone ? cString::String(bone->GetName()) : "None";
	ImGui::Text(boneName.c_str());

	//���õ� ���
	ImGui::Text("Shape : "); ImGui::SameLine();

	//hack : �浹ü �þ���� ���� �÷������
	ImGui::Combo("", &_shapeNumber, &cColliderFactory::GetList()[0], 3);

	//����, �ǰ� ����
	ImGui::Checkbox("Offensive", &_selectAttack);

	if (ImGui::Button("Add"))
	{
		if (!BoneAlert())
			AddCollider();
	}
}

void UI::cColliderTool::AddCollider()
{
	eColliderShape shape = static_cast<eColliderShape>(_shapeNumber);

	auto colliders = _colliders.lock();

	auto type = _selectAttack ? eColliderType::COMBO_ATTACK : eColliderType::DAMAGE;

	colliders->AddCollider(type, shape, _selectedBone,
						   cMath::MATRIX_IDENTITY);

	cDebug::Log("Collider Added!");
}

void UI::cColliderTool::ShowColliderInspector(eColliderType type, int& index)
{
	auto bones = _model.lock()->GetBones();
	for (auto&& bone : bones)
		ShowColliders(type, bone, index);
}

void UI::cColliderTool::ShowColliders(eColliderType type, weak_ptr<cModelBone> bone, int& index)
{
	auto bonePtr = bone.lock();
	auto colliderVec = bonePtr->GetColliders();

	for (auto&& col : colliderVec)
	{
		if (!col)
			continue;

		if (col->GetType() == type)
		{
			ImGui::PushID(index++);
			if (ImGui::Selectable("Collider", true, 0, ImVec2(150, 20)))
			{
				auto transform = col->GetLocalTransform();
				auto world = col->GetWorldTransform();
				cGizmo::Get()->AddTransform(transform, world);
			}

			ImGui::SameLine();

			if (ImGui::Button("Delete"))
				bonePtr->DeleteCollider(col);
			else
			{
				ImGui::Text("Bone : %s", cString::String(bonePtr->GetName()).c_str());

				auto shape = cColliderFactory::GetList()[static_cast<int>(col->GetShape())];
				ImGui::Text("Shape : %s", shape);

				ImGui::Separator();
			}
			ImGui::PopID();
		}
	}
}

void UI::cColliderTool::SelectBone(weak_ptr<cModelBone> bone)
{
	_selectedBone = bone;
}

void UI::cColliderTool::SelectMesh(weak_ptr<cModelMesh> mesh)
{
	//_selectedMesh = mesh;
}

void UI::cColliderTool::ChangeModel(weak_ptr<cModel> newModel)
{
	_model = newModel;
	_selectedBone.reset();
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

bool UI::cColliderTool::BoneAlert()
{
	if (Alert())
		return true;

	if (_selectedBone.expired())
	{
		cDebug::Log("Select Bone First!");
		return true;
	}

	return false;
}

void UI::cColliderTool::SaveJson(Json::Value& root)
{
	Json::Value colliders;

	auto modelPtr = _model.lock();

	//���� ��ȸ
	for (auto&& bone : modelPtr->GetBones())
	{
		string boneName = cString::String(bone->GetName());

		//���� ������ �浹ü ��ȸ
		for (auto&& collider : bone->GetColliders())
		{
			Json::Value colliderJson;

			Json::SetValue(colliderJson, "ParentName", boneName);

			int shape = static_cast<int>(collider->GetShape());
			Json::SetValue(colliderJson, "Shape", shape);

			int type = static_cast<int>(collider->GetType());
			Json::SetValue(colliderJson, "Type", type);

			auto tmPtr = collider->GetLocalTransform().lock();
			Json::SetValue(colliderJson, "LocalTM", tmPtr->Matrix);

			colliders.append(colliderJson);
		}
	}

	root["Colliders"] = colliders;
}

void UI::cColliderTool::LoadJson()
{
	//DO NOTHING
}
