#include "stdafx.h"
#include "cMovingGizmo.h"
#include "./Collider/cArrowCollider.h"
#include "./Transform/sTransform.h"
#include "./Helper/cMath.h"

cMovingGizmo::cMovingGizmo(weak_ptr<sGlobalVariable> global)
	:cBasicGizmo(global)
{
	_axises.emplace_back(make_unique<cArrowCollider>(_length, D3DXCOLOR{ 1,0,0,1 }, D3DXVECTOR3{ 1,0,0 }, D3DXVECTOR3{ 0,-rate,-rate }, D3DXVECTOR3{ _length,rate,rate }));

	_axises.emplace_back(make_unique<cArrowCollider>(_length, D3DXCOLOR{ 0,1,0,1 }, D3DXVECTOR3{ 0,1,0 }, D3DXVECTOR3{ -rate,0,-rate }, D3DXVECTOR3{ rate,_length,rate }));

	_axises.emplace_back(make_unique<cArrowCollider>(_length, D3DXCOLOR{ 0,0,1,1 }, D3DXVECTOR3{ 0,0,1 }, D3DXVECTOR3{ -rate,-rate,-_length }, D3DXVECTOR3{ rate,rate,0 }));
}

cMovingGizmo::~cMovingGizmo()
{
}

void cMovingGizmo::Update(const D3DXMATRIX & gizmoMatrix, const D3DXVECTOR3 & mousePos, const D3DXVECTOR3 & mouseDir)
{
	//마우스와 기즈모 교차 체크
	bool intersect = false;
	if (_isClick == false)
		intersect = IsIntersect(mousePos, mouseDir);

	if (intersect && cMouse::Get()->Down(0))
		_isClick = true;

	if (_isClick && cMouse::Get()->Press(0))
		Move(gizmoMatrix);
	else if (_isClick && cMouse::Get()->Up(0))
		_isClick = false;
}

void cMovingGizmo::Move(const D3DXMATRIX& matrix)
{
	auto globalPtr = _global.lock();
	D3DXMATRIX view, proj;
	globalPtr->MainCamera->GetMatrix(&view);
	globalPtr->Perspective->GetMatrix(&proj);

	auto mouse = GetMouseDelta();

	D3DXVECTOR3 dir, total = { 0,0,0 };
	for (int i = 0; i < 3; i++)
	{
		//현재 축을 뷰 프로젝션 역변환
		dir = cMath::Mul(_direction, axis[i]);
		D3DXVec3TransformNormal(&dir, &dir, &matrix);
		D3DXVec3TransformNormal(&dir, &dir, &view);
		D3DXVec3TransformNormal(&dir, &dir, &proj);

		//누른 축을 트랜스폼 행렬로 변환
		auto delta = axis[i];
		D3DXVec3TransformNormal(&delta, &delta, &matrix);

		//방향과 마우스 델타 값 내적
		auto deltaF = D3DXVec3Dot(&dir, &mouse);

		delta *= deltaF;
		total += delta;
	}

	auto transform = _transform.lock();
	//todo : 소프트 코딩으로 변경
	transform->Position += total * 4.0f;
	transform->Update();
}