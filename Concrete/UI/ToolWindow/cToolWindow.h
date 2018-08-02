#pragma once
#include "./Message/cBaseGameEntity.h"

class cActor;
namespace UI
{
	class cTool;
	class cToolWindow : public cBaseGameEntity
	{
	public:
		cToolWindow();
		~cToolWindow();
	public:
		void Init();
		void Update();
		void Render();
		void PostRender();

		// cBaseGameEntity을(를) 통해 상속됨
		virtual void HandleMessage(const sTelegram & msg) override;
	private:
		virtual void FunctionInitialize() override;

		void RecieveActor(const sTelegram & msg);
		void RecieveTool(const sTelegram & msg);
		void SaveActor(const sTelegram & msg);
	private:
		weak_ptr<cActor> _actor;
		vector<shared_ptr<cTool>> _tools;
		weak_ptr<cTool> _selectedTool;
	};
}