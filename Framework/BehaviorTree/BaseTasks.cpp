#include "stdafx.h"
#include "BaseTasks.h"
#include "cBehaviorTree.h"

cTask::cTask(weak_ptr<cBehaviorTree> tree, string name,
			 const ImVec2& position, const weak_ptr<cTask>& parent)
	: _taskName(name)
	, _pos(position)
	, _delete(eTaskState::NONE)
	, _tree(tree)
	, _state(eState::NONE)
	, _parent(parent)
	, _hide(false)
{
}

void cTask::SetInitState(const bool& includeRunning)
{
	//러닝 포함 모두 초기화이거나 러닝상태 아닐 경우 초기화
	if (includeRunning || _state != eState::RUNNING)
			_state = eState::NONE;
	
	if (GetChildren())
	{
		for (auto&& child : *GetChildren())
			child->SetInitState(includeRunning);
	}
}

void cTask::RenderAllInfo()
{
	RenderInfo();

	ImGui::NewLine();
	
	switch (_state)
	{
		case eState::NONE:
			ImGui::Text("NONE");
			break;
		case eState::FAILURE:
			ImGui::Text("FAILURE");
			break;
		case eState::SUCCESS:
			ImGui::Text("SUCCESS");
			break;
		case eState::RUNNING:
			ImGui::Text("RUNNING");
			break;
	}
}

bool cTask::RenderMenu()
{
	unique_ptr<cTask> task = nullptr;
	//노드 삭제 메뉴
	if (ImGui::MenuItem("Delete All", nullptr, false, true))
		_delete = eTaskState::DELETE_ALL;

	if (ImGui::MenuItem("Delete Only Me", nullptr, false, true))
		_delete = eTaskState::DELETE_ONLY_ME;
	
	if (ImGui::MenuItem("Delete Only SubNodes", nullptr, false, true))
		_delete = eTaskState::DELETE_ONLY_SUBNODES;

	//노드 복사 메뉴
	if (ImGui::MenuItem("Copy With SubNodes", nullptr, false, true))
		return true;

	return false;
}

cTask::TaskList * cTask::GetChildren()
{
	return nullptr;
}

weak_ptr<cTask> cTask::GetParent() const
{
	return _parent;
}

void cTask::LoadDefaultInfo(Json::Value & task)
{
	Json::GetValue(task, "Task Name", _taskName);

	D3DXVECTOR2 vec2;

	Json::GetValue(task, "Position", vec2);
	_pos.x = vec2.x, _pos.y = vec2.y;

	Json::GetValue(task, "Size", vec2);
	_size.x = vec2.x, _size.y = vec2.y;
}

void cTask::SaveDefaultInfo(Json::Value & task)
{
	Json::SetValue(task, "Task Name", _taskName);
	Json::SetValue(task, "Position", D3DXVECTOR2{ _pos.x, _pos.y });
	Json::SetValue(task, "Size", D3DXVECTOR2{ _size.x, _size.y });
}

/**********************************************************/

const char* cCompositeTask::ITEMS[3] = { "Selector", "Sequence", "Parallel" };

cCompositeTask::cCompositeTask(weak_ptr<cBehaviorTree> tree,
							   const ImVec2& position,
							   weak_ptr<cTask> parent)
	: cTask(tree, "Composite Task", position, parent)
	, _type(0)
	, _summury("")
{
}

cCompositeTask::~cCompositeTask()
{
}

cTask::eState cCompositeTask::Run()
{
	UINT i = 0;
	switch (_type)
	{
		case 0: //Selector
		{
			if (_state == eState::RUNNING)
			{
				//러닝 상태는 중간에 하나가 러닝 중임
				//앞의 것은 결과가 나왔으므로 생략한다.
				for (; i < _children.size(); ++i)
				{
					if (_children[i]->GetRunningState() == eState::RUNNING)
						break;
				}
			}

			for (; i < _children.size(); ++i)
			{
				_state = _children[i]->Run();
				if (_state == eState::SUCCESS || _state == eState::RUNNING)
					return _state;
			}
			
			return _state = eState::FAILURE;
		}
		case 1: //Sequence
		{
			if (_state == eState::RUNNING)
			{
				//러닝 상태는 중간에 하나가 러닝 중임
				//앞의 것은 결과가 나왔으므로 생략한다.
				for (; i < _children.size(); ++i)
				{
					if (_children[i]->GetRunningState() == eState::RUNNING)
						break;;
				}
			}

			for (; i < _children.size(); ++i)
			{
				_state = _children[i]->Run();
				if (_state == eState::FAILURE || _state == eState::RUNNING)
					return _state;
			}

			return _state = eState::SUCCESS;
		}
		case 2: //Parallel
		{
			UINT successCount = 0;
			if (_state == eState::RUNNING)
			{
				//러닝 중인것을 찾고 중간에 성공 태스크는 카운트에 추가
				for (; i < _children.size(); ++i)
				{
					auto state = _children[i]->GetRunningState();
					if (state == eState::RUNNING)
						break;
					else if (state == eState::SUCCESS)
						successCount++;
				}
			}

			for (; i < _children.size(); ++i)
			{
				_state = _children[i]->Run();

				if (_state == eState::FAILURE)
					return _state;

				if (_state == eState::SUCCESS)
					successCount++;
			}

			_state = successCount >= _children.size() ? eState::SUCCESS : eState::RUNNING;

			if (_state == eState::RUNNING)
				_tree.lock()->SetCurrentTask(shared_from_this());

			return _state;
		}
	}

	return eState::NONE;
}

cCompositeTask::TaskList * cCompositeTask::GetChildren()
{
	return &_children;
}

void cCompositeTask::RenderInfo()
{
	if (_hide)
		ImGui::TextColored(ImVec4(0, 255, 255, 255), "%s", _summury);
	else
	{
		if (GetName() == "ROOT")
			ImGui::TextColored(ImVec4(0, 0, 0, 255), "%s", GetName().c_str());
		else
			ImGui::TextColored(ImVec4(0, 255, 255, 255), "%s", GetName().c_str());
	}

	ImGui::NewLine();

	ImGui::PushID(0);
	ImGui::Combo("", &_type, ITEMS, 3);
	ImGui::PopID();

	ImGui::Checkbox("Hide sub-nodes", &_hide);
	ImGui::PushID(1);
	{
		if (_hide)
			ImGui::InputText("", _summury, IM_ARRAYSIZE(_summury));
	}
	ImGui::PopID();
}

void cCompositeTask::AddChild(std::shared_ptr<cTask> child)
{
	_children.emplace_back(move(child));
}

std::unique_ptr<cTask> cCompositeTask::Clone() const
{
	auto clone = make_unique<cCompositeTask>(_tree, _pos, _parent);
	
	clone->_type = _type;
	clone->_hide = _hide;
	strcpy(clone->_summury, _summury);

	//자식이 있다면 자식 복사 뒤 자식의 부모를 자신으로 세팅
	for (auto&& child : _children)
	{
		auto childClone = child->Clone();
		clone->AddChild(move(childClone));
	}

	return move(clone);
}

void cCompositeTask::LoadJson(Json::Value & root)
{
	LoadDefaultInfo(root);
	
	Json::GetValue(root, "Type", _type);
	Json::GetValue(root, "Hide", _hide);

	string summury;
	Json::GetValue(root, "Summury", summury);
	strcpy(_summury, summury.c_str());
}

void cCompositeTask::SaveJson(Json::Value & root)
{
	Json::Value task;

	SaveDefaultInfo(task);

	Json::SetValue(task, "Type", _type);
	Json::SetValue(task, "Hide", _hide);

	string summury = _summury;
	Json::SetValue(task, "Summury", summury);

	Json::Value childValue;

	UINT size = _children.size();
	for (auto&& child : _children)
		child->SaveJson(childValue);

	task["Children"] = childValue;

	root.append(task);
}

/**********************************************************/

cRootTask::cRootTask()
	:cCompositeTask(weak_ptr<cBehaviorTree>())
{
	_taskName = "ROOT";
	_type = 0;
}

cRootTask::~cRootTask()
{
}
