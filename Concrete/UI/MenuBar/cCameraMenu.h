#pragma once
#include "./Message/cBaseGameEntity.h"

class cActor;
namespace UI
{
	class cCameraMenu : public cBaseGameEntity
	{
	public:
		cCameraMenu();
		~cCameraMenu();

		void Init();
		void Update();
		void PostRender();

		// cBaseGameEntity을(를) 통해 상속됨
		virtual void HandleMessage(const sTelegram & msg) override;
	private:
		virtual void FunctionInitialize() override;
	};
}