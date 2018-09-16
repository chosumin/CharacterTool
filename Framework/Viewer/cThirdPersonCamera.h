#pragma once
#include "cCamera.h"

struct sTransform;
class cThirdPersonCamera : public cCamera
{
public:
	cThirdPersonCamera(weak_ptr<sTransform> transform, float rotationSpeed, float dstToObject = 120.0f, float angleX = 80.0f);
	~cThirdPersonCamera();

	void SetTransformToFollow(const weak_ptr<sTransform>& transform);
	void SetRotationSpeed(float speed);
	void SetDistToObject(float dstToObejct);

	// cCamera을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PostRender() override;
private:
	weak_ptr<sTransform> _transformToFollow;
	float _originRotationSpeed;
	float _rotationSpeed;
	float _dstToObject;
	float _interpolate;

	D3DXVECTOR3 _nextPos;
	D3DXVECTOR3 _currentPos;
};