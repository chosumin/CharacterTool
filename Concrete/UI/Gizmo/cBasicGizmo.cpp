#include "stdafx.h"
#include "cBasicGizmo.h"
#include "./Collider/cBoxCollider.h"
#include "./Collider/cQuadCollider.h"
#include "./Collider/cRayCollider.h"
#include "./Transform/sTransform.h"
#include "./Interface/iRenderable.h"

weak_ptr<sTransform> cBasicGizmo::_transform;
bool cBasicGizmo::_isClick = false;
float cBasicGizmo::rate = 0.1f;
float cBasicGizmo::_length = 4.0f;

cBasicGizmo::cBasicGizmo(weak_ptr<sGlobalVariable> global)
	: _global(global)
	, _prevMousePos(0, 0, 0)
	, _curMousePos(0, 0, 0)
{
	_quads.emplace_back(make_unique<cQuadCollider>(_transform, D3DXVECTOR3{ 0,0,0 }, D3DXVECTOR3{ 1,0,1 }, D3DXCOLOR{ 0,1,0,0.7f }));

	_quads.emplace_back(make_unique<cQuadCollider>(_transform, D3DXVECTOR3{ 0,1,0 }, D3DXVECTOR3{ 0,0,1 }, D3DXCOLOR{ 1,0,0,0.7f }));

	_quads.emplace_back(make_unique<cQuadCollider>(_transform, D3DXVECTOR3{ 0,1,0 }, D3DXVECTOR3{ 1,0,0 }, D3DXCOLOR{ 0,0,1,0.7f }));
}

cBasicGizmo::~cBasicGizmo()
{
}

void cBasicGizmo::SetTransform(weak_ptr<sTransform> transform)
{
	_transform = transform;
}

void cBasicGizmo::Render()
{
	for (int i = 0; i < 3; i++)
	{
		_axises[i]->RenderGizmo();
		_meshes[i]->Render();
	}

	for (auto&& quad : _quads)
		quad->Render();
}

void cBasicGizmo::UpdateCollider(const D3DXMATRIX& gizmoMatrix)
{
	for (auto&& axis : _axises)
	{
		axis->Update();
		axis->SetWorld(gizmoMatrix);
	}
	for (auto&& quad : _quads)
	{
		quad->Update();
		quad->SetWorld(gizmoMatrix);
	}
}

/*******************************
	����� ��, ������ ���� üũ
********************************/
bool cBasicGizmo::IsIntersect(const D3DXMATRIX& world, const D3DXVECTOR3 & pos, const D3DXVECTOR3 & dir)
{
	for (auto&& axis : _axises)
		axis->ResetState();
	for (auto&& quad : _quads)
		quad->ResetState();

	auto temp = weak_ptr<sTransform>();

	//��� ��ȯ�� ����
	auto ray = make_shared<cRayCollider>(temp, pos, dir);

	for (UINT i = 0; i < _quads.size(); i++)
	{
		if (_quads[i]->Contains(ray) == eContainmentType::Intersects)
		{
			_direction = GetDirection(i + _axises.size());
			return true;
		}
	}

	for (UINT i = 0; i < _axises.size(); i++)
	{
		if (_axises[i]->Contains(ray) == eContainmentType::Intersects)
		{
			_direction = GetDirection(i);
			return true;
		}
	}

	return false;
}

D3DXVECTOR3 cBasicGizmo::GetMouseDelta()
{
	//todo : ����Ʈ �ڵ����� �ٲٱ�
	_prevMousePos = _curMousePos;
	_curMousePos = cMouse::Get()->GetPosition();
	auto mouse = _curMousePos - _prevMousePos;
	D3DXVec3Normalize(&mouse, &mouse);
	mouse.x = ((2.0f * mouse.x) / 800);
	mouse.y = (((2.0f * mouse.y) / 600)) * -1.0f;
	return mouse * 5.0f;
}

D3DXVECTOR3 cBasicGizmo::GetDirection(UINT num)
{
	switch (num)
	{
		case 0:
			return { 1,0,0 };
		case 1:
			return { 0,1,0 };
		case 2:
			return { 0,0,1 };
		case 3:
			return { 1,0,1 };
		case 4:
			return { 0,1,1 };
		case 5:
			return { 1,1,0 };
	}
	return{ 0,0,0 };
}