#pragma once
#include <functional>
#include "sTelegram.h"

//최상위 클래스
class cBaseGameEntity
	: public inheritable_enable_shared_from_this<cBaseGameEntity>
{
private:
	friend class cEntityManager;
public:
	cBaseGameEntity() {}
	virtual ~cBaseGameEntity() {}

	UINT GetID() const { return id; }
	//메세지 통신
	virtual void HandleMessage(const sTelegram& msg) = 0;
protected:
	virtual void FunctionInitialize() = 0;

	//해당 객체가 메시지를 받은 뒤 그 메시지에 따라 해야할 콜백 함수
	map<eMessageType, function<void(const sTelegram&)>> msgMap;
private:
	UINT id; //객체 식별 번호
};