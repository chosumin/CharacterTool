#pragma once

enum class eMessageType;
struct sTelegram
{
	UINT sender; //�۽��� id
	vector<UINT> receivers; //������ id, �迭
	eMessageType message; //�޽��� ����
	DWORD dispatchTime; //�����ð�, �����ð� ��� �� �޽��� �̺�Ʈ ȣ��
	void* extraInfo; //�߰� ����

	sTelegram();

	sTelegram(DWORD dispatchTime, UINT sender, vector<UINT> receivers, eMessageType type, void* extraInfo);

	bool operator<(const sTelegram& ref) const;
};