#include "stdafx.h"
#include "cScalingGizmo.h"
#include "GizmoMeshes.h"
#include "./Collider/cBoxCollider.h"
#include "./Collider/cQuadCollider.h"
#include "./Transform/sTransform.h"
#include "./Helper/cMath.h"

cScalingGizmo::cScalingGizmo(weak_ptr<sGlobalVariable> global)
	:cBasicGizmo(global)
{
	auto len = rate * 4.0f;
	_meshes.emplace_back(make_unique<cBoxLine>(_length, D3DXCOLOR{ 1,0,0,1 }, D3DXVECTOR3{ 1,0,0 }, D3DXVECTOR3{ 0,-len,-len }, D3DXVECTOR3{ _length,len,len }));

	_meshes.emplace_back(make_unique<cBoxLine>(_length, D3DXCOLOR{ 0,1,0,1 }, D3DXVECTOR3{ 0,1,0 }, D3DXVECTOR3{ -len,0,-len }, D3DXVECTOR3{ len,_length,len }));

	_meshes.emplace_back(make_unique<cBoxLine>(_length, D3DXCOLOR{ 0,0,1,1 }, D3DXVECTOR3{ 0,0,1 }, D3DXVECTOR3{ -len,-len,-0 }, D3DXVECTOR3{ len,len,_length }));

	_axises.emplace_back(make_unique<cBoxCollider>(_transform, D3DXVECTOR3{ 0,-len,-len }, D3DXVECTOR3{ _length,len,len }));

	_axises.emplace_back(make_unique<cBoxCollider>(_transform, D3DXVECTOR3{ -len,0,-len }, D3DXVECTOR3{ len,_length,len }));

	_axises.emplace_back(make_unique<cBoxCollider>(_transform, D3DXVECTOR3{ -len,-len,0 }, D3DXVECTOR3{ len,len,_length }));
}

cScalingGizmo::~cScalingGizmo()
{
}

void cScalingGizmo::Update(const D3DXMATRIX & gizmoWorld, const D3DXMATRIX & gizmoLocal, const D3DXVECTOR3 & mousePos, const D3DXVECTOR3 & mouseDir)
{
	bool intersect = false;
	if (_isClick == false)
		intersect = IsIntersect(gizmoWorld, mousePos, mouseDir);

	if (intersect && cMouse::Get()->Down(0))
		_isClick = true;

	if (_isClick && cMouse::Get()->Press(0))
		Scale(gizmoWorld, &gizmoLocal);

	if (_isClick && cMouse::Get()->Up(0))
		_isClick = false;

	UpdateCollider(gizmoWorld);
}

void cScalingGizmo::Update(const D3DXMATRIX & gizmoLocal, const D3DXVECTOR3 & mousePos, const D3DXVECTOR3 & mouseDir)
{
	bool intersect = false;
	if (_isClick == false)
		intersect = IsIntersect(gizmoLocal, mousePos, mouseDir);

	if (intersect && cMouse::Get()->Down(0))
		_isClick = true;

	if (_isClick && cMouse::Get()->Press(0))
		Scale(gizmoLocal);

	if (_isClick && cMouse::Get()->Up(0))
		_isClick = false;

	UpdateCollider(gizmoLocal);
}

void cScalingGizmo::Scale(const D3DXMATRIX& colMatrix, const D3DXMATRIX* updateMatrix)
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
		D3DXVec3TransformNormal(&dir, &dir, &colMatrix);
		D3DXVec3TransformNormal(&dir, &dir, &view);
		D3DXVec3TransformNormal(&dir, &dir, &proj);

		//누른 축을 트랜스폼 행렬로 변환
		auto delta = axis[i];
		D3DXVec3TransformNormal(&delta, &delta, &colMatrix);
		D3DXVec3Normalize(&delta, &delta);

		//방향과 마우스 델타 값 내적
		auto deltaF = D3DXVec3Dot(&dir, &mouse);

		delta *= deltaF;
		total += delta;
	}

	if (updateMatrix)
	{
		//todo : 나중에 시간되면 디버깅
		sTransform temp;
		temp.Matrix = colMatrix;
		temp.Decompose();

		D3DXVec3TransformNormal(&total, &total, &temp.GetRotationMatrix());
	}

	auto transform = _transform.lock();
	//todo : 소프트코딩으로 바꾸기
	transform->Scaling += total * 10.0f;
	
	transform->Scaling.x = max(transform->Scaling.x, D3DX_16F_EPSILON);
	transform->Scaling.y = max(transform->Scaling.y, D3DX_16F_EPSILON);
	transform->Scaling.z = max(transform->Scaling.z, D3DX_16F_EPSILON);

	transform->Update();
}