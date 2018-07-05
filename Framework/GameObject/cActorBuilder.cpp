#include "stdafx.h"
#include "cActorBuilder.h"
#include "cActor.h"
#include "./Transform/sTransform.h"
#include "./Model/cModel.h"
#include "./Model/cModelFactory.h"
#include "./Model/cModelAnimPlayer.h"
#include "./Model/ModelPart/cModelMesh.h"

#include "./BehaviorTree/cBehaviorTree.h"

//test : colliderfactory로 바꾸기
#include "./Collider/cCylinderCollider.h"
#include "./Model/ModelPart/cModelMesh.h"

cActorBuilder::cActorBuilder()
{
	//트릭 구조체
	struct make_shared_enabler : public cActor {};
	_actor = make_shared<make_shared_enabler>();
}

cActorBuilder::~cActorBuilder()
{
	cModelFactory::Delete();
}

shared_ptr<cActor> cActorBuilder::CreateActor()
{
	auto actor = _actor;
	_actor.reset();
	return actor;
}

cActorBuilder & cActorBuilder::CreateTransform()
{
	_actor->_transform = make_shared<sTransform>();
	_actor->_transform->Scaling = { 0.1f, 0.1f, 0.1f };

	return *this;
}

cActorBuilder & cActorBuilder::CreateBehaviorTree()
{
	_actor->_behaviorTree = make_unique<cBehaviorTree>(_actor);
	return *this;
}

cActorBuilder & cActorBuilder::CreateModel()
{
	vector<wstring> anims{ L"paladin_idle", L"paladin_walk", L"paladin_run", L"paladin_attack" };

	auto model = cModelFactory::Get()->Create(Model + L"paladin/", L"paladin", anims);
	_actor->_model = move(model);

	_actor->_anim = make_unique<cModelAnimPlayer>(_actor->_model);

	return *this;
}

cActorBuilder & cActorBuilder::CreateAction()
{
	return *this;
}

cActorBuilder & cActorBuilder::CreateCollider()
{
	auto transform = _actor->_model->GetMeshes()[0]->GetParentTransform();
	//test : colliderfactory에서 생성하기
	auto col = make_shared<cCylinderCollider>(transform, _actor->_model->GetMeshes()[0]->GetMeshPoint(), 1, 0.5f);

	_actor->_colliders.emplace_back(move(col));

	return *this;
}
