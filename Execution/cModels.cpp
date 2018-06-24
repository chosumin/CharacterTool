#include "stdafx.h"
#include "cModels.h"
#include "./Model/cModel.h"
#include "./Model/FbxExporter/Exporter.h"
#include "./Model/cModelFactory.h"
#include "./Model/cModelAnimPlayer.h"
#include "./Transform/cGizmo.h"
#include "./Hierarchy/cHierarchy.h"

cModels::cModels(weak_ptr<sGlobalVariable> global)
	:_global(global)
{
	cModelFactory::Get();
	cGizmo::Get()->SetGlobalVariable(global);
	cDebug::SetDebugMode(true);
}

cModels::~cModels()
{
	SAFE_DELETE(hierarchy);
	SAFE_DELETE(animPlayer);
}

void cModels::Init()
{
	/*auto exporter = new Fbx::Exporter(Asset + L"Dude/Dude.fbx");
	exporter->ExportMaterial(Model + L"Dude/", L"Dude");
	exporter->ExportMesh(Model + L"Dude/", L"Dude");
	exporter->ExportAnimation(Model + L"Dude/", L"Dude");*/

	model = cModelFactory::Get()->Create(Model + L"Dude/Dude");
	//cGizmo::Get()->AddTransform(model->GetTransform());

	animPlayer = new cModelAnimPlayer(model);
	hierarchy = new cHierarchy();
	hierarchy->PicksModel(model);
}

void cModels::Update()
{
	//model->Update();
	cGizmo::Get()->Update();

	animPlayer->Update();
}

void cModels::Render()
{
	//model->Render();
	cGizmo::Get()->Render();

	animPlayer->Render();
}

void cModels::PostRender()
{
	hierarchy->PostRender();
	cGizmo::Get()->PostRender();
}