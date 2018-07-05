#include "stdafx.h"
#include "cModels.h"
#include "./Model/cModelAnimPlayer.h"
#include "./GameObject/cActor.h"
#include "./GameObject/cActorBuilder.h"

#include "./Collider/cCylinderCollider.h"

#include "./Transform/sTransform.h"

#include "./Model/FbxExporter/Exporter.h"

cModels::cModels(weak_ptr<sGlobalVariable> global)
	:_global(global)
{
	cDebug::SetDebugMode(true);
}

cModels::~cModels()
{
	SAFE_DELETE(col);
}

void cModels::Init()
{
	/*auto exporter = Fbx::Exporter(Asset + L"paladin/paladin.fbx");
	exporter.ExportMaterial(Model + L"paladin/", L"paladin");
	exporter.ExportMesh(Model + L"paladin/", L"paladin");*/
	
	cActorBuilder builder;
	builder.CreateTransform().CreateModel().CreateBehaviorTree().CreateCollider();
	_actor = builder.CreateActor();

	col = new cCylinderCollider(weak_ptr<sTransform>(), { 0,0,0 }, 1, 0.5f);
}

void cModels::Update()
{
	_actor->Update();
	col->Update();
	
}

void cModels::Render()
{
	_actor->Render();
	col->Render();
}

void cModels::PostRender()
{
}