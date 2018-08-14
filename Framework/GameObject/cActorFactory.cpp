#include "stdafx.h"
#include "cActorFactory.h"
#include "cActor.h"
#include "./Transform/sTransform.h"
#include "./Model/cModel.h"
#include "./Model/cModelFactory.h"
#include "./Model/ModelPart/cModelMesh.h"
#include "./Collider/cActorColliders.h"
#include "./Animator/cAnimator.h"
#include "./Animator/cAnimClip.h"
#include "./BehaviorTree/cBehaviorTree.h"
#include "./Helper/cString.h"
#include "./Helper/cPath.h"

cActorFactory::cActorFactory()
{
	//Ʈ�� ����ü
	struct make_shared_enabler : public cActor {};
	_actor = make_shared<make_shared_enabler>();
	_actor->Init();
}

cActorFactory::~cActorFactory()
{
}

shared_ptr<cActor> cActorFactory::CreateActor()
{
	auto actor = _actor;
	_actor.reset();
	return actor;
}

shared_ptr<cActor> cActorFactory::CreateActor(wstring jsonPath)
{
	Json::Value root;
	Json::ReadData(jsonPath, &root);

	//Name
	string name = root["ActorName"].asString();
	_actor->SetName(cString::Wstring(name));

	CreateTransform(root);
	CreateModel(root);
	CreateCollider(root);
	CreateAnimator(root);

	//todo : behaviortree
	//todo : spec

	auto actor = _actor;
	_actor.reset();
	return actor;
}

void cActorFactory::CreateTransform(Json::Value & root)
{
	auto& scale = _actor->GetTransform().lock()->Scaling;
	Json::GetValue(root["Transform"], "Scale", scale);
}

void cActorFactory::CreateModel(Json::Value& root)
{
	string filePath;
	Json::GetValue(root["Model"], "FilePath", filePath);

	auto path = cString::Wstring(cPath::GetDirectoryName(filePath));
	auto name = cString::Wstring(cPath::GetFileNameWithoutExtension(filePath));

	shared_ptr<cModel> model = cModelFactory::Create(path, name);
	_actor->SetModel(model);
}

void cActorFactory::CreateCollider(Json::Value& root)
{
	for (auto collider : root["Colliders"])
	{
		//�θ� �� �ҷ���
		string boneName;
		Json::GetValue(collider, "ParentName", boneName);
		auto bone = _actor->GetModel().lock()->GetBone(cString::Wstring(boneName));

		//��� ������
		int shape;
		Json::GetValue(collider, "Shape", shape);
		auto eShape = static_cast<eColliderShape>(shape);

		//Ÿ�� ������
		int type;
		Json::GetValue(collider, "Type", type);
		auto eType = static_cast<eColliderType>(type);

		//���� Ʈ������ ������
		D3DXMATRIX mat;
		Json::GetValue(collider, "LocalTM", mat);

		//�ݶ��̴� ���丮���� ����
		auto colliders = _actor->GetColliders().lock();
		colliders->AddCollider(eType, eShape, bone, mat);
	}
}

void cActorFactory::CreateAnimator(Json::Value& root)
{
	auto bones = _actor->GetModel().lock()->GetBones();
	auto animator = _actor->GetAnimator().lock();

	for (auto&& animation : root["Animations"])
	{
		string fullPath = animation.asString();
		wstring path = cString::Wstring(cPath::GetDirectoryName(fullPath));

		wstring name = cString::Wstring(cPath::GetFileName(fullPath));

		auto clip = cAnimClip::Create(path, name);

		if (clip->IsCorrectBones(bones))
			animator->AddClip(clip);
	}
}

void cActorFactory::CreateBehaviorTree(Json::Value& root)
{
	//todo : �ൿƮ�� �Ľ�
}

void cActorFactory::CreateAction()
{
}