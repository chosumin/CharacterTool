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
		virtual void ShowHierarchy() override;
		virtual void ShowInspector() override;
	private:
		void ShowTransform();
	};
}