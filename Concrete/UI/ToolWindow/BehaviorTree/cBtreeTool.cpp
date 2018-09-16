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

	//루트 노드 삽입
	if (actorPtr)
		_bTree = actorPtr->GetBehaviorTree();

	//만료된 노드 삭제
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

			//todo : 로드할 것인지 다시 한번 물어보는 메시지 박스
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
		//hack : 이미 저장된 행동트리를 다시 저장할지 고민
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

	//그리드 생성
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

		//저장
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

		//부모의 시작점
		ImVec2 p1 = _offset + GetNodeEnd(taskPtr, i);

		//자식의 도착점
		ImVec2 p2 = _offset + GetNodeStart(child);

		//두 점 연결한 베지어 곡선
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

	//트리 노드 순회
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

	//노드 정보 출력
	ImGui::BeginGroup();
	taskPtr->RenderAllInfo();
	ImGui::EndGroup();

	bool node_widgets_active = (!old_any_active && ImGui::IsAnyItemActive());

	//사이즈 지정
	taskPtr->SetSize(ImGui::GetItemRectSize() + NODE_WINDOW_PADDING + NODE_WINDOW_PADDING);
	auto taskSize = taskPtr->GetSize();

	//너비는 자식 노드 수, 컨텐츠 사이즈 중 큰 것 선택
	if(taskPtr->GetChildren())
		taskPtr->SetSize(ImVec2(max(taskSize.x, taskPtr->GetChildren()->size() * NODE_GAP), taskSize.y));
	ImVec2 node_rect_max = node_rect_min + taskPtr->GetSize();

	//노드 박스 출력
	_drawList->ChannelsSetCurrent(0); // Background
	ImGui::SetCursorScreenPos(node_rect_min);
	ImGui::InvisibleButton("node", taskPtr->GetSize());

	//오른쪽 마우스 클릭
	if (ImGui::IsItemHovered())
	{
		_hoveredTask = taskPtr;
		_openContextMenu |= ImGui::IsMouseClicked(1);
	}

	//노드 피킹
	bool node_moving_active = ImGui::IsItemActive();
	if (node_widgets_active || node_moving_active)
		_selectedTask = taskPtr;

	//노드 드래그
	if (ImGui::IsItemActive() && ImGui::IsMouseDragging(0))
		taskPtr->SetPosition(taskPtr->GetPosition() + ImGui::GetIO().MouseDelta);

	//채움 색

	//hack : 루트 하드코딩
	ImU32 node_bg_color = ImGui::IsItemHovered() ? IM_COL32(75, 75, 75, 255) : IM_COL32(60, 60, 60, 255);

	if (taskPtr->GetName() == "ROOT")
		node_bg_color = IM_COL32(200, 200, 200, 255);


	//노드 사각형
	_drawList->AddRectFilled(node_rect_min, node_rect_max, node_bg_color, 4.0f);
	_drawList->AddRect(node_rect_min, node_rect_max, IM_COL32(100, 100, 100, 255), 4.0f);

	auto pos = taskPtr->GetPosition();
	auto size = taskPtr->GetSize();
	//부모에서 들어오는 점
	ImVec2 p2 = _offset + ImVec2(pos.x + size.x * 0.5f, pos.y);
	_drawList->AddCircleFilled(p2, NODE_SLOT_RADIUS, IM_COL32(150, 150, 150, 250));

	ImGui::PopID();

	if (taskPtr->GetHide())
		return;

	if (taskPtr->GetChildren() == nullptr)
		return;

	UINT count = taskPtr->GetChildren()->size();
	//자식으로 나가는 점
	for (UINT i = 0; i < count; i++)
	{
		//부모의 시작점
		ImVec2 p1 = _offset + ImVec2(pos.x + i * NODE_GAP, pos.y + size.y);

		_drawList->AddCircleFilled(p1, NODE_SLOT_RADIUS, IM_COL32(150, 150, 150, 250));
	}
	
	//노드 추가 원
	ImVec2 p1 = _offset + ImVec2(pos.x + size.x, pos.y + size.y);
	_drawList->AddCircleFilled(p1, NODE_SLOT_RADIUS, IM_COL32(0, 255, 0, 150));

	//재귀
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
	
	//붙여넣기
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

	//선택한 노드의 삭제, 복사 메뉴
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
				//호버링된 원 색 변경
				_drawList->AddCircleFilled(_offset + circlePos, NODE_SLOT_RADIUS, IM_COL32(250, 250, 250, 250));

				//노드 클릭됨
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

		//마우스와 베지어 곡선
		_drawList->AddBezierCurve(p1, p1 + ImVec2(50, 0), p2 + ImVec2(-50, 0), p2, IM_COL32(200, 200, 100, 50), 3.0f);

		auto treePtr = _bTree.lock();
		if (treePtr)
		{
			//트리의 모든 노드 중 추가가 되었는지 확인
			FindConnectNode(p2, treePtr->GetRoot());

			if (ImGui::IsMouseReleased(0))
			{
				//연결
				if (!_connectedTask.expired())
				{
					_connectedTask.lock()->AddChild(_parentOfNewTasks->GetChildren()->at(_selectedNewTask));

					//노드 부모 세팅
					_parentOfNewTasks->GetChildren()->at(_selectedNewTask)->SetParent(_connectedTask);

					//새 노드 목록에서 삭제
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
		//이동할 노드 선택 중
		SelectMoveNode(GetMousePos(), treePtr->GetRoot(), [this](weak_ptr<cTask> task, UINT i)
		{
			_clickMoveNode = ImGui::IsMouseClicked(0);

			//이동할 노드 클릭
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

		//이동 대상 노드를 선택 중
		SelectMoveNode(GetMousePos(), treePtr->GetRoot(), [this](weak_ptr<cTask> task, UINT i)
		{
			_targetToMove.first = task;
			_targetToMove.second = i;
		});

		FindConnectNode(_offset + GetMousePos(), treePtr->GetRoot());

		//이동 대상 노드를 찾음
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
					//앞 번호와 바꾸면 뒤의 번호도 모두 뒤로 이동
					if (i < moveIndex)
					{
						children->erase(children->begin() + moveIndex);
						children->insert(children->begin() + i, temp);
					}
					//뒷번호와 바꾸면 본인만 이동
					else if (i > moveIndex)
					{
						(*children)[moveIndex] = (*children)[i];
						(*children)[i] = temp;
					}
				}
				else
				{
					//다른 부모의 노드를 이동하는지
					(*parent->GetChildren())[moveIndex] = (*targetParent->GetChildren())[i];
					(*targetParent->GetChildren())[i] = temp;

					//새로운 부모 세팅
					(*parent->GetChildren())[moveIndex]->SetParent(parent);

					(*targetParent->GetChildren())[i]->SetParent(targetParent);
				}
				temp.reset();
				_targetToMove.first.reset();
			}

			//_targetNewParent 이용해서 부모 변경하기
			auto temp = _connectedTask.lock();
			if (temp)
			{
				//옮겨질 태스크
				auto target = parent->GetChildren()->at(moveIndex);

				//부모 변경
				target->SetParent(temp);
				temp->AddChild(target);

				//원래 부모에서 삭제
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
			//호버링 노드 색 변경
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
				//하위노드까지 모두 삭제
				children->erase(children->begin() + i);
			}
			break;
			case eTaskState::DELETE_ONLY_ME:
			{
				//자식의 자식을 부모로 옮김
				auto grandChildren = child->GetChildren();
				if (grandChildren)
				{
					for (auto iter = grandChildren->rbegin(); iter != grandChildren->rend(); iter++)
					{
						(*iter)->SetParent(taskPtr);
						children->insert(children->begin() + i, *iter);
					}

					//자식 삭제
					children->erase(children->begin() + i + grandChildren->size());
				}
			}
			break;
			case eTaskState::DELETE_ONLY_SUBNODES:
			{
				//자식의 자식만 삭제
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