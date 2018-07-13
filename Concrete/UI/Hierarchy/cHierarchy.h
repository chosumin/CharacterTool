#pragma once
#include "./Message/cBaseGameEntity.h"

class cModel;
class cActor;
class iUI;
class cHierarchy :
	public cBaseGameEntity
{
public:
	cHierarchy();
	~cHierarchy();
public:
	void Init();
	void Update();
	void Render();
	void PostRender();

	// cBaseGameEntity을(를) 통해 상속됨
	virtual void HandleMessage(const sTelegram & msg) override;
private:
	virtual void FunctionInitialize() override;
private:
	ImGuiTreeNodeFlags _bOpen;
	

	unique_ptr<class cModelHierarchy> _modelHierarchy;
	
};