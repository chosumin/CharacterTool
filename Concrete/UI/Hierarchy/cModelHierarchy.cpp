#include "stdafx.h"
#include "cModelHierarchy.h"
#include "./GameObject/cActor.h"
#include "./UI/Inspector/Transform/cGizmo.h"
#include "./Model/ModelPart/cModelMesh.h"
#include "./Model/cModel.h"
#include "./Helper/cString.h"

cModelHierarchy::cModelHierarchy(weak_ptr<cActor> actor, ImGuiTreeNodeFlags open)
	:_actor(actor)
	,_open(open)
{
}

cModelHierarchy::~cModelHierarchy()
{
}

void cModelHierarchy::SetActor(weak_ptr<cActor> actor)
{
	_actor = actor;
}

void cModelHierarchy::Update()
{
}

void cModelHierarchy::PostRender()
{
	if (ImGui::TreeNode(""))
		ImGui::TreePop();
	ImGui::SameLine();

	if(ImGui::Selectable("Transform"))
		ShowTransform();

	if (ImGui::TreeNode("Model"))
	{
		ShowModel();
		ImGui::TreePop();
	}
}

void cModelHierarchy::ShowTransform()
{
	if (AlertActor())
		return;

	auto actorPtr = _actor.lock();
	if (actorPtr->GetTransform().expired())
		return;

	cGizmo::Get()->AddTransform(actorPtr->GetTransform());
}

void cModelHierarchy::ShowModel()
{
	if (AlertActor())
		return;

	auto actorPtr = _actor.lock();
	_model = actorPtr->GetModel();

	if (AlertModel())
		return;

	auto meshes = _model.lock()->GetMeshes();
	ShowMeshes(meshes);
}

void cModelHierarchy::ShowMeshes(const vector<shared_ptr<cModelMesh>>& meshes)
{
	for (auto&& mesh : meshes)
	{
		auto name = cString::String(mesh->GetName());
		if (ImGui::Selectable(name.c_str()))
		{
			cDebug::Log("mesh selected");
		}
	}
}

bool cModelHierarchy::AlertActor()
{
	bool expired = _actor.expired();
	if (expired)
		cDebug::Log("Add Actor First!");

	return expired;
}

bool cModelHierarchy::AlertModel()
{
	if (_model.expired())
	{
		cDebug::Log("Add Model First!");
		return true;
	}

	return false;
}
