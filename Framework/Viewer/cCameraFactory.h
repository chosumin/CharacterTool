#pragma once
#include "./Common/cSingletone.h"

struct sTransform;
class cFreePointCamera;
class cThirdPersonCamera;
class cCameraFactory : public cSingletone<cCameraFactory>
{
private:
	friend class cSingletone<cCameraFactory>;
public:
	weak_ptr<cCamera> GetThirdPersonCamera(weak_ptr<sTransform> transformToFollow, float rotationSpeed = 1.0f);
	weak_ptr<cCamera> GetFreePointCamera(const D3DXVECTOR3& position, const D3DXVECTOR2& rotationDegree);
private:
	cCameraFactory();
	~cCameraFactory();
private:
	shared_ptr<cThirdPersonCamera> _thirdPersonCamera;
	shared_ptr<cFreePointCamera> _freePointCamera;
};