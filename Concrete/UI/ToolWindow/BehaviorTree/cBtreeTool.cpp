#include "stdafx.h"
#include "cBtreeTool.h"
#include "cBlackboardTool.h"
#include "./Concrete/Message/eIdGroup.h"
#include "./GameObject/cActor.h"
#include "./BehaviorTree/cBehaviorTree.h"
#include "./Component/BehaviorTree/cTaskFactory.h"
#include "./Helper/cString.h"
#include "./Helper/cPath.h"

static inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); }
static inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y); }

UI::cBtreeTool::cBtreeTool()
{
	_parentOfNewTasks = make_shared<cRootTask>();

	_blackboardTool = make_shared<UI::cBlackboardTool>();
}

UI::cBtreeTool::~cBtreeTool()
{
}

void UI::cBtreeTool::Init()
{
	__super::Init();

	cEntityManager::Get()->RegisterEntity(eIdGroup::CHARACTER_TOOL, _blackboardTool);
}

void UI::cBtreeTool::Update()
{
	static bool init = true;
	auto actorPtr = _actor.lock();

	//��Ʈ ��� ����
	if (actorPtr)
		_bTree = actorPtr->GetBehaviorTree();

	//����� ��� ����
	if(!_bTree.expired())
		DeleteNode(_bTree.lock()->GetRoot());

	DeleteNode(_parentOfNewTasks);

	_openContextMenu = false;
	_hoveredTask.reset();

	_blackboardTool->SetActor(_actor);
	_blackboardTool->Update();
}

void UI::cBtreeTool::Render()
{
	_blackboardTool->Render();
}

void UI::cBtreeTool::ShowHierarchy(int i)
{
	if (ImGui::TreeNode(to_string(i).c_str()))
	{
		SelectBTree();

		ImGui::TreePop();
	}
	else
		SelectBTree();
}

void UI::cBtreeTool::ShowInspector()
{
	if (ImGui::CollapsingHeader("Behavior Tree"))
	{
		auto treePtr = _bTree.lock();
		if (treePtr)
		{
			ImGui::Text("Name :"); ImGui::SameLine();
			auto& name = cString::String(treePtr->GetName());
			name = cPath::GetFileName(name);
			ImGui::Text(name.c_str());

			//todo : �ε��� ������ �ٽ� �ѹ� ����� �޽��� �ڽ�
			if (ImGui::Button("Load"))
				LoadBehaviorTree();

			ImGui::SameLine();

			if (ImGui::Button("Save"))
				SaveBehaviorTree();
		}
	}

	_blackboardTool->ShowInspector();

	ImGui::Begin("Behavior Tree Tool");
	{
		ShowBehaviorTreeTool();
	}
	ImGui::End();
}

void UI::cBtreeTool::SaveJson(Json::Value& root)
{
	Json::Value behaviorTree;

	auto bTreePtr = _bTree.lock();
	if (bTreePtr)
	{
		//hack : �̹� ����� �ൿƮ���� �ٽ� �������� ���
		//if (bTreePtr->GetName() == bTreePtr->GetDefaultName())
			SaveBehaviorTree();
		
		behaviorTree.append(cString::String(bTreePtr->GetName()));
	}

	root["Behavior Tree"] = behaviorTree;

	_blackboardTool->SaveJson(root);
}

void UI::cBtreeTool::LoadJson()
{
	_blackboardTool->LoadJson();
}

void UI::cBtreeTool::SelectBTree()
{
	ImGui::SameLine();
	if (ImGui::Selectable("Behavior Tree"))
	{
		if (AlertActor())
			return;

		SendMe();
		cDebug::Log("Behavior Tree Selected!");
	}
}

void UI::cBtreeTool::ShowBehaviorTreeTool()
{
	ImGui::BeginGroup();
	DrawBackground();

	DrawLinks();
	DrawNodes();
	DrawTextMenu();

	ConnectNode();
	MoveNode();

	Scroll();

	EndBackground();
	ImGui::EndGroup();
}

void UI::cBtreeTool::DrawBackground()
{
	// Create our child canvas
	ImGui::Text("Screen Position (%.2f,%.2f)", _scrolling.x, _scrolling.y);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1, 1));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, IM_COL32(60, 60, 70, 200));
	ImGui::BeginChild("scrolling_region", ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove);
	ImGui::PushItemWidth(120.0f);

	_offset = ImGui::GetCursorScreenPos() + _scrolling - ImGui::GetWindowPos() + NODE_WINDOW_PADDING;
	
	_drawList = ImGui::GetWindowDrawList();

	//�׸��� ����
	ImU32 GRID_COLOR = IM_COL32(200, 200, 200, 40);
	float GRID_SZ = 64.0f;
	ImVec2 win_pos = ImGui::GetCursorScreenPos();
	ImVec2 canvas_sz = ImGui::GetWindowSize();
	for (float x = fmodf(_scrolling.x, GRID_SZ); x < canvas_sz.x; x += GRID_SZ)
		_drawList->AddLine(ImVec2(x, 0.0f) + win_pos, ImVec2(x, canvas_sz.y) + win_pos, GRID_COLOR);
	for (float y = fmodf(_scrolling.y, GRID_SZ); y < canvas_sz.y; y += GRID_SZ)
		_drawList->AddLine(ImVec2(0.0f, y) + win_pos, ImVec2(canvas_sz.x, y) + win_pos, GRID_COLOR);

}

void UI::cBtreeTool::EndBackground()
{
	ImGui::PopItemWidth();
	ImGui::EndChild();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar(2);
}

void UI::cBtreeTool::LoadBehaviorTree()
{
	cPath::OpenFileDialog(L"Open Behavior Tree", cPath::BehaviorTreeFilter, Model, [this](wstring path)
	{
		Json::Value root;
		Json::ReadData(path, &root);

		auto treePtr = _bTree.lock();
		if (treePtr)
		{
			treePtr->LoadJson(root);
			treePtr->SetName(path);
		}
		_parentOfNewTasks->GetChildren()->clear();
	});
}

void UI::cBtreeTool::SaveBehaviorTree()
{
	cPath::SaveFileDialog(L"Save Behavior Tree", cPath::BehaviorTreeFilter, Model, [this](wstring path)
	{
		Json::Value root;

		//����
		_bTree.lock()->SaveJson(root);

		wstring temp = cPath::GetDirectoryName(path) + cPath::GetFileNameWithoutExtension(path);
		temp += L".bt";

		_bTree.lock()->SetName(temp);

		Json::WriteData(temp, &root);
	});
}

void UI::cBtreeTool::DrawLinks()
{
	_drawList->ChannelsSplit(2);
	_drawList->ChannelsSetCurrent(0); // Background

	auto treePtr = _bTree.lock();
	if (!treePtr)
		return;

	DrawLink(treePtr->GetRoot());

	for (auto&& task : *_parentOfNewTasks->GetChildren())
		DrawLink(task);
}

void UI::cBtreeTool::DrawLink(weak_ptr<cTask> task)
{
	auto taskPtr = task.lock();

	if (taskPtr->GetChildren() == nullptr)
		return;

	if (taskPtr->GetHide())
		return;

	UINT count = taskPtr->GetChildren()->size();
	for (UINT i = 0; i < count; i++)
	{
		auto child = taskPtr->GetChildren()->at(i);

		//�θ��� ������
		ImVec2 p1 = _offset + GetNodeEnd(taskPtr, i);

		//�ڽ��� ������
		ImVec2 p2 = _offset + GetNodeStart(child);

		//�� �� ������ ������ �
		if(p1.x > p2.x)
			_drawList->AddBezierCurve(p1, p1 + ImVec2(-50, 0), p2 + ImVec2(50, 0), p2, IM_COL32(200, 200, 100, 255), 3.0f);
		else
			_drawList->AddBezierCurve(p1, p1 + ImVec2(50, 0), p2 + ImVec2(-50, 0), p2, IM_COL32(200, 200, 100, 255), 3.0f);

		DrawLink(child);
	}
}

void UI::cBtreeTool::DrawNodes()
{
	auto treePtr = _bTree.lock();
	if (!treePtr)
		return;

	//Ʈ�� ��� ��ȸ
	int id = 0;
	DrawNode(id, treePtr->GetRoot());

	for (auto&& task : *_parentOfNewTasks->GetChildren())
		DrawNode(id, task);

	_drawList->ChannelsMerge();
}

void UI::cBtreeTool::DrawNode(OUT int& id, weak_ptr<cTask> task)
{
	auto taskPtr = task.lock();
	if (!taskPtr)
		return;

	ImGui::PushID(id++);
	ImVec2 node_rect_min = _offset + taskPtr->GetPosition();

	_drawList->ChannelsSetCurrent(1); // Foreground
	bool old_any_active = ImGui::IsAnyItemActive();
	ImGui::SetCursorScreenPos(node_rect_min + NODE_WINDOW_PADDING);

	//��� ���� ���
	ImGui::BeginGroup();
	taskPtr->RenderAllInfo();
	ImGui::EndGroup();

	bool node_widgets_active = (!old_any_active && ImGui::IsAnyItemActive());

	//������ ����
	taskPtr->SetSize(ImGui::GetItemRectSize() + NODE_WINDOW_PADDING + NODE_WINDOW_PADDING);
	auto taskSize = taskPtr->GetSize();

	//�ʺ�� �ڽ� ��� ��, ������ ������ �� ū �� ����
	if(taskPtr->GetChildren())
		taskPtr->SetSize(ImVec2(max(taskSize.x, taskPtr->GetChildren()->size() * NODE_GAP), taskSize.y));
	ImVec2 node_rect_max = node_rect_min + taskPtr->GetSize();

	//��� �ڽ� ���
	_drawList->ChannelsSetCurrent(0); // Background
	ImGui::SetCursorScreenPos(node_rect_min);
	ImGui::InvisibleButton("node", taskPtr->GetSize());

	//������ ���콺 Ŭ��
	if (ImGui::IsItemHovered())
	{
		_hoveredTask = taskPtr;
		_openContextMenu |= ImGui::IsMouseClicked(1);
	}

	//��� ��ŷ
	bool node_moving_active = ImGui::IsItemActive();
	if (node_widgets_active || node_moving_active)
		_selectedTask = taskPtr;

	//��� �巡��
	if (ImGui::IsItemActive() && ImGui::IsMouseDragging(0))
		taskPtr->SetPosition(taskPtr->GetPosition() + ImGui::GetIO().MouseDelta);

	//ä�� ��

	//hack : ��Ʈ �ϵ��ڵ�
	ImU32 node_bg_color = ImGui::IsItemHovered() ? IM_COL32(75, 75, 75, 255) : IM_COL32(60, 60, 60, 255);

	if (taskPtr->GetName() == "ROOT")
		node_bg_color = IM_COL32(200, 200, 200, 255);


	//��� �簢��
	_drawList->AddRectFilled(node_rect_min, node_rect_max, node_bg_color, 4.0f);
	_drawList->AddRect(node_rect_min, node_rect_max, IM_COL32(100, 100, 100, 255), 4.0f);

	auto pos = taskPtr->GetPosition();
	auto size = taskPtr->GetSize();
	//�θ𿡼� ������ ��
	ImVec2 p2 = _offset + ImVec2(pos.x + size.x * 0.5f, pos.y);
	_drawList->AddCircleFilled(p2, NODE_SLOT_RADIUS, IM_COL32(150, 150, 150, 250));

	ImGui::PopID();

	if (taskPtr->GetHide())
		return;

	if (taskPtr->GetChildren() == nullptr)
		return;

	UINT count = taskPtr->GetChildren()->size();
	//�ڽ����� ������ ��
	for (UINT i = 0; i < count; i++)
	{
		//�θ��� ������
		ImVec2 p1 = _offset + ImVec2(pos.x + i * NODE_GAP, pos.y + size.y);

		_drawList->AddCircleFilled(p1, NODE_SLOT_RADIUS, IM_COL32(150, 150, 150, 250));
	}
	
	//��� �߰� ��
	ImVec2 p1 = _offset + ImVec2(pos.x + size.x, pos.y + size.y);
	_drawList->AddCircleFilled(p1, NODE_SLOT_RADIUS, IM_COL32(0, 255, 0, 150));

	//���
	for (auto&& child : *taskPtr->GetChildren())
		DrawNode(id, child);
}

void UI::cBtreeTool::DrawTextMenu()
{
	if (!ImGui::IsAnyItemHovered() && ImGui::IsMouseHoveringWindow() && ImGui::IsMouseClicked(1))
	{
		_selectedTask.reset();
		_hoveredTask.reset();
		_openContextMenu = true;
		_newTaskPos = GetMousePos();
	}

	if (_openContextMenu)
	{
		ImGui::OpenPopup("context_menu");

		if (!_hoveredTask.expired())
			_selectedTask = _hoveredTask;
	}

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));

	if (ImGui::BeginPopup("context_menu"))
	{
		if (_selectedTask.expired())
			DrawAddMenu();
		else
			DrawNodeMenu();

		ImGui::EndPopup();
	}

	ImGui::PopStyleVar();
}

void UI::cBtreeTool::DrawAddMenu()
{
	ImGui::Text("Create Task");
	ImGui::Separator();

	cTaskFactory taskFactory;
	shared_ptr<cTask> task;

	if (ImGui::BeginMenu("Actions", true))
	{
		if (!_actor.expired())
		{
			task = taskFactory.ShowActionMenu(_actor, _bTree, _newTaskPos);
		}
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Conditions", true))
	{
		if (!_actor.expired())
		{
			auto blackboard = _actor.lock()->GetBlackboard();
			task = taskFactory.ShowConditionMenu(blackboard, _bTree, _newTaskPos);
		}
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Decorators", true))
	{
		task = taskFactory.ShowDecoratorMenu(_bTree, _newTaskPos);

		ImGui::EndMenu();
	}

	if (ImGui::MenuItem("Composite Task", nullptr, false, true))
		task = taskFactory.CreateComposition(_bTree, _newTaskPos);
	
	//�ٿ��ֱ�
	if (ImGui::MenuItem("Paste Node", nullptr, false, true))
	{
		cTaskFactory factory;
		task = factory.CopyTask(_copiedTask);
		task->SetPosition(move(_newTaskPos));
		if (task->GetChildren())
		{
			for (auto&& child : *task->GetChildren())
				SetNodePosition(child, task->GetPosition());
		}
	}

	if (task)
		_parentOfNewTasks->GetChildren()->emplace_back(move(task));
}

void UI::cBtreeTool::SetNodePosition(weak_ptr<cTask> task, const ImVec2 & parentPos)
{
	auto taskPtr = task.lock();
	taskPtr->SetPosition(taskPtr->GetPosition() + parentPos);

	if (taskPtr->GetChildren())
	{
		for (auto&& child : *taskPtr->GetChildren())
			SetNodePosition(child, taskPtr->GetPosition());
	}
}

void UI::cBtreeTool::DrawNodeMenu()
{
	auto taskPtr = _selectedTask.lock();

	ImGui::Text("%s", taskPtr->GetName().c_str());
	ImGui::Separator();

	//������ ����� ����, ���� �޴�
	if (taskPtr->RenderMenu())
		_copiedTask = taskPtr;
}

void UI::cBtreeTool::ConnectNode()
{
	if (_clickNewNode == false)
	{
		for (UINT i = 0; i < _parentOfNewTasks->GetChildren()->size(); i++)
		{
			auto task = _parentOfNewTasks->GetChildren()->at(i);

			auto circlePos = GetNodeStart(task);

			if (Intersect(GetMousePos(), circlePos))
			{
				//ȣ������ �� �� ����
				_drawList->AddCircleFilled(_offset + circlePos, NODE_SLOT_RADIUS, IM_COL32(250, 250, 250, 250));

				//��� Ŭ����
				_clickNewNode = ImGui::IsMouseClicked(0);
				if (_clickNewNode)
				{
					_selectedNewTask = i;
					break;
				}
			}
		}
	}

	if (_clickNewNode)
	{
		ImVec2 p1 = _offset + GetNodeStart(_parentOfNewTasks->GetChildren()->at(_selectedNewTask));
		ImVec2 p2 = _offset + GetMousePos();

		//���콺�� ������ �
		_drawList->AddBezierCurve(p1, p1 + ImVec2(50, 0), p2 + ImVec2(-50, 0), p2, IM_COL32(200, 200, 100, 50), 3.0f);

		auto treePtr = _bTree.lock();
		if (treePtr)
		{
			//Ʈ���� ��� ��� �� �߰��� �Ǿ����� Ȯ��
			FindConnectNode(p2, treePtr->GetRoot());

			if (ImGui::IsMouseReleased(0))
			{
				//����
				if (!_connectedTask.expired())
				{
					_connectedTask.lock()->AddChild(_parentOfNewTasks->GetChildren()->at(_selectedNewTask));

					//��� �θ� ����
					_parentOfNewTasks->GetChildren()->at(_selectedNewTask)->SetParent(_connectedTask);

					//�� ��� ��Ͽ��� ����
					_parentOfNewTasks->GetChildren()->erase(_parentOfNewTasks->GetChildren()->begin() + _selectedNewTask);
				}
				_clickNewNode = false;
				_selectedNewTask = -1;
			}
			_connectedTask.reset();
		}
	}
}

void UI::cBtreeTool::FindConnectNode(const ImVec2& mousePos, weak_ptr<cTask> task)
{
	auto taskPtr = task.lock();

	auto pos = taskPtr->GetPosition();
	auto size = taskPtr->GetSize();

	ImVec2 addCircle = _offset + ImVec2(pos.x + size.x, pos.y + size.y);

	if (taskPtr->GetChildren())
	{
		if (Intersect(mousePos, addCircle))
		{
			_connectedTask = task;
			_drawList->AddCircleFilled(addCircle, NODE_SLOT_RADIUS, IM_COL32(255, 0, 0, 150));
		}

		for (auto task2 : *taskPtr->GetChildren())
			FindConnectNode(mousePos, task2);
	}
}

void UI::cBtreeTool::MoveNode()
{
	auto treePtr = _bTree.lock();
	if (!treePtr)
		return;

	if (_clickMoveNode == false)
	{
		//�̵��� ��� ���� ��
		SelectMoveNode(GetMousePos(), treePtr->GetRoot(), [this](weak_ptr<cTask> task, UINT i)
		{
			_clickMoveNode = ImGui::IsMouseClicked(0);

			//�̵��� ��� Ŭ��
			if (_clickMoveNode)
			{
				_nodeToMove.first = task;
				_nodeToMove.second = i;
			}
		});
	}

	if (_clickMoveNode)
	{
		_drawList->AddLine(_offset + GetNodeEnd(_nodeToMove.first.lock(), _nodeToMove.second), _offset + GetMousePos(), IM_COL32(250, 250, 250, 150), 3.0f);

		//�̵� ��� ��带 ���� ��
		SelectMoveNode(GetMousePos(), treePtr->GetRoot(), [this](weak_ptr<cTask> task, UINT i)
		{
			_targetToMove.first = task;
			_targetToMove.second = i;
		});

		FindConnectNode(_offset + GetMousePos(), treePtr->GetRoot());

		//�̵� ��� ��带 ã��
		if (ImGui::IsMouseReleased(0))
		{
			auto parent = _nodeToMove.first.lock();
			auto targetParent = _targetToMove.first.lock();
			UINT i = _targetToMove.second;
			UINT moveIndex = _nodeToMove.second;
			if (targetParent)
			{
				auto temp = parent->GetChildren()->at(moveIndex);
				if (parent == targetParent)
				{
					auto children = parent->GetChildren();
					//�� ��ȣ�� �ٲٸ� ���� ��ȣ�� ��� �ڷ� �̵�
					if (i < moveIndex)
					{
						children->erase(children->begin() + moveIndex);
						children->insert(children->begin() + i, temp);
					}
					//�޹�ȣ�� �ٲٸ� ���θ� �̵�
					else if (i > moveIndex)
					{
						(*children)[moveIndex] = (*children)[i];
						(*children)[i] = temp;
					}
				}
				else
				{
					//�ٸ� �θ��� ��带 �̵��ϴ���
					(*parent->GetChildren())[moveIndex] = (*targetParent->GetChildren())[i];
					(*targetParent->GetChildren())[i] = temp;

					//���ο� �θ� ����
					(*parent->GetChildren())[moveIndex]->SetParent(parent);

					(*targetParent->GetChildren())[i]->SetParent(targetParent);
				}
				temp.reset();
				_targetToMove.first.reset();
			}

			//_targetNewParent �̿��ؼ� �θ� �����ϱ�
			auto temp = _connectedTask.lock();
			if (temp)
			{
				//�Ű��� �½�ũ
				auto target = parent->GetChildren()->at(moveIndex);

				//�θ� ����
				target->SetParent(temp);
				temp->AddChild(target);

				//���� �θ𿡼� ����
				parent->GetChildren()->erase(parent->GetChildren()->begin() + moveIndex);

				_connectedTask.reset();

			}
			
			_clickMoveNode = false;
			_nodeToMove.first.reset();
		}
	}
}

void UI::cBtreeTool::SelectMoveNode(const ImVec2& mousePos,
									weak_ptr<cTask> task,
									function<void(weak_ptr<cTask>, UINT)> func)
{
	auto taskPtr = task.lock();
	if (!taskPtr || !taskPtr->GetChildren())
		return;

	for (UINT i = 0; i < taskPtr->GetChildren()->size(); i++)
	{
		auto circlePos = GetNodeEnd(taskPtr, i);

		if (Intersect(mousePos, circlePos))
		{
			//ȣ���� ��� �� ����
			_drawList->AddCircleFilled(_offset + circlePos, NODE_SLOT_RADIUS, IM_COL32(250, 250, 250, 250));

			func(task, i);
		}

		auto child = taskPtr->GetChildren()->at(i);
		SelectMoveNode(mousePos, child, func);
	}
}

void UI::cBtreeTool::Scroll()
{
	if (ImGui::IsWindowHovered() && ImGui::IsMouseDragging(0, 0.0f) && !(_clickNewNode || _clickMoveNode))
		_scrolling = _scrolling + ImGui::GetIO().MouseDelta;
}

void UI::cBtreeTool::DeleteNode(weak_ptr<cTask> task)
{
	auto taskPtr = task.lock();
	if (!taskPtr || !taskPtr->GetChildren())
		return;

	auto children = taskPtr->GetChildren();
	for (UINT i = 0; i < children->size(); i++)
	{
		auto child = taskPtr->GetChildren()->at(i);
		switch (child->GetState())
		{
			case eTaskState::DELETE_ALL:
			{	
				//���������� ��� ����
				children->erase(children->begin() + i);
			}
			break;
			case eTaskState::DELETE_ONLY_ME:
			{
				//�ڽ��� �ڽ��� �θ�� �ű�
				auto grandChildren = child->GetChildren();
				if (grandChildren)
				{
					for (auto iter = grandChildren->rbegin(); iter != grandChildren->rend(); iter++)
					{
						(*iter)->SetParent(taskPtr);
						children->insert(children->begin() + i, *iter);
					}

					//�ڽ� ����
					children->erase(children->begin() + i + grandChildren->size());
				}
			}
			break;
			case eTaskState::DELETE_ONLY_SUBNODES:
			{
				//�ڽ��� �ڽĸ� ����
				if(child->GetChildren())
					child->GetChildren()->clear();
			}
			break;
		}
	}

	for (auto&& child : *children)
		DeleteNode(child);
}

ImVec2 UI::cBtreeTool::GetNodeStart(shared_ptr<cTask> task)
{
	auto pos = task->GetPosition();
	auto size = task->GetSize();

	return ImVec2(pos.x + size.x * 0.5f, pos.y);
}

ImVec2 UI::cBtreeTool::GetNodeEnd(shared_ptr<cTask> task, UINT childNum)
{
	auto pos = task->GetPosition();
	auto size = task->GetSize();

	return ImVec2(pos.x + childNum * NODE_GAP, pos.y + size.y);
}

ImVec2 UI::cBtreeTool::GetMousePos()
{
	auto mousePos = ImGui::GetMousePos() - NODE_WINDOW_PADDING;
	mousePos.y += MOUSE_PADDING;
	mousePos = mousePos - _scrolling;

	return mousePos;
}

bool UI::cBtreeTool::Intersect(const ImVec2& mouse, const ImVec2 & circlePos)
{
	bool isIn = true;
	isIn &= mouse.x > circlePos.x - NODE_SLOT_RADIUS;
	isIn &= mouse.x < circlePos.x + NODE_SLOT_RADIUS;
	isIn &= mouse.y > circlePos.y - NODE_SLOT_RADIUS;
	isIn &= mouse.y < circlePos.y + NODE_SLOT_RADIUS;

	return isIn;
}