#include "stdafx.h"
#include "cBlackboardTool.h"
#include "./Blackboard/cBlackboard.h"
#include "./GameObject/cActor.h"
#include "./Helper/cString.h"
#include "./Helper/cPath.h"

UI::cBlackboardTool::cBlackboardTool()
	: _init(false)
{
}

UI::cBlackboardTool::~cBlackboardTool()
{
}

void UI::cBlackboardTool::Update()
{
	auto actorPtr = _actor.lock();
	if (!actorPtr)
		return;

	_blackboard = actorPtr->GetBlackboard();

	auto blackboardPtr = _blackboard.lock();
	if (!blackboardPtr)
		return;

	//한번만 목록 불러오기
	if (!_init)
	{
		for (auto&& vector3s : blackboardPtr->GetVector3s())
			_vector3s[vector3s.first] = vector3s.second;

		for (auto&& bools : blackboardPtr->GetBools())
			_bools[bools.first] = bools.second;

		for (auto&& floats : blackboardPtr->GetFloats())
			_floats[floats.first] = floats.second;

		for (auto&& ints : blackboardPtr->GetInts())
			_ints[ints.first] = ints.second;

		_init = true;
	}
}

void UI::cBlackboardTool::Render()
{
}

void UI::cBlackboardTool::ShowHierarchy(int i)
{
	//DO NOTHING
}

void UI::cBlackboardTool::ShowInspector()
{
	if (ImGui::CollapsingHeader("Blackboard"))
	{
		SelectBlackboardType();
		SetInitialDataOfBlackboard();
	}
}

void UI::cBlackboardTool::SaveJson(Json::Value& root)
{
	auto blackboardPtr = _blackboard.lock();
	assert(blackboardPtr && "블랙보드를 생성하세요!");

	Json::Value blackboard;

	//플레이어/AI 타입 저장
	int type = static_cast<int>(blackboardPtr->GetInspectorType());
	Json::SetValue(blackboard, "Inspector Type", type);

	//이름과 값 저장

	Json::Value vector3;
	for (auto&& vector3s : blackboardPtr->GetVector3s())
	{
		D3DXVECTOR3 value = vector3s.second;
		Json::SetValue(vector3, vector3s.first, value);
	}
	blackboard["Vector3"] = vector3;

	Json::Value boolJson;
	for (auto&& bools : blackboardPtr->GetBools())
	{
		bool value = bools.second;
		Json::SetValue(boolJson, bools.first, value);
	}
	blackboard["Bool"] = boolJson;

	Json::Value floatJson;
	for (auto&& floats : blackboardPtr->GetFloats())
	{
		float value = floats.second;
		Json::SetValue(floatJson, floats.first, value);
	}
	blackboard["Float"] = floatJson;

	Json::Value intJson;
	for (auto&& ints : blackboardPtr->GetInts())
	{
		int value = ints.second;
		Json::SetValue(intJson, ints.first, value);
	}
	blackboard["Int"] = intJson;

	root["Blackboard"] = blackboard;
}

void UI::cBlackboardTool::LoadJson()
{
	//DO NOTHING
}

void UI::cBlackboardTool::SelectBlackboardType()
{
	auto blackboardPtr = _blackboard.lock();
	if (!blackboardPtr)
		return;

	int type = static_cast<int>(blackboardPtr->GetInspectorType());

	ImGui::Combo("Inspector", &type, "User\0AI\0");

	blackboardPtr->SetChangeInspector(static_cast<cBlackboard::eInspectorType>(type));
}

void UI::cBlackboardTool::SetInitialDataOfBlackboard()
{
	if (ImGui::Button("Data Apply"))
	{
		ApplyData();
		cDebug::Log("Apply Blackboard Data!");
	}
	int index = 0;

	for (auto&& vectorValue : _vector3s)
	{
		ImGui::PushID(index++);

		ImGui::Text((vectorValue.first + " :").c_str());
		ImGui::InputFloat3("", vectorValue.second);

		ImGui::PopID();
	}

	ImGui::Separator();

	for (auto&& boolValue : _bools)
	{
		ImGui::PushID(index++);

		ImGui::Text((boolValue.first + " :").c_str());

		ImGui::SameLine();

		if(boolValue.second)
			ImGui::Checkbox("True", &boolValue.second);
		else
			ImGui::Checkbox("False", &boolValue.second);

		ImGui::PopID();
	}

	ImGui::Separator();

	for (auto&& floatValue : _floats)
	{
		ImGui::PushID(index++);

		ImGui::Text((floatValue.first + " :").c_str());
		ImGui::InputFloat("", &floatValue.second);

		ImGui::PopID();
	}

	ImGui::Separator();

	for (auto&& intValue : _ints)
	{
		ImGui::PushID(index++);

		ImGui::Text((intValue.first + " :").c_str());
		ImGui::InputInt("", &intValue.second);

		ImGui::PopID();
	}
}

void UI::cBlackboardTool::ApplyData()
{
	auto blackboardPtr = _blackboard.lock();

	if (!blackboardPtr)
		return;

	for (auto&& vectorValue : _vector3s)
	{
		blackboardPtr->SetVector3(vectorValue.first, vectorValue.second);
	}

	for (auto&& boolValue : _bools)
	{
		blackboardPtr->SetBool(boolValue.first, boolValue.second);
	}

	for (auto&& floatValue : _floats)
	{
		blackboardPtr->SetFloat(floatValue.first, floatValue.second);
	}

	for (auto&& intValue : _ints)
	{
		blackboardPtr->SetInt(intValue.first, intValue.second);
	}
}
