#pragma once

class cGameObject
{
public:
	cGameObject();
	~cGameObject();
private:
	shared_ptr<struct sTransform> _transform;
	//todo : 충돌체 컨테이너 추가
	//todo : 메쉬 추가
	//todo : 모델 추가
};