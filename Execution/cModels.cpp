#include "stdafx.h"
#include "cModels.h"
#include "./Model/cModel.h"
#include "./Model/FbxExporter/Exporter.h"
#include "./Model/cModelFactory.h"
#include "./Transform/cGizmo.h"

cModels::cModels(weak_ptr<sGlobalVariable> global)
	:_global(global)
{
	cModelFactory::Get();
	cGizmo::Get()->SetGlobalVariable(global);
	//cDebug::SetDebugMode(false);
}

cModels::~cModels()
{
}

void cModels::Init()
{
	/*auto exporter = new Fbx::Exporter(Asset + L"Dude/Dude.fbx");
	exporter->ExportMaterial(Model + L"Dude/", L"Dude");
	exporter->ExportMesh(Model + L"Dude/", L"Dude");*/

	model = cModelFactory::Get()->Create(Model + L"Dude/Dude");
	cGizmo::Get()->AddTransform(model->GetTransform());
}

void cModels::Update()
{
	model->Update();
	cGizmo::Get()->Update();
}

void cModels::Render()
{
	model->Render();
	cGizmo::Get()->Render();
}

void cModels::PostRender()
{
	cGizmo::Get()->PostRender();
}