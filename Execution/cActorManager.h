#pragma once
#include "./Message/cBaseGameEntity.h"

class cActorManager :
	public iExecutable,
	public cBaseGameEntity
{
public:
	cActorManager(weak_ptr<sGlobalVariable> global);
	~cActorManager();

	// iExecutable을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void PreRender() override {}
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void ResizeScreen() override {}

	// cBaseGameEntity을(를) 통해 상속됨
	virtual void HandleMessage(const sTelegram & msg) override;
private:
	virtual void FunctionInitialize() override;

	void SendStartMessage();
private:
	weak_ptr<sGlobalVariable> _global;
	shared_ptr<class cActor> _actor;

	bool _isStart;
};