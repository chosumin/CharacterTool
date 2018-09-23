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
		// cTool��(��) ���� ��ӵ�
		virtual void Init() override;
		virtual void Update() override;
		virtual void Render() override;
		virtual void ShowHierarchy(int i) override;
		virtual void ShowInspector() override;
		virtual void SaveJson(Json::Value& root) override;
		virtual void LoadJson() override;
	private:
		//���̾��Ű���� �׸� ����
		void SelectBTree();
		
		//������ ��尡 �ִٸ� ����
		void DeleteNode(weak_ptr<cTask> task);
	private:
		/**************
			��ư ����
		***************/
		void LoadBehaviorTree();
		void SaveBehaviorTree();
	private:
		/***********************
			�ൿƮ�� ���� ȭ��
		************************/
		void ShowBehaviorTreeTool();

		void DrawBackground();

		//������ � ���
		void DrawLinks();
		void DrawLink(const shared_ptr<cTask> & task);

		//��� ���
		void DrawNodes();
		void DrawNode(OUT int& id, const shared_ptr<cTask> & task);

		//�޴� �˾� ���
		void DrawTextMenu();
		//��� ���� �˾�
		void DrawAddMenu();
		//��� ��ġ ����(�θ� ���� ��� ��ġ��ŭ)
		void SetNodePosition(shared_ptr<cTask> & task, const ImVec2& parentPos);
		//������ ����� ����, ���� �޴�
		void DrawNodeMenu();

		//���� �߰��� ��带 Ʈ���� ����
		void ConnectNode();
		//����� ��尡 �ִ��� Ȯ��
		//@param : ���콺 ��ǥ
		//@param : ��� ���
		void FindConnectNode(const ImVec2& mousePos, const shared_ptr<cTask> & task);

		//Ʈ�� ��尣 �̵�
		void MoveNode();
		//Ʈ�� ��� �� �̵��� ��� ���õǾ����� Ȯ��
		//@param : ���콺 ��ǥ
		//@param : ��� ���
		//@param : �̵��� ��� ���� �� �ݹ� �Լ�
		void SelectMoveNode(const ImVec2& mousePos,
							const shared_ptr<cTask> & task,
							function<void(weak_ptr<cTask>, UINT)> func);

		void Scroll();

		void EndBackground();
	private:
		//�θ� ��
		ImVec2 GetNodeStart(const shared_ptr<cTask> & task);
		//�ڽ� ��
		ImVec2 GetNodeEnd(const shared_ptr<cTask> & task, UINT childNum);
		//���콺 ��ġ
		ImVec2 GetMousePos();

		//�� �� ���� ����
		bool Intersect(const ImVec2& mouse, const ImVec2& circlePos);
	private:
		const float NODE_SLOT_RADIUS = 5.5f; //�� ������
		const ImVec2 NODE_WINDOW_PADDING = ImVec2(8.0f, 8.0f);
		const float MOUSE_PADDING = 0;// -58.0f;
		const float NODE_GAP = 25.0f; //�ڽ� ��� �� ����
		ImVec2 _scrolling = ImVec2(0.0f, 0.0f); //��ũ�� ��ġ
		
		weak_ptr<cBehaviorTree> _bTree;

		ImDrawList* _drawList; //ĵ����
		ImVec2 _offset; //���콺�� ��ũ�� ��ġ ���� ��
		bool _openContextMenu = false;
		weak_ptr<cTask> _hoveredTask;
		
		weak_ptr<cTask> _selectedTask; //���� ���õ� ���

		//Ʈ���� �߰����� ���� ���
		shared_ptr<cRootTask> _parentOfNewTasks;
		bool _clickNewNode = false; //Ŭ���Ǿ�����
		int _selectedNewTask; //�� ��� ����Ʈ �� ���õ� ���
		weak_ptr<cTask> _connectedTask; //������ ���
		ImVec2 _newTaskPos; //���� ������ ����� ��ġ

		//@first : �̵��� ����� �θ� ��� @second : �̵��� ����� ��ȣ
		pair<weak_ptr<cTask>, UINT> _nodeToMove;
		pair<weak_ptr<cTask>, UINT> _targetToMove;
		bool _clickMoveNode = false; //�̵��� ��尡 Ŭ���Ǿ�����

		shared_ptr<cTask> _copiedTask;

		shared_ptr<cBlackboardTool> _blackboardTool;
	};
}