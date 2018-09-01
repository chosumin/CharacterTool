#include "stdafx.h"
#include "cCameraFactory.h"
#include "cThirdPersonCamera.h"
#include "cFreePointCamera.h"

weak_ptr<cCamera> cCameraFactory::GetThirdPersonCamera(weak_ptr<sTransform> transformToFollow, float rotationSpeed)
{
	_thirdPersonCamera->SetTransformToFollow(transformToFollow);
	
	_thirdPersonCamera->SetRotationSpeed(rotationSpeed);
	
	return _thirdPersonCamera;
}

weak_ptr<cCamera> cCameraFactory::GetFreePointCamera(const D3DXVECTOR3 & position, const D3DXVECTOR2 & rotationDegree)
{
	_freePointCamera->SetPosition(position.x, position.y, position.z);

	_freePointCamera->SetRotationDegree(rotationDegree.x, rotationDegree.y);
	
	return _freePointCamera;
}

cCameraFactory::cCameraFactory()
{
	_thirdPersonCamera = make_shared<cThirdPersonCamera>(weak_ptr<sTransform>(), 10.0f);

	_freePointCamera = make_shared<cFreePointCamera>();
}

cCameraFactory::~cCameraFactory()
{
}
