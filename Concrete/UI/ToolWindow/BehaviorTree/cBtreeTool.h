#pragma once
#include "../cTool.h"
#include "./BehaviorTree/BaseTasks.h"

class cBehaviorTree;
namespace UI
{
	struct sBtreeNode;
	struct sBtreeNodeLink;
	class cBtreeTool : public cTool
	{
	public:
		cBtreeTool();
		~cBtreeTool();
	public:
		// cTool을(를) 통해 상속됨
		virtual void Init() override;
		virtual void Update() override;
		virtual void Render() override;
		virtual void ShowHierarchy(int i) override;
		virtual void ShowInspector() override;
		virtual void SaveJson(Json::Value& root) override;
		virtual void LoadJson() override;
	private:
		void SelectBTree();

		void DrawBackground();
		void EndBackground();
		void DrawLinks();
		void DrawLink(weak_ptr<cTask> task);
		void DrawNodes();
		void DrawNode(OUT int& id, weak_ptr<cTask> task);
		void DrawTextMenu();
		void DrawAddMenu();
		void DrawNodeMenu();
		void Scroll();

		//새로 추가된 노드를 트리에 연결
		void ConnectNode();

		//연결된 노드가 있는지 확인
		void FindConnectNode(const ImVec2& mousePos, weak_ptr<cTask> task);

		//트리 노드간 이동
		void MoveNode();

		//트리 노드 중 이동할 노드 선택되었는지 확인
		void SelectMoveNode(const ImVec2& mousePos,
							weak_ptr<cTask> task,
							function<void(weak_ptr<cTask>, UINT)> func);

		//삭제할 노드가 있다면 삭제
		void DeleteNode(weak_ptr<cTask> task);
	private:
		ImVec2 GetNodeStart(shared_ptr<cTask> task); //부모 점
		ImVec2 GetNodeEnd(shared_ptr<cTask> task, UINT childNum); //자식 점
		ImVec2 GetMousePos();
		bool MouseIsInCircle(const ImVec2& mouse, const ImVec2& circlePos);
	private:
		const float NODE_SLOT_RADIUS = 5.5f;
		const ImVec2 NODE_WINDOW_PADDING = ImVec2(8.0f, 8.0f);
		const float MOUSE_PADDING = -58.0f;
		const float NODE_GAP = 25.0f; //자식 노드 선 간격

		weak_ptr<cBehaviorTree> _bTree;

		ImVec2 _scrolling = ImVec2(0.0f, 0.0f);
		
		ImDrawList* _drawList;
		ImVec2 _offset;
		bool _openContextMenu = false;
		weak_ptr<cTask> _hoveredTask;
		
		weak_ptr<cTask> _selectedTask; //현재 선택된 노드

		//트리에 추가되지 않은 노드
		vector<shared_ptr<cTask>> _newTasks;
		bool _clickNewNode = false; //클릭되었는지
		int _selectedNewTask; //새 노드 리스트 중 선택된 노드
		weak_ptr<cTask> _connectedTask; //연결할 노드

		//@first : 이동할 노드의 부모 노드 @second : 이동할 노드의 번호
		pair<weak_ptr<cTask>, UINT> _nodeToMove;
		pair<weak_ptr<cTask>, UINT> _targetToMove;
		bool _clickMoveNode = false; //이동할 노드가 클릭되었는지
	};
}