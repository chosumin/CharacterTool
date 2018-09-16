#include "stdafx.h"
#include "cBlackboard.h"
#include "cUserInspector.h"
#include "cAiInspector.h"

cBlackboard::cBlackboard(weak_ptr<cActor> actor)
	: _type(eInspectorType::USER)
	, _me(actor)
{
	_vector3s["Direction"] = { 0,0,0 }; //키보드 방향
	_vector3s["TargetPos"] = { 0,0,0 }; //타겟 위치
	_vector3s["MyPos"] = { 0,0,0 };
	_vector3s["TargetDirection"] = { 0,0,0 }; //타겟과의 방향
	_vector3s["AttackerPosition"] = { 0,0,0 }; //나를 공격한 상대의 위치

	_bools["Dash"] = false; //대쉬 여부
	_bools["MeleeAttack"] = false; //공격 여부
	_bools["Animate"] = false; //애니메이션 여부
	_bools["Damage"] = false; //적에게 공격 받았는지 여부

	//_bools[]
	//콤보 상태
	//0 - 공격 아님
	//1 - 공격 중
	//2 - 콤보 가능
	_ints["AttackState"] = 0;

	_ints["AttackNumber"] = 0; //현재 공격 번호
	_ints["MaxAttackNumber"] = 1; //최대 공격 번호

	_floats["HP"] = 0.0f; //체력
	_floats["SearchRange"] = 0.0f; //적을 탐지하는 최대 거리
	_floats["DistanceGap"] = 0.0f; //상대와 나의 거리 차이
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

	//타입 가져옴
	int type;
	Json::GetValue(blackboardJson, "Inspector Type", type);
	SetChangeInspector(static_cast<cBlackboard::eInspectorType>(type));

	//벡터3 변수 가져옴
	Json::Value vector3Json = blackboardJson["Vector3"];
	for (auto&& vector3 : GetVector3s())
	{
		D3DXVECTOR3 value;
		Json::GetValue(vector3Json, vector3.first, value);
		SetVector3(vector3.first, value);
	}

	//부울 변수 가져옴
	Json::Value boolJson = blackboardJson["Bool"];
	for (auto&& bools : GetBools())
	{
		bool value;
		Json::GetValue(boolJson, bools.first, value);
		SetBool(bools.first, value);
	}

	//플롯 변수 가져옴
	Json::Value floatJson = blackboardJson["Float"];
	for (auto&& floats : GetFloats())
	{
		float value;
		Json::GetValue(floatJson, floats.first, value);
		SetFloat(floats.first, value);
	}

	//인트 변수 가져옴
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
	//test : 테스트
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