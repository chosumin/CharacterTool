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

	// cBaseGameEntity��(��) ���� ��ӵ�
	virtual void HandleMessage(const sTelegram & msg) override;
private:
	virtual void FunctionInitialize() override;
private:
	ImGuiTreeNodeFlags _bOpen;
	

	unique_ptr<class cModelHierarchy> _modelHierarchy;
	
};