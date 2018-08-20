#include "stdafx.h"
#include "cKeyCondition.h"
#include "cTaskFactory.h"
#include "./GameObject/cActor.h"
#include "./Interface/iAction.h"

cKeyCondition::cKeyCondition(weak_ptr<cBehaviorTree> tree, const ImVec2& position)
	: cTask(tree, "Key Condition", position)
	, _selectedButton("")
	, _type(cKeyCondition::eKeyType::DOWN)
{
}

cKeyCondition::~cKeyCondition()
{
}

std::unique_ptr<cTask> cKeyCondition::Clone() const
{
	auto clone = make_unique<cKeyCondition>(_tree, _pos);
	clone->_type = _type;
	clone->_selectedButton = _selectedButton;
	clone->_button = _button;
	return move(clone);
}

bool cKeyCondition::Run()
{
	switch (_type)
	{
		case cKeyCondition::eKeyType::DOWN:
			return cKeyboard::Get()->Down(_button);
		case cKeyCondition::eKeyType::PRESS:
			return cKeyboard::Get()->Press(_button);
		case cKeyCondition::eKeyType::UP:
			return cKeyboard::Get()->Up(_button);
	}

	return false;
}

void cKeyCondition::Update()
{
	//DO NOTHING
}

void cKeyCondition::RenderInfo()
{
	ImGui::TextColored(ImVec4(0, 255, 0, 1), _taskName.c_str());

	ImGui::NewLine();

	ImGui::Text("Type");

	ImGui::PushID(0);
	{
		const char* items[] = { "Down", "Press", "Up" };
		int type = static_cast<int>(_type);
		ImGui::Combo("", &type, items, IM_ARRAYSIZE(items));
		_type = static_cast<cKeyCondition::eKeyType>(type);
	}
	ImGui::PopID();

	ImGui::Text("Button");

	ImGui::PushID(1);
	if (ImGui::BeginCombo("", _selectedButton.c_str()))
	{
		int i = 0;
		for (auto&& key : cTaskFactory::GetKeyList())
		{
			string keyName = key.second.second;
			bool isSelected = _selectedButton == keyName;
			if (ImGui::Selectable(keyName.c_str(), isSelected))
			{
				_selectedButton = keyName;
				_button = key.second.first;
			}
			ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}
	ImGui::PopID();
}