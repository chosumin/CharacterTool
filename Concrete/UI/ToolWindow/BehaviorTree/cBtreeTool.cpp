#include "stdafx.h"
#include "cBtreeTool.h"
#include "./GameObject/cActor.h"
#include "./BehaviorTree/cBehaviorTree.h"

static inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); }
static inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y); }

UI::cBtreeTool::cBtreeTool()
{
}

UI::cBtreeTool::~cBtreeTool()
{
}

void UI::cBtreeTool::Init()
{
	cTool::Init();
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

	_openContextMenu = false;
	_hoveredTask.reset();
}

void UI::cBtreeTool::Render()
{
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
	ImGui::Begin("Behavior Tree Tool");
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
	ImGui::End();
}

void UI::cBtreeTool::SaveJson(Json::Value& root)
{

}

void UI::cBtreeTool::LoadJson()
{

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

void UI::cBtreeTool::DrawBackground()
{
	// Create our child canvas
	ImGui::Text("Hold middle mouse button to scroll (%.2f,%.2f)", _scrolling.x, _scrolling.y);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1, 1));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, IM_COL32(60, 60, 70, 200));
	ImGui::BeginChild("scrolling_region", ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove);
	ImGui::PushItemWidth(120.0f);

	//그리드 생성
	_offset = ImGui::GetCursorScreenPos() + _scrolling;
	_drawList = ImGui::GetWindowDrawList();
	// Display grid

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

void UI::cBtreeTool::DrawLinks()
{
	_drawList->ChannelsSplit(2);
	_drawList->ChannelsSetCurrent(0); // Background

	auto treePtr = _bTree.lock();
	if (!treePtr)
		return;

	DrawLink(treePtr->GetRoot());
}

void UI::cBtreeTool::DrawLink(weak_ptr<cTask> task)
{
	auto taskPtr = task.lock();

	if (taskPtr->GetChildren() == nullptr)
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

	for (auto&& task : _newTasks)
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
	ImGui::BeginGroup(); // Lock horizontal position		
	taskPtr->RenderInfo();
	ImGui::EndGroup();

	bool node_widgets_active = (!old_any_active && ImGui::IsAnyItemActive());

	//사이즈 지정
	taskPtr->SetSize(ImGui::GetItemRectSize() + NODE_WINDOW_PADDING + NODE_WINDOW_PADDING);
	auto taskSize = taskPtr->GetSize();
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
	ImU32 node_bg_color = ImGui::IsItemHovered() ? IM_COL32(75, 75, 75, 255) : IM_COL32(60, 60, 60, 255);

	//노드 사각형
	_drawList->AddRectFilled(node_rect_min, node_rect_max, node_bg_color, 4.0f);
	_drawList->AddRect(node_rect_min, node_rect_max, IM_COL32(100, 100, 100, 255), 4.0f);

	auto pos = taskPtr->GetPosition();
	auto size = taskPtr->GetSize();
	//부모에서 들어오는 점
	ImVec2 p2 = _offset + ImVec2(pos.x + size.x * 0.5f, pos.y);
	_drawList->AddCircleFilled(p2, NODE_SLOT_RADIUS, IM_COL32(150, 150, 150, 250));

	ImGui::PopID();

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
		ImVec2 scene_pos = ImGui::GetMousePosOnOpeningCurrentPopup() - _offset;
		
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
	ImGui::Text("Create Node");
	ImGui::Separator();

	//새 노드
	if (ImGui::MenuItem("New Node", nullptr, false, true))
	{
		_newTasks.emplace_back(make_shared<cSelector>(GetMousePos()));
		
	}

	//붙여넣기
	if (ImGui::MenuItem("Paste Node", nullptr, false, true))
	{
	}
}

void UI::cBtreeTool::DrawNodeMenu()
{
	auto taskPtr = _selectedTask.lock();

	ImGui::Text("%s", taskPtr->GetName().c_str());
	ImGui::Separator();

	taskPtr->RenderMenu();
}

void UI::cBtreeTool::Scroll()
{
	if (ImGui::IsWindowHovered() && ImGui::IsMouseDragging(0, 0.0f) && !(_clickNewNode || _clickMoveNode))
		_scrolling = _scrolling + ImGui::GetIO().MouseDelta;
}

void UI::cBtreeTool::ConnectNode()
{
	if (_clickNewNode == false)
	{
		for (UINT i = 0; i < _newTasks.size(); i++)
		{
			auto task = _newTasks[i];
			
			auto circlePos = GetNodeStart(task);

			if (MouseIsInCircle(GetMousePos(), circlePos))
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
		ImVec2 p1 = _offset + GetNodeStart(_newTasks[_selectedNewTask]);
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
					_connectedTask.lock()->AddChild(_newTasks[_selectedNewTask]);

					//새 노드 목록에서 삭제
					_newTasks.erase(_newTasks.begin() + _selectedNewTask);
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
		if (MouseIsInCircle(mousePos, addCircle))
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
		_drawList->AddLine(_offset + GetNodeEnd(_nodeToMove.first.lock(), _nodeToMove.second), _offset + GetMousePos(), IM_COL32(250,250,250,150), 3.0f);

		//이동 대상 노드를 선택 중
		SelectMoveNode(GetMousePos(), treePtr->GetRoot(), [this](weak_ptr<cTask> task, UINT i)
		{
			_targetToMove.first = task;
			_targetToMove.second = i;
		});

		//이동 대상 노드를 찾음
		if (ImGui::IsMouseReleased(0))
		{
			auto parent = _nodeToMove.first.lock();
			auto targetParent = _targetToMove.first.lock();
			if (targetParent)
			{
				UINT i = _targetToMove.second;
				UINT moveIndex = _nodeToMove.second;
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
				}
				temp.reset();
				_targetToMove.first.reset();
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
	if (!taskPtr && !taskPtr->GetChildren())
		return;

	for (UINT i = 0; i < taskPtr->GetChildren()->size(); i++)
	{
		auto circlePos = GetNodeEnd(taskPtr, i);

		if (MouseIsInCircle(mousePos, circlePos))
		{
			//호버링 노드 색 변경
			_drawList->AddCircleFilled(_offset + circlePos, NODE_SLOT_RADIUS, IM_COL32(250, 250, 250, 250));

			func(task, i);
		}

		auto child = taskPtr->GetChildren()->at(i);
		SelectMoveNode(mousePos, child, func);
	}
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
				for (auto iter = grandChildren->rbegin(); iter != grandChildren->rend(); iter++)
				{
					children->insert(children->begin() + i, *iter);
				}

				//자식 삭제
				children->erase(children->begin() + i + grandChildren->size());
			}
			break;
			case eTaskState::DELETE_ONLY_SUBNODES:
			{
				//자식의 자식만 삭제
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

bool UI::cBtreeTool::MouseIsInCircle(const ImVec2& mouse, const ImVec2 & circlePos)
{
	bool isIn = true;
	isIn &= mouse.x > circlePos.x - NODE_SLOT_RADIUS;
	isIn &= mouse.x < circlePos.x + NODE_SLOT_RADIUS;
	isIn &= mouse.y > circlePos.y - NODE_SLOT_RADIUS;
	isIn &= mouse.y < circlePos.y + NODE_SLOT_RADIUS;

	return isIn;
}
