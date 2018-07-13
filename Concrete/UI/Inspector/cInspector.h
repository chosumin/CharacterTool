#pragma once
#include "./Message/cBaseGameEntity.h"

class cInspector :
	public cBaseGameEntity
{
public:
	cInspector();
	~cInspector();

	void Init();
	void Update();
	void Render();
	void PostRender();

	void SetMesh(weak_ptr<class cModelMesh> mesh);

	// cBaseGameEntity을(를) 통해 상속됨
	virtual void HandleMessage(const sTelegram & msg) override;
private:
	virtual void FunctionInitialize() override;
private:
	weak_ptr<class cActor> _actor;
	unique_ptr<class cColliderInspector> _colInspector;
};