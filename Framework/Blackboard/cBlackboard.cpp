#include "stdafx.h"
#include "cBlackboard.h"
#include "cUserInspector.h"
#include "cAiInspector.h"

cBlackboard::cBlackboard(weak_ptr<cActor> actor)
	: _type(eInspectorType::USER)
	, _me(actor)
{
	_vector3s["Direction"] = { 0,0,0 }; //Ű���� ����
	_vector3s["TargetPos"] = { 0,0,0 }; //Ÿ�� ��ġ
	_vector3s["MyPos"] = { 0,0,0 };
	_vector3s["TargetDirection"] = { 0,0,0 }; //Ÿ�ٰ��� ����
	_vector3s["AttackerPosition"] = { 0,0,0 }; //���� ������ ����� ��ġ

	_bools["Dash"] = false; //�뽬 ����
	_bools["MeleeAttack"] = false; //���� ����
	_bools["Animate"] = false; //�ִϸ��̼� ����
	_bools["Damage"] = false; //������ ���� �޾Ҵ��� ����

	//_bools[]
	//�޺� ����
	//0 - ���� �ƴ�
	//1 - ���� ��
	//2 - �޺� ����
	_ints["AttackState"] = 0;

	_ints["AttackNumber"] = 0; //���� ���� ��ȣ
	_ints["MaxAttackNumber"] = 1; //�ִ� ���� ��ȣ

	_floats["HP"] = 0.0f; //ü��
	_floats["SearchRange"] = 0.0f; //���� Ž���ϴ� �ִ� �Ÿ�
	_floats["DistanceGap"] = 0.0f; //���� ���� �Ÿ� ����
}

cBlackboard::~cBlackboard()
{
}

void cBlackboard::Init()
{
	SetChangeInspector(_type);
}

void cBlackboard::ResetData()
{
	Json::Value root;
	Json::ReadData(_jsonPath, &root);

	Json::Value blackboardJson = root["Blackboard"];

	//Ÿ�� ������
	int type;
	Json::GetValue(blackboardJson, "Inspector Type", type);
	SetChangeInspector(static_cast<cBlackboard::eInspectorType>(type));

	//����3 ���� ������
	Json::Value vector3Json = blackboardJson["Vector3"];
	for (auto&& vector3 : GetVector3s())
	{
		D3DXVECTOR3 value;
		Json::GetValue(vector3Json, vector3.first, value);
		SetVector3(vector3.first, value);
	}

	//�ο� ���� ������
	Json::Value boolJson = blackboardJson["Bool"];
	for (auto&& bools : GetBools())
	{
		bool value;
		Json::GetValue(boolJson, bools.first, value);
		SetBool(bools.first, value);
	}

	//�÷� ���� ������
	Json::Value floatJson = blackboardJson["Float"];
	for (auto&& floats : GetFloats())
	{
		float value;
		Json::GetValue(floatJson, floats.first, value);
		SetFloat(floats.first, value);
	}

	//��Ʈ ���� ������
	Json::Value intJson = blackboardJson["Int"];
	for (auto&& ints : GetInts())
	{
		int value;
		Json::GetValue(intJson, ints.first, value);
		SetInt(ints.first, value);
	}
}

bool cBlackboard::IsChange()
{
	//test : �׽�Ʈ
	if (_type == cBlackboard::AI)
	{
		for (auto&& vectorValue : _vector3s)
		{
			cDebug::RenewalLog((vectorValue.first + " :").c_str());
			cDebug::RenewalLog("%f, %f, %f", vectorValue.second.x, vectorValue.second.y, vectorValue.second.z);
		}

		for (auto&& boolValue : _bools)
		{
			cDebug::RenewalLog((boolValue.first + " :").c_str());

			cDebug::RenewalLog("%d", boolValue.second);
		}

		for (auto&& floatValue : _floats)
		{
			cDebug::RenewalLog((floatValue.first + " :").c_str());
			cDebug::RenewalLog("%f", floatValue.second);
		}

		for (auto&& intValue : _ints)
		{
			cDebug::RenewalLog((intValue.first + " :").c_str());
			cDebug::RenewalLog("%d", intValue.second);
		}
	}

	if (!_changeInspector)
		return false;

	return _changeInspector->IsChange();
}

void cBlackboard::SetChangeInspector(eInspectorType type)
{
	_changeInspector.reset();

	switch (type)
	{
		case cBlackboard::USER:
			_changeInspector = make_unique<cUserInspector>(shared_from_this());
			break;
		case cBlackboard::AI:
			_changeInspector = make_unique<cAiInspector>(shared_from_this());
			break;
	}

	_type = type;
}