#pragma once

class cBaseGameEntity;
enum class eMessageType;
struct sTelegram;
class cMessageDispatcher : public cSingletone<cMessageDispatcher>
{
private:
	friend class cSingletone<cMessageDispatcher>;
public:
	//다수의 리시버에게 메시지를 보낸다.
	//@param : receiver가 메시지를 받을 지연 시간
	//@param : 보내는 객체의 id
	//@param : 받을 객체의 id 컨테이너
	//@param : 메시지 타입
	//@param : 보내줄 상세 정보
	void DispatchMessages(DWORD delay, UINT sender, vector<UINT> receiver, eMessageType msg,
						 void* extraInfo);

	//리시버에게 메시지를 보낸다.
	//@param : receiver가 메시지를 받을 지연 시간
	//@param : 보내는 객체의 id
	//@param : 받을 객체의 id
	//@param : 메시지 타입
	//@param : 보내줄 상세 정보
	void DispatchMessages(DWORD delay, UINT sender, UINT receiver, eMessageType msg, void* extraInfo);
private:
	cMessageDispatcher();
	~cMessageDispatcher();

	//대기중인 전보들을 스레드에서 계속 검사
	void DispatchDelayedMessages();
	void Send(DWORD delay, sTelegram& telegram);
private:
	//중복 된 전보들을 자동으로 제거, 우선순위 큐
	multiset<sTelegram> _priorityQ;

	//메시시 큐 순회 스레드
	thread _process;
	bool _isFinish;
};