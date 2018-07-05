#pragma once
#include "../../stdafx.h"

enum class eMessageType
{
	None
};

struct sTelegram
{
	UINT sender; //송신자 id
	vector<UINT> receivers; //수신자 id, 배열
	eMessageType message; //메시지 종류
	DWORD dispatchTime; //지연시간, 지연시간 경과 후 메시지 이벤트 호출
	void* extraInfo; //추가 정보

	sTelegram()
	{
		message = eMessageType::None;
		extraInfo = NULL;
	}

	sTelegram(DWORD dispatchTime, UINT sender, vector<UINT> receivers, eMessageType type, void* extraInfo)
		:dispatchTime(dispatchTime), sender(sender), receivers(receivers), message(type), extraInfo(extraInfo)
	{
	}

	bool operator<(const sTelegram& ref) const
	{
		return dispatchTime < ref.dispatchTime;
	}
};