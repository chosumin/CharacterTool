#pragma once
#include <functional>
#include "sTelegram.h"

//�ֻ��� Ŭ����
class cBaseGameEntity
	: public inheritable_enable_shared_from_this<cBaseGameEntity>
{
private:
	friend class cEntityManager;
public:
	cBaseGameEntity() {}
	virtual ~cBaseGameEntity() {}

	UINT GetID() const { return id; }
	//�޼��� ���
	virtual void HandleMessage(const sTelegram& msg) = 0;
protected:
	virtual void FunctionInitialize() = 0;

	//�ش� ��ü�� �޽����� ���� �� �� �޽����� ���� �ؾ��� �ݹ� �Լ�
	map<eMessageType, function<void(const sTelegram&)>> msgMap;
private:
	UINT id; //��ü �ĺ� ��ȣ
};