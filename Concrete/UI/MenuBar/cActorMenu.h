#pragma once
#include "./Message/cBaseGameEntity.h"

class cActor;
namespace UI
{
	class cActorMenu : public cBaseGameEntity
	{
	public:
		cActorMenu();
		~cActorMenu();

		void Init();
		void Update();
		void PostRender();

		// cBaseGameEntity을(를) 통해 상속됨
		virtual void HandleMessage(const sTelegram & msg) override;
	private:
		virtual void FunctionInitialize() override;

		void NewActor();
	private:
		weak_ptr<cActor> _actor;
	};
}