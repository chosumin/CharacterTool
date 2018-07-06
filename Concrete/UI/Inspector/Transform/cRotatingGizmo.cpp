#include "stdafx.h"
#include "cRotatingGizmo.h"
#include "./Collider/cBoxCollider.h"
#include "./Collider/cQuadCollider.h"
#include "./Transform/sTransform.h"
#include "./GizmoMeshes.h"
#include "./Helper/cMath.h"

cRotatingGizmo::cRotatingGizmo(weak_ptr<sGlobalVariable> global)
	:cBasicGizmo(global)
{
	_meshes.emplace_back(make_unique<cRotateLine>(_length, D3DXCOLOR{ 1,0,0,1 }, D3DXVECTOR3{ 1,0,0 }));

	_meshes.emplace_back(make_unique<cRotateLine>(_length, D3DXCOLOR{ 0,1,0,1 }, D3DXVECTOR3{ 0,1,0 }));

	_meshes.emplace_back(make_unique<cRotateLine>(_length, D3DXCOLOR{ 0,0,1,1 }, D3DXVECTOR3{ 0,0,1 }));

	_axises.emplace_back(make_unique<cBoxCollider>(_transform, D3DXVECTOR3{ 0,-rate,-rate }, D3DXVECTOR3{ _length,rate,rate }));

	_axises.emplace_back(make_unique<cBoxCollider>(_transform, D3DXVECTOR3{ -rate,0,-rate }, D3DXVECTOR3{ rate,_length,rate }));

	_axises.emplace_back(make_unique<cBoxCollider>(_transform, D3DXVECTOR3{ -rate,-rate,-_length }, D3DXVECTOR3{ rate,rate,0 }));
}

cRotatingGizmo::~cRotatingGizmo()
{
}

void cRotatingGizmo::Update(const D3DXMATRIX & gizmoMatrix, const D3DXVECTOR3 & mousePos, const D3DXVECTOR3 & mouseDir)
{
	auto transformPtr = _transform.lock();
	bool intersect = false;
	if (_isClick == false)
		intersect = IsIntersect(mousePos, mouseDir);

	if (intersect && cMouse::Get()->Down(0))
	{
		_isClick = true;
		_fixedMatrix = transformPtr->GetRotationMatrix();
	}

	if (_isClick && cMouse::Get()->Press(0))
	{
		transformPtr->RotateToFixedMatrix(_fixedMatrix, GetDelta(gizmoMatrix));
		transformPtr->Update();
	}
	else if (_isClick && cMouse::Get()->Up(0))
		_isClick = false;

	for (auto&& axis : _axises)
		axis->SetWorld(gizmoMatrix);

	for (auto&& quad : _quads)
		quad->SetWorld(gizmoMatrix);
}

D3DXVECTOR3 cRotatingGizmo::GetDelta(const D3DXMATRIX& matrix)
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

		//방향과 마우스 델타 값 내적
		auto deltaF = D3DXVec3Dot(&dir, &mouse);

		//_direction과 내적 값 곱하기
		auto delta = axis[i];
		delta *= deltaF;
		total += delta * 100.0f;
	}

	auto transform = _transform.lock();
	transform->Rotation += total;

	return total;
}