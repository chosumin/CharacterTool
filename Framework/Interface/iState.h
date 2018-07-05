#pragma once
#include <memory>

template<typename T>
class iState
{
public:
	virtual ~iState() {}
	//바뀐 뒤 초기 상태 지정
	virtual void Enter(std::weak_ptr<T> object) = 0;
	//매 프레임마다 업데이트 할 내용
	virtual void Update() {}
	//종료시 마지막 처리 메소드
	virtual void End() {}
	//상태 전이
	virtual unique_ptr<iState<T>> OnMessage(const struct sTelegram& msg) = 0;
};