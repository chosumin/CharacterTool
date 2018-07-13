#include "stdafx.h"
#include "cColliderInspector.h"
#include "./GameObject/cActor.h"
#include "./Model/ModelPart/cModelMesh.h"
#include "./Collider/cCylinderCollider.h"
#include "./Helper/cString.h"

cColliderInspector::cColliderInspector(weak_ptr<cActor> actor)
	:_actor(actor)
{
}

cColliderInspector::~cColliderInspector()
{
}

void cColliderInspector::Update()
{
}

void cColliderInspector::Render()
{
}

void cColliderInspector::PostRender()
{
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "Collider");
	ImGui::Text("Mesh : ");

	if (_mesh.expired())
		return;

	auto meshPtr = _mesh.lock();
	auto meshName = cString::String(meshPtr->GetName());
	ImGui::SameLine();
	ImGui::Text(meshName.c_str());

	if (ImGui::Button("Create"))
	{
		
	}
}

void cColliderInspector::SetMesh(weak_ptr<cModelMesh> mesh)
{
	_mesh = mesh;
}
