#include "stdafx.h"
#include "cDirectionalLight.h"
#include "./GameObject/cActor.h"
#include "./Model/cModel.h"
#include "./Graphic/cMaterial.h"
#include "./Graphic/cRenderTarget.h"
#include "./Helper/cMath.h"
#include "./Graphic/cTexture.h"

cDirectionalLight::cDirectionalLight(weak_ptr<sGlobalVariable> global)
	: _position(0,0,0) //빛의 포지션
	, _lookAt(0, 0, 0) //빛이 쏘여질 방향
	, _global(global)
{
	D3DXMatrixIdentity(&_view);
	D3DXMatrixIdentity(&_projection);

	_shader = cShader::Create(Shader + L"Shadow_Skinned.hlsl");
	_renderTarget = make_unique<cRenderTarget>();

	_lightVPBuffer = make_unique<cViewProjectionBuffer>();
}

cDirectionalLight::~cDirectionalLight()
{
}

void cDirectionalLight::AddObject(shared_ptr<class cActor> & actor)
{
	_objects.emplace_back(actor);
}

ID3D11ShaderResourceView* cDirectionalLight::SRV()
{
	return _renderTarget->GetSRV();
}

void cDirectionalLight::Update()
{
	_position = _lookAt - _global.lock()->GlobalLight->Data.Direction * 200.0f;

	//View
	{
		D3DXVECTOR3 up = { 0,1,0 };
		D3DXMatrixLookAtLH(&_view, &_position, &_lookAt, &up);
	}

	//그림자를 표현하려면 width : height 가 1 : 1이어야하므로 수직시야각이 90도이어야함
	//Projection
	{
		D3DXMatrixPerspectiveFovLH(&_projection, cMath::PI * 0.5f, 1.0f, 1.0f, 1000.0f);
	}
}

void cDirectionalLight::Render()
{
	_renderTarget->Set(0xFFFFFFFF);

	_lightVPBuffer->SetView(_view);
	_lightVPBuffer->SetProjection(_projection);
	_lightVPBuffer->SetVSBuffer(10);

	//그림자용 쉐이더
	_shader->Render();

	for (auto&& object : _objects)
	{
		auto objPtr = object.lock();
		if (!objPtr)
			continue;

		objPtr->RenderShadow();
	}
}
