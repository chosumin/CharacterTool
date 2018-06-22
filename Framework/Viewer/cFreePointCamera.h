#pragma once
#include "cCamera.h"

class cFreePointCamera : public cCamera
{
public:
	cFreePointCamera(float fMoveSpeed = 20, float fRotationSpeed = 2.5f);
	~cFreePointCamera();

	// cCamera��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void PostRender() override;
private:
	float m_fMoveSpeed;
	float m_fRotationSpeed;
};