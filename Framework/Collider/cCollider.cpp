#include "stdafx.h"
#include "cCollider.h"
#include "./Graphic/ConstBuffer/cColliderBuffer.h"
#include "./Transform/sTransform.h"
#include "./Helper/cMath.h"

cCollider::cCollider(weak_ptr<sTransform> boneTransform)
	:_boneTransform(boneTransform)
{
	_worldBuffer = make_unique<cWorldBuffer>();

	_shader = cShader::Create(Shader + L"002_Collider.hlsl");
	_cbuffer = make_unique<cColliderBuffer>();

	_localTransform = make_shared<sTransform>();

	Update();
}

cCollider::~cCollider()
{
}

void cCollider::Update()
{
	_localTransform->Update();

	if (_boneTransform.expired())
	{
		_world = _localTransform->Matrix;
		_scale = _localTransform->Scaling;
	}
	else
	{
		_world = _localTransform->Matrix * _boneTransform.lock()->Matrix;
		_scale = cMath::Mul(_localTransform->Scaling, _boneTransform.lock()->Scaling);
	}
	_worldBuffer->SetMatrix(_world);
}

void cCollider::Render()
{
	_worldBuffer->SetVSBuffer(1);
	_shader->Render();
	_cbuffer->SetPSBuffer(2);
}

weak_ptr<sTransform> cCollider::GetLocalTransform() const
{
	return _localTransform;
}

D3DXMATRIX cCollider::GetWorld() const
{
	return _world;
}

void cCollider::SetWorld(const D3DXMATRIX & world)
{
	_worldBuffer->SetMatrix(world);
}