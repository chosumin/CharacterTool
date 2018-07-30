#include "stdafx.h"
#include "cActorBuilder.h"
#include "cActor.h"
#include "./Transform/sTransform.h"
#include "./Model/cModel.h"
#include "./Model/cModelFactory.h"
#include "./Model/ModelPart/cModelMesh.h"

#include "./BehaviorTree/cBehaviorTree.h"

cActorBuilder::cActorBuilder()
{
	//트릭 구조체
	struct make_shared_enabler : public cActor {};
	_actor = make_shared<make_shared_enabler>();
	_actor->Init();
}

cActorBuilder::~cActorBuilder()
{
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

	return *this;
}

cActorBuilder & cActorBuilder::CreateBehaviorTree(int i)
{
	_actor->_behaviorTree = make_unique<cBehaviorTree>(_actor, i);
	return *this;
}

cActorBuilder & cActorBuilder::CreateModel()
{
	vector<wstring> anims{ L"paladin_idle", L"paladin_walk", L"paladin_run", L"paladin_attack", L"paladin_impact" };

	auto model = cModelFactory::Create(Model + L"paladin/", L"paladin");
	_actor->_model = move(model);

	return *this;
}

cActorBuilder & cActorBuilder::CreateAction()
{
	return *this;
}

cActorBuilder & cActorBuilder::CreateCollider()
{
	//auto transform = _actor->_model->GetMeshes()[0]->GetParentTransform();
	////test : colliderfactory에서 생성하기
	//auto col = make_shared<cCylinderCollider>(transform, 1.0f, 0.5f);
	//_actor->_colliders.emplace_back(move(col));

	//auto transform2 = _actor->_model->GetMeshes()[2]->GetParentTransform();
	//auto col2 = make_shared<cCylinderCollider>(transform2, 1.0f, 0.5f);
	//_actor->_colliders.emplace_back(move(col2));
	//
	return *this;
}
