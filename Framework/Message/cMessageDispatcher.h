#pragma once

class cBaseGameEntity;
enum class eMessageType;
struct sTelegram;
class cMessageDispatcher : public cSingletone<cMessageDispatcher>
{
private:
	friend class cSingletone<cMessageDispatcher>;
public:
	//�ټ��� ���ù����� �޽����� ������.
	//@param : receiver�� �޽����� ���� ���� �ð�
	//@param : ������ ��ü�� id
	//@param : ���� ��ü�� id �����̳�
	//@param : �޽��� Ÿ��
	//@param : ������ �� ����
	void DispatchMessages(DWORD delay, UINT sender, vector<UINT> receiver, eMessageType msg,
						 void* extraInfo);

	//���ù����� �޽����� ������.
	//@param : receiver�� �޽����� ���� ���� �ð�
	//@param : ������ ��ü�� id
	//@param : ���� ��ü�� id
	//@param : �޽��� Ÿ��
	//@param : ������ �� ����
	void DispatchMessages(DWORD delay, UINT sender, UINT receiver, eMessageType msg, void* extraInfo);
private:
	cMessageDispatcher();
	~cMessageDispatcher();

	//������� �������� �����忡�� ��� �˻�
	void DispatchDelayedMessages();
	void Send(DWORD delay, sTelegram& telegram);
private:
	//�ߺ� �� �������� �ڵ����� ����, �켱���� ť
	multiset<sTelegram> _priorityQ;

	//�޽ý� ť ��ȸ ������
	thread _process;
	bool _isFinish;
};