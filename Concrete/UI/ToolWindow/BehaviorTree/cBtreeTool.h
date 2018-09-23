#pragma once
#include "../cTool.h"
#include "./BehaviorTree/BaseTasks.h"

class cBehaviorTree;
namespace UI
{
	class cBlackboardTool;
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
		//하이어라키에서 항목 선택
		void SelectBTree();
		
		//삭제할 노드가 있다면 삭제
		void DeleteNode(weak_ptr<cTask> task);
	private:
		/**************
			버튼 관련
		***************/
		void LoadBehaviorTree();
		void SaveBehaviorTree();
	private:
		/***********************
			행동트리 제작 화면
		************************/
		void ShowBehaviorTreeTool();

		void DrawBackground();

		//베지어 곡선 출력
		void DrawLinks();
		void DrawLink(const shared_ptr<cTask> & task);

		//노드 출력
		void DrawNodes();
		void DrawNode(OUT int& id, const shared_ptr<cTask> & task);

		//메뉴 팝업 출력
		void DrawTextMenu();
		//노드 생성 팝업
		void DrawAddMenu();
		//노드 위치 조정(부모 노드와 상대 위치만큼)
		void SetNodePosition(shared_ptr<cTask> & task, const ImVec2& parentPos);
		//선택한 노드의 삭제, 복사 메뉴
		void DrawNodeMenu();

		//새로 추가된 노드를 트리에 연결
		void ConnectNode();
		//연결된 노드가 있는지 확인
		//@param : 마우스 좌표
		//@param : 재귀 노드
		void FindConnectNode(const ImVec2& mousePos, const shared_ptr<cTask> & task);

		//트리 노드간 이동
		void MoveNode();
		//트리 노드 중 이동할 노드 선택되었는지 확인
		//@param : 마우스 좌표
		//@param : 재귀 노드
		//@param : 이동할 노드 선택 후 콜백 함수
		void SelectMoveNode(const ImVec2& mousePos,
							const shared_ptr<cTask> & task,
							function<void(weak_ptr<cTask>, UINT)> func);

		void Scroll();

		void EndBackground();
	private:
		//부모 점
		ImVec2 GetNodeStart(const shared_ptr<cTask> & task);
		//자식 점
		ImVec2 GetNodeEnd(const shared_ptr<cTask> & task, UINT childNum);
		//마우스 위치
		ImVec2 GetMousePos();

		//두 점 교차 판정
		bool Intersect(const ImVec2& mouse, const ImVec2& circlePos);
	private:
		const float NODE_SLOT_RADIUS = 5.5f; //원 반지름
		const ImVec2 NODE_WINDOW_PADDING = ImVec2(8.0f, 8.0f);
		const float MOUSE_PADDING = 0;// -58.0f;
		const float NODE_GAP = 25.0f; //자식 노드 선 간격
		ImVec2 _scrolling = ImVec2(0.0f, 0.0f); //스크린 위치
		
		weak_ptr<cBehaviorTree> _bTree;

		ImDrawList* _drawList; //캔버스
		ImVec2 _offset; //마우스와 스크린 위치 더한 값
		bool _openContextMenu = false;
		weak_ptr<cTask> _hoveredTask;
		
		weak_ptr<cTask> _selectedTask; //현재 선택된 노드

		//트리에 추가되지 않은 노드
		shared_ptr<cRootTask> _parentOfNewTasks;
		bool _clickNewNode = false; //클릭되었는지
		int _selectedNewTask; //새 노드 리스트 중 선택된 노드
		weak_ptr<cTask> _connectedTask; //연결할 노드
		ImVec2 _newTaskPos; //새로 생성될 노드의 위치

		//@first : 이동할 노드의 부모 노드 @second : 이동할 노드의 번호
		pair<weak_ptr<cTask>, UINT> _nodeToMove;
		pair<weak_ptr<cTask>, UINT> _targetToMove;
		bool _clickMoveNode = false; //이동할 노드가 클릭되었는지

		shared_ptr<cTask> _copiedTask;

		shared_ptr<cBlackboardTool> _blackboardTool;
	};
}