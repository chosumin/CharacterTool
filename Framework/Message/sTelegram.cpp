#include "stdafx.h"
#include "sTelegram.h"
#include "./Concrete/Message/eMessageType.h"

sTelegram::sTelegram()
{
	message = eMessageType::NONE;
	extraInfo = NULL;
}

sTelegram::sTelegram(DWORD dispatchTime, UINT sender, vector<UINT> receivers, eMessageType type, void * extraInfo) 
	: dispatchTime(dispatchTime), sender(sender), receivers(receivers), message(type), extraInfo(extraInfo)
{
}

bool sTelegram::operator<(const sTelegram & ref) const
{
	return dispatchTime < ref.dispatchTime;
}
