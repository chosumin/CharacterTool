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
		// cTool��(��) ���� ��ӵ�
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

		//���� �߰��� ��带 Ʈ���� ����
		void ConnectNode();

		//����� ��尡 �ִ��� Ȯ��
		void FindConnectNode(const ImVec2& mousePos, weak_ptr<cTask> task);

		//Ʈ�� ��尣 �̵�
		void MoveNode();

		//Ʈ�� ��� �� �̵��� ��� ���õǾ����� Ȯ��
		void SelectMoveNode(const ImVec2& mousePos,
							weak_ptr<cTask> task,
							function<void(weak_ptr<cTask>, UINT)> func);

		//������ ��尡 �ִٸ� ����
		void DeleteNode(weak_ptr<cTask> task);
	private:
		ImVec2 GetNodeStart(shared_ptr<cTask> task); //�θ� ��
		ImVec2 GetNodeEnd(shared_ptr<cTask> task, UINT childNum); //�ڽ� ��
		ImVec2 GetMousePos();
		bool MouseIsInCircle(const ImVec2& mouse, const ImVec2& circlePos);
	private:
		const float NODE_SLOT_RADIUS = 5.5f;
		const ImVec2 NODE_WINDOW_PADDING = ImVec2(8.0f, 8.0f);
		const float MOUSE_PADDING = -58.0f;
		const float NODE_GAP = 25.0f; //�ڽ� ��� �� ����

		weak_ptr<cBehaviorTree> _bTree;

		ImVec2 _scrolling = ImVec2(0.0f, 0.0f);
		
		ImDrawList* _drawList;
		ImVec2 _offset;
		bool _openContextMenu = false;
		weak_ptr<cTask> _hoveredTask;
		
		weak_ptr<cTask> _selectedTask; //���� ���õ� ���

		//Ʈ���� �߰����� ���� ���
		vector<shared_ptr<cTask>> _newTasks;
		bool _clickNewNode = false; //Ŭ���Ǿ�����
		int _selectedNewTask; //�� ��� ����Ʈ �� ���õ� ���
		weak_ptr<cTask> _connectedTask; //������ ���

		//@first : �̵��� ����� �θ� ��� @second : �̵��� ����� ��ȣ
		pair<weak_ptr<cTask>, UINT> _nodeToMove;
		pair<weak_ptr<cTask>, UINT> _targetToMove;
		bool _clickMoveNode = false; //�̵��� ��尡 Ŭ���Ǿ�����
	};
}