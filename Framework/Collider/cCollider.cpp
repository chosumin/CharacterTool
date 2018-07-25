#include "stdafx.h"
#include "cCollider.h"
#include "./Graphic/ConstBuffer/cColliderBuffer.h"
#include "./Transform/sTransform.h"
#include "./Helper/cMath.h"

cCollider::cCollider(weak_ptr<sTransform> parentTransform)
	:_parentTransform(parentTransform)
{
	_shader = cShader::Create(Shader + L"002_Collider.hlsl");
	_cbuffer = make_unique<cColliderBuffer>();

	_localTransform = make_shared<sTransform>();

	_worldTransform = make_shared<sTransform>();
	
	Update();
}

cCollider::~cCollider()
{
}

void cCollider::Update()
{
	_localTransform->Update();

	if (_parentTransform.expired())
		*_worldTransform = *_localTransform;
	else
	{
		*_worldTransform = *_localTransform * *_parentTransform.lock();
	}
}

void cCollider::Render()
{
	_worldTransform->SetVSBuffer(1);
	_shader->Render();
	_cbuffer->SetPSBuffer(2);
}

void cCollider::ResetState()
{
	_cbuffer->Data.Intersect = 0;
}

weak_ptr<sTransform> cCollider::GetLocalTransform() const
{
	return _localTransform;
}

void cCollider::SetLocalTransform(const D3DXMATRIX & matrix)
{
	_localTransform->Matrix = matrix;
	_localTransform->Decompose();
}

D3DXMATRIX& cCollider::GetWorld() const
{
	return _worldTransform->Matrix;
}

void cCollider::SetWorld(const D3DXMATRIX & world)
{
	_worldTransform->Matrix = world;
	_worldTransform->SetMatrix(world);
}

weak_ptr<sTransform> cCollider::GetWorldTransform() const
{
	return _worldTransform;
}