#pragma once
#include "../cTool.h"

class cBlackboard;
namespace UI
{
	class cBlackboardTool : public cTool
	{
	public:
		cBlackboardTool();
		~cBlackboardTool();
	public:
		// cTool��(��) ���� ��ӵ�
		virtual void Update() override;
		virtual void Render() override;
		virtual void ShowHierarchy(int i) override;
		virtual void ShowInspector() override;
		virtual void SaveJson(Json::Value& root) override;
		virtual void LoadJson() override;
	private:
		//����, ai �� ������ ����
		void SelectBlackboardType();

		//������ �������� �ʱⰪ ����
		void SetInitialDataOfBlackboard();

		//������ ���� �����忡 ����
		void ApplyData();
	private:
		bool _init;
		weak_ptr<cBlackboard> _blackboard;

		unordered_map<string, D3DXVECTOR3> _vector3s;
		unordered_map<string, bool> _bools;
		unordered_map<string, float> _floats;
		unordered_map<string, int> _ints;
	};
}