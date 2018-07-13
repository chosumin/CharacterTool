#include "stdafx.h"
#include "cModels.h"
#include "./Model/cModelAnimPlayer.h"
#include "./GameObject/cActor.h"
#include "./GameObject/cActorBuilder.h"

#include "./Collider/cCylinderCollider.h"

#include "./Transform/sTransform.h"

#include "./Model/FbxExporter/Exporter.h"
#include "./UI/Hierarchy/cHierarchy.h"

cModels::cModels(weak_ptr<sGlobalVariable> global)
	:_global(global)
{
	cDebug::SetDebugMode(true);
}

cModels::~cModels()
{
}

void cModels::Init()
{
	cActorBuilder builder;
	builder.CreateTransform().CreateModel().CreateBehaviorTree(0).CreateCollider();
	_actor = builder.CreateActor();

	cActorBuilder builder2;
	builder2.CreateTransform().CreateModel().CreateBehaviorTree(1).CreateCollider();
	_actor2 = builder2.CreateActor();
}

void cModels::Update()
{
	_actor->Update();
	_actor2->Update();

	if(_actor->GetCollider(1).lock()->IntersectsWith(_actor2->GetCollider(0)))
	{
		_actor2->isDamage = true;
		_actor2->time = timeGetTime();
	}
}

void cModels::Render()
{
	_actor->Render();
	_actor2->Render();
}

void cModels::PostRender()
{
}