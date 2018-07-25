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

	_axises.emplace_back(make_unique<cBoxCollider>(_transform, D3DXVECTOR3{ -rate,-rate,_length }, D3DXVECTOR3{ rate,rate,0 }));
}

cRotatingGizmo::~cRotatingGizmo()
{
}

void cRotatingGizmo::Update(const D3DXMATRIX & gizmoWorld, const D3DXMATRIX & gizmoLocal, const D3DXVECTOR3 & mousePos, const D3DXVECTOR3 & mouseDir)
{
	auto transformPtr = _transform.lock();
	bool intersect = false;
	if (_isClick == false)
		intersect = IsIntersect(gizmoWorld, mousePos, mouseDir);

	if (intersect && cMouse::Get()->Down(0))
	{
		_isClick = true;
		SetFixedMatrix(true, gizmoLocal);
	}

	if (_isClick && cMouse::Get()->Press(0))
	{
		//���� ȸ�� ��ȯ
		transformPtr->RotateToFixedMatrix(_fixedMatrix, GetDelta(gizmoWorld, &gizmoLocal));
		transformPtr->Update();

		//todo : ���� ����� ���� ���� ����� ����
	}
	else if (_isClick && cMouse::Get()->Up(0))
		_isClick = false;

	UpdateCollider(gizmoWorld);
}

void cRotatingGizmo::Update(const D3DXMATRIX & gizmoLocal, const D3DXVECTOR3 & mousePos, const D3DXVECTOR3 & mouseDir)
{
	auto transformPtr = _transform.lock();
	bool intersect = false;
	if (_isClick == false)
		intersect = IsIntersect(gizmoLocal, mousePos, mouseDir);

	if (intersect && cMouse::Get()->Down(0))
	{
		_isClick = true;
		SetFixedMatrix(false, gizmoLocal);
	}

	if (_isClick && cMouse::Get()->Press(0))
	{
		transformPtr->RotateToFixedMatrix(_fixedMatrix, GetDelta(gizmoLocal));
		transformPtr->Update();
	}
	else if (_isClick && cMouse::Get()->Up(0))
		_isClick = false;

	UpdateCollider(gizmoLocal);
}

void cRotatingGizmo::SetFixedMatrix(bool isWorld, const D3DXMATRIX& local)
{
	//���� ��� ��Ÿ���� ���� ��Ŀ��� ȸ�� ��ĸ� �̾ƿ�
	if (isWorld)
	{
		sTransform temp;
		temp.Matrix = local;
		temp.Decompose();
		_fixedMatrix = temp.GetRotationMatrix();
	}
	//���� ���
	else
	{
		auto transformPtr = _transform.lock();
		_fixedMatrix = transformPtr->GetRotationMatrix();
	}
}

D3DXVECTOR3 cRotatingGizmo::GetDelta(const D3DXMATRIX& colMatrix, const D3DXMATRIX* updateMatrix)
{
	auto globalPtr = _global.lock();
	D3DXMATRIX view, proj;
	globalPtr->MainCamera->GetMatrix(&view);
	globalPtr->Perspective->GetMatrix(&proj);

	auto mouse = GetMouseDelta();

	D3DXVECTOR3 dir, total = { 0,0,0 };
	for (int i = 0; i < 3; i++)
	{
		//���� ���� �� �������� ����ȯ
		dir = cMath::Mul(_direction, axis[i]);
		D3DXVec3TransformNormal(&dir, &dir, &colMatrix);
		D3DXVec3TransformNormal(&dir, &dir, &view);
		D3DXVec3TransformNormal(&dir, &dir, &proj);

		//����� ���콺 ��Ÿ �� ����
		auto deltaF = D3DXVec3Dot(&dir, &mouse);

		//_direction�� ���� �� ���ϱ�
		auto delta = axis[i];
		D3DXVec3Normalize(&delta, &delta);

		delta *= deltaF;
		total += delta * 100.0f;
	}

	if (updateMatrix)
	{
		//colMatrix�� ���Ͽ� ȸ���� �Ϸ���
		/*D3DXMATRIX invMat;
		D3DXMatrixInverse(&invMat, nullptr, &colMatrix);
		auto finalMat = invMat * *updateMatrix;

		D3DXVec3TransformNormal(&total, &total, &finalMat);*/
	}

	auto transform = _transform.lock();
	transform->Rotation += total;

	return total;
}