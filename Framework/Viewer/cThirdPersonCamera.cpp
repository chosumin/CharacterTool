#include "stdafx.h"
#include "cThirdPersonCamera.h"
#include "./Transform/sTransform.h"

cThirdPersonCamera::cThirdPersonCamera(weak_ptr<sTransform> transform, float rotationSpeed, float dstToObject, float angleX)
	: _transformToFollow(transform)
	, _rotationSpeed(rotationSpeed)
	, _dstToObject(dstToObject)
	, _angleX(angleX)
	, _nextDstToObject(_dstToObject)
	, _nextDeltaX(0.0f)
	, _interpolation(0.0f)
{
}

cThirdPersonCamera::~cThirdPersonCamera()
{
}

void cThirdPersonCamera::SetTransformToFollow(const weak_ptr<sTransform>& transform)
{
	_transformToFollow = transform;
}

void cThirdPersonCamera::SetRotationSpeed(float speed)
{
	_rotationSpeed = speed;
}

void cThirdPersonCamera::SetDistToObject(float dstToObejct)
{
	_dstToObject = dstToObejct;
}

void cThirdPersonCamera::SetAngleX(float angleX)
{
	_angleX = angleX;
}

void cThirdPersonCamera::Update()
{
	//마우스 회전에 따라 회전함
	auto vRotation = cMouse::Get()->GetMoveValue();
	/*if (vRotation.x > D3DX_16F_EPSILON)
		_nextDeltaX = vRotation.x;*/

	m_vRotation.x = _angleX * RADIAN;
	Rotate(vRotation.x, 0, _rotationSpeed);

	//평행이동은 플레이어 위치따라
	auto tmPtr = _transformToFollow.lock();
	if (tmPtr)
	{
		//타겟 위치부터 거리를 떨어뜨림
		auto gap = -m_vForward * _dstToObject;
		gap += tmPtr->Position;

		SetPosition(gap.x, gap.y, gap.z);
	}
}

void cThirdPersonCamera::PostRender()
{
}
