#include "stdafx.h"
#include "cCollider.h"
#include "./Graphic/ConstBuffer/cColliderBuffer.h"
#include "./Transform/sTransform.h"
#include "./Helper/cMath.h"

cCollider::cCollider(weak_ptr<sTransform> boneTransform,
					 D3DXVECTOR3 center)
	: _boneTransform(boneTransform)
	, _center(center)
{
	_worldBuffer = make_unique<cWorldBuffer>();
	
	_shader = cShader::Create(Shader + L"002_Collider.hlsl");
	_cbuffer = make_unique<cColliderBuffer>();

	_localTransform = make_shared<sTransform>();
	SetLocalTransform();
}

cCollider::~cCollider()
{
}

void cCollider::Update()
{
	//todo : 기즈모 변경 가능하도록 변경
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

void cCollider::SetLocalTransform()
{
	_localTransform->Position = _center;
	_localTransform->Scaling = { 100,100,100 };
	_localTransform->Update();
}
