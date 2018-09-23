#include "stdafx.h"
#include "cThirdPersonCamera.h"
#include "./Transform/sTransform.h"
#include "./Helper/cMath.h"

cThirdPersonCamera::cThirdPersonCamera(weak_ptr<sTransform> transform, float rotationSpeed, float dstToObject, float angleX)
	: _transformToFollow(transform)
	, _originRotationSpeed(rotationSpeed)
	, _rotationSpeed(0.0f)
	, _dstToObject(dstToObject)
{
}

cThirdPersonCamera::~cThirdPersonCamera()
{
}

void cThirdPersonCamera::SetTransformToFollow(const weak_ptr<sTransform>& transform)
{
	_transformToFollow = transform;
	auto tmPtr = _transformToFollow.lock();
	_currentPos = tmPtr->Position;
	_nextPos = tmPtr->Position;
}

void cThirdPersonCamera::SetRotationSpeed(float speed)
{
	_originRotationSpeed = speed;
	_rotationSpeed = speed;
}

void cThirdPersonCamera::SetDistToObject(float dstToObejct)
{
	_dstToObject = dstToObejct;
}

void cThirdPersonCamera::Update()
{
	auto tmPtr = _transformToFollow.lock();
	if(tmPtr)
		_nextPos = tmPtr->Position;

	if (cMath::IsVec3Equal(_nextPos, _currentPos))
		_interpolate = 0.0f;

	_interpolate += cFrame::Delta() * 0.05f;

	if (_interpolate >= 1.0f)
		_interpolate = 1.0f;

	//타겟 위치부터 거리를 떨어뜨림
	auto gap = -m_vForward * _dstToObject;
	D3DXVec3Lerp(&_currentPos, &_currentPos, &_nextPos, _interpolate);
	gap += _currentPos;

	SetPosition(gap.x, gap.y + 40.0f, gap.z);

	//마우스 회전에 따라 회전함
	auto vRotation = cMouse::Get()->GetMoveValue();
	Rotate(vRotation.x, vRotation.y, _rotationSpeed + 3.0f);
}

void cThirdPersonCamera::PostRender()
{
}
