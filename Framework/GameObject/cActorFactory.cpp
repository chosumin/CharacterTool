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
#include "./Blackboard/cBlackboard.h"
#include "./Helper/cString.h"
#include "./Helper/cPath.h"

cActorFactory::cActorFactory()
{
	InitActor();
}

cActorFactory::~cActorFactory()
{
}

shared_ptr<cActor> cActorFactory::CreateActor()
{
	if (_actor)
		return move(_actor);

	return nullptr;
}

shared_ptr<cActor> cActorFactory::CreateActor(wstring jsonPath)
{
	InitActor();

	_jsonPath = jsonPath;

	Json::Value root;
	Json::ReadData(jsonPath, &root);

	//Name
	string name = root["ActorName"].asString();
	_actor->SetName(cString::Wstring(name));

	CreateTransform(root);
	CreateModel(root);
	CreateCollider(root);
	CreateAnimator(root);
	CreateBehaviorTree(root);
	CreateBlackboard(root);

	_actor->TestUpdate();
	_actor->GetModel().lock()->ResetBones();

	return move(_actor);
}

void cActorFactory::InitActor()
{
	_actor.reset();
	//Ʈ�� ����ü
	struct make_shared_enabler : public cActor {};
	_actor = make_shared<make_shared_enabler>();
	_actor->Init();
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

	float speed;
	Json::GetValue(root["Animations"], "Animation Speed", speed);
	animator->SetAnimSpeed(speed);

	for (auto&& clipJson : root["Animations"]["Clips"])
	{
		//��� ��������
		string fullPath;
		Json::GetValue(clipJson, "Path", fullPath);

		wstring path = cString::Wstring(cPath::GetDirectoryName(fullPath));

		wstring name = cString::Wstring(cPath::GetFileName(fullPath));

		auto clip = cAnimClip::Create(path, name);

		if (clip->IsCorrectBones(bones))
			animator->AddClip(clip);

		//�̺�Ʈ ������ ��������
		CreateEventFrames(clipJson, clip);
	}

	animator->SetModel(_actor->GetModel());
}

void cActorFactory::CreateBehaviorTree(Json::Value& root)
{
	string filePath;
	Json::GetValue(root, "Behavior Tree", filePath);

	auto bTreePtr = _actor->GetBehaviorTree().lock();
	if (!bTreePtr)
		assert(false && "�ൿƮ���� �߰��ϼ���!");

	if (filePath == "")
		return;

	wstring wstrPath = cString::Wstring(filePath);
	Json::Value treeValue;
	Json::ReadData(wstrPath, &treeValue);

	bTreePtr->SetName(wstrPath);
	bTreePtr->LoadJson(treeValue);
}

void cActorFactory::CreateBlackboard(Json::Value & root)
{
	auto blackboardPtr = _actor->GetBlackboard().lock();

	Json::Value blackboardJson = root["Blackboard"];
	
	blackboardPtr->SetJsonPath(_jsonPath);
	blackboardPtr->ResetData();

	////Ÿ�� ������
	//int type;
	//Json::GetValue(blackboardJson, "Inspector Type", type);
	//blackboardPtr->SetChangeInspector(static_cast<cBlackboard::eInspectorType>(type));

	////����3 ���� ������
	//Json::Value vector3Json = blackboardJson["Vector3"];
	//for (auto&& vector3 : blackboardPtr->GetVector3s())
	//{
	//	D3DXVECTOR3 value;
	//	Json::GetValue(vector3Json, vector3.first, value);
	//	blackboardPtr->SetVector3(vector3.first, value);
	//}

	////�ο� ���� ������
	//Json::Value boolJson = blackboardJson["Bool"];
	//for (auto&& bools : blackboardPtr->GetBools())
	//{
	//	bool value;
	//	Json::GetValue(boolJson, bools.first, value);
	//	blackboardPtr->SetBool(bools.first, value);
	//}
	//
	////�÷� ���� ������
	//Json::Value floatJson = blackboardJson["Float"];
	//for (auto&& floats : blackboardPtr->GetFloats())
	//{
	//	float value;
	//	Json::GetValue(floatJson, floats.first, value);
	//	blackboardPtr->SetFloat(floats.first, value);
	//}

	////��Ʈ ���� ������
	//Json::Value intJson = blackboardJson["Int"];
	//for (auto&& ints : blackboardPtr->GetInts())
	//{
	//	int value;
	//	Json::GetValue(intJson, ints.first, value);
	//	blackboardPtr->SetInt(ints.first, value);
	//}
}

void cActorFactory::CreateAction()
{
}

void cActorFactory::CreateEventFrames(Json::Value & clipJson,
									  shared_ptr<cAnimClip>& clip)
{
	for (auto&& eventFrame : clipJson["Event Frames"])
	{
		string name;
		Json::GetValue(eventFrame, "Event Name", name);

		UINT index;
		Json::GetValue(eventFrame, "Frame", index);

		clip->AddEventFrame(cString::Wstring(name), index);
	}
}
