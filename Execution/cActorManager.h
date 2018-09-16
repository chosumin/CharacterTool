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

	// iExecutable을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void PreRender() override {}
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void ResizeScreen() override {}

	//메시지 처리
	virtual void HandleMessage(const sTelegram & msg) override;
private:
	//메시지 처리 서브루틴 초기화
	virtual void FunctionInitialize() override;

	//시작 메시지 전달
	void SendStartMessage();
private:
	void StopScene();

	//현재 수정 중인 액터 설정
	void SetEditedActor(const weak_ptr<cActor>* actor);

	//적 액터 설정
	void SetEnemy(const weak_ptr<cActor>* actor);
private:
	weak_ptr<sGlobalVariable> _global;
	
	unique_ptr<class cCollisionManager> _collisionManager;
	shared_ptr<cActor> _editedActor;
	shared_ptr<cActor> _enemy;

	bool _isStart;
};