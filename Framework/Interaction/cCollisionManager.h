#pragma once
#include "./Message/cBaseGameEntity.h"

class cActor;
class cCollisionManager : public cBaseGameEntity
{
public:
	cCollisionManager();
	~cCollisionManager();

	void Update();

	//test : �浹 �׽�Ʈ
	void SetPlayer(const weak_ptr<cActor> & player)
	{
		_player = player;
	}
	void SetEnemy(const weak_ptr<cActor> & enemy)
	{
		_enemy = enemy;
	}
public:
	// cBaseGameEntity��(��) ���� ��ӵ�
	virtual void HandleMessage(const sTelegram & msg) override;
	virtual void FunctionInitialize() override;
private:
	weak_ptr<cActor> _player;
	weak_ptr<cActor> _enemy;
};