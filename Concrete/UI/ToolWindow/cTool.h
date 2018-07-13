#pragma once
#include "./Message/cBaseGameEntity.h"

class cActor;
namespace UI
{
	class cTool : public cBaseGameEntity
	{
	public:
		cTool();
		virtual ~cTool();

		virtual void Init();
		virtual void HandleMessage(const sTelegram & msg) override;

		void SetActor(weak_ptr<cActor> actor);
	public:
		virtual void Update() = 0;
		virtual void Render() = 0;

		virtual void ShowHierarchy() = 0;
		virtual void ShowInspector() = 0;
	protected:
		bool AlertActor();
		void SendMe();
	private:
		virtual void FunctionInitialize() override;
	protected:
		weak_ptr<cActor> _actor;
	};
}