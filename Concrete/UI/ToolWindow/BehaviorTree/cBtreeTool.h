#pragma once
#include "../cTool.h"

namespace UI
{
	class cBtreeTool : public cTool
	{
	public:
		cBtreeTool();
		~cBtreeTool();

	public:
		// cTool을(를) 통해 상속됨
		virtual void Update() override;
		virtual void Render() override;
		virtual void ShowHierarchy(int i) override;
		virtual void ShowInspector() override;
		virtual void SaveJson(Json::Value& root) override;
		virtual void LoadJson() override;
	};
}