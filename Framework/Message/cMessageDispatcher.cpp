#include "stdafx.h"
#include "cMessageDispatcher.h"
#include "cBaseGameEntity.h"
#include "sTelegram.h"
#include "cEntityManager.h"

void cMessageDispatcher::DispatchMessages(DWORD delay, UINT sender, vector<UINT> receiver, eMessageType msg, void* extraInfo)
{
	sTelegram telegram(0, sender, receiver, msg, extraInfo);

	Send(delay, telegram);
}

void cMessageDispatcher::DispatchMessages(DWORD delay, UINT sender, UINT receiver, eMessageType msg, void* extraInfo)
{
	vector<UINT> vec;
	vec.push_back(receiver);
	sTelegram telegram(0, sender, vec, msg, extraInfo);

	Send(delay, telegram);
}

cMessageDispatcher::cMessageDispatcher()
	:_isFinish(false)
{
	_process = thread(&cMessageDispatcher::DispatchDelayedMessages, this);
}

cMessageDispatcher::~cMessageDispatcher()
{
	_isFinish = true;
	_process.join();
}

void cMessageDispatcher::DispatchDelayedMessages()
{
	//������ ����
	while (_isFinish == false)
	{
		if (_priorityQ.size() <= 0) continue;
		
		//���� �ð��� �� ������ ����
		DWORD curTime = timeGetTime();
		if (_priorityQ.begin()->dispatchTime < curTime && _priorityQ.begin()->dispatchTime > 0)
		{
			//ť�� �� �� ���� ������
			auto telegram = *_priorityQ.begin();

			//������ ã�Ƴ�
			auto receiverID = telegram.receivers;
			for (auto i : receiverID)
			{
				//fixme : ���� ���ù� ��ü�� ������ ��Ȳ���� �޽����� �����Եȴٸ�?
				auto pReceiver = cEntityManager::Get()->GetEntityFromId(i);

				//������ ������
				if(!pReceiver.expired())
					pReceiver.lock()->HandleMessage(telegram);
			}
			_priorityQ.erase(_priorityQ.begin());
		}
	}
}

void cMessageDispatcher::Send(DWORD delay, sTelegram & telegram)
{
	if (delay <= 0.0f)
	{
		for (auto i : telegram.receivers)
		{
			auto pReceiver = cEntityManager::Get()->GetEntityFromId(i);

			//������ ������
			if(!pReceiver.expired())
				pReceiver.lock()->HandleMessage(telegram);
		}
	}
	else
	{
		DWORD curTime = timeGetTime();
		telegram.dispatchTime = curTime + delay;
		_priorityQ.insert(telegram);
	}
}