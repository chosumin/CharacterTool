#pragma once
#include "./Message/cBaseGameEntity.h"

class cActor;
class cActorManager :
	public iExecutable,
	public cBaseGameEntity
{
public:
	cActorManager(weak_ptr<sGlobalVariable> global);
	~cActorManager();

	// iExecutable��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void Update() override;
	virtual void PreRender() override {}
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void ResizeScreen() override {}

	//�޽��� ó��
	virtual void HandleMessage(const sTelegram & msg) override;
private:
	//�޽��� ó�� �����ƾ �ʱ�ȭ
	virtual void FunctionInitialize() override;

	//���� �޽��� ����
	void SendStartMessage();
private:
	void StopScene();

	//���� ���� ���� ���� ����
	void SetEditedActor(const weak_ptr<cActor>* actor);

	//�� ���� ����
	void SetEnemy(const weak_ptr<cActor>* actor);
private:
	weak_ptr<sGlobalVariable> _global;
	
	unique_ptr<class cCollisionManager> _collisionManager;
	shared_ptr<cActor> _editedActor;
	shared_ptr<cActor> _enemy;

	bool _isStart;
};