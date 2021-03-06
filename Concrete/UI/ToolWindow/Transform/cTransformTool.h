#pragma once
#include "../cTool.h"

namespace UI
{
	class cTransformTool : public cTool
	{
	public:
		cTransformTool();
		~cTransformTool();
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
		void ShowTransform();
	};
}