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

		// cBaseGameEntity��(��) ���� ��ӵ�
		virtual void HandleMessage(const sTelegram & msg) override;
	private:
		virtual void FunctionInitialize() override;

		void NewActor();
		void SaveActor();

		//���Ͱ� ������ ��� ���â ���
		bool AlertActor();
	private:
		string _actorName;
		weak_ptr<cActor> _actor;
	};
}