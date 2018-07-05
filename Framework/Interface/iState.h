#pragma once
#include <memory>

template<typename T>
class iState
{
public:
	virtual ~iState() {}
	//�ٲ� �� �ʱ� ���� ����
	virtual void Enter(std::weak_ptr<T> object) = 0;
	//�� �����Ӹ��� ������Ʈ �� ����
	virtual void Update() {}
	//����� ������ ó�� �޼ҵ�
	virtual void End() {}
	//���� ����
	virtual unique_ptr<iState<T>> OnMessage(const struct sTelegram& msg) = 0;
};