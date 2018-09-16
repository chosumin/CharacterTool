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
		// cTool을(를) 통해 상속됨
		virtual void Update() override;
		virtual void Render() override;
		virtual void ShowHierarchy(int i) override;
		virtual void ShowInspector() override;
		virtual void SaveJson(Json::Value& root) override;
		virtual void LoadJson() override;
	private:
		//유저, ai 중 블랙보드 선택
		void SelectBlackboardType();

		//블랙보드 데이터의 초기값 지정
		void SetInitialDataOfBlackboard();

		//설정한 값을 블랙보드에 적용
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