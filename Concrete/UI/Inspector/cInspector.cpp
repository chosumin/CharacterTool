#include "stdafx.h"
#include "cInspector.h"
#include "./Collider/cColliderInspector.h"
#include "./Model/ModelPart/cModelMesh.h"
#include "./Transform/cGizmo.h"
#include "./GameObject/cActor.h"

#include "./Message/cEntityManager.h"
#include "./Message/cMessageDispatcher.h"

cInspector::cInspector()
{
}

cInspector::~cInspector()
{
	cEntityManager::Get()->RemoveEntity(GetID());
	cGizmo::Delete();
}

void cInspector::Init()
{
	cEntityManager::Get()->RegisterEntity(eIdGroup::CharacterTool, shared_from_this());
}

void cInspector::Update()
{
	cGizmo::Get()->Update();
}

void cInspector::Render()
{
	cGizmo::Get()->Render();
}

void cInspector::PostRender()
{
	ImGui::Begin("Inspector");
	{
		cGizmo::Get()->PostRender();
		//_colInspector->PostRender();
	}
	ImGui::End();
}

void cInspector::SetMesh(weak_ptr<cModelMesh> mesh)
{
	//_colInspector->SetMesh(mesh);
}

void cInspector::HandleMessage(const sTelegram & msg)
{
	switch (msg.message)
	{
		case eMessageType::RecieveActor:
		{
			_actor = *(weak_ptr<cActor>*)(msg.extraInfo);
		}
		break;
	}
}

void cInspector::FunctionInitialize()
{
}
