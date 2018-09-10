#pragma once
#include "../../stdafx.h"

enum class eMessageType
{
	NONE,

	/*************
		UI ����
	**************/
	RECIEVE_ACTOR,
	RECIEVE_TOOL,
	SAVE_ACTOR,
	LOAD_ACTOR,
	CLICK_START,
};

struct sTelegram
{
	UINT sender; //�۽��� id
	vector<UINT> receivers; //������ id, �迭
	eMessageType message; //�޽��� ����
	DWORD dispatchTime; //�����ð�, �����ð� ��� �� �޽��� �̺�Ʈ ȣ��
	void* extraInfo; //�߰� ����

	sTelegram()
	{
		message = eMessageType::NONE;
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