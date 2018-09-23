#include "stdafx.h"
#include "cWorldGrid.h"
#include "./Mesh/MeshUtility.h"
#include "./Message/eIdGroup.h"
#include "./Message/eMessageType.h"
#include "./GameObject/cActor.h"
#include "./Light/cDirectionalLight.h"
#include "./Model/cModel.h"
#include "./Graphic/cMaterial.h"

cWorldGrid::cWorldGrid(weak_ptr<sGlobalVariable> global)
	: _vertexSize(0)
{
	_world = make_unique<cWorldBuffer>();
	_shader = cShader::Create(Shader + L"GridPlane.hlsl");

	CreatePlane();

	cMeshUtility<VertexType>::CreateVertexBuffer(&_vb, _vertices);

	_vertexSize = _vertices.size();

	//Create Blend State
	{
		D3D11_BLEND_DESC desc;
		cStates::GetBlendDesc(&desc);
		cStates::CreateBlend(&desc, &_alphaBlend[0]);

		desc.RenderTarget[0].BlendEnable = true;
		cStates::CreateBlend(&desc, &_alphaBlend[1]);
	}

	_directionalLight = make_unique<cDirectionalLight>(global);
}

cWorldGrid::~cWorldGrid()
{
	SAFE_RELEASE(_vb);
	cEntityManager::Get()->RemoveEntity(GetID());
}

void cWorldGrid::Init()
{
	cEntityManager::Get()->RegisterEntity(eIdGroup::TERRAIN, shared_from_this());
	FunctionInitialize();

	_shadowBuffer = make_unique<cViewProjectionBuffer>();
}

void cWorldGrid::Update()
{
	_directionalLight->Update();
}

void cWorldGrid::PreRender()
{
	//그림자 맵
	_directionalLight->Render();
}

void cWorldGrid::Render()
{
	if (cDebug::GetDebugMode() == false)
		return;

	auto view = _directionalLight->SRV();
	D3D::GetDC()->PSSetShaderResources(5, 1, &view);

	D3D::GetDC()->OMSetBlendState(_alphaBlend[1], nullptr, 0xFF);

	_world->SetVSBuffer(1);
	_shader->Render();

	cMeshUtility<VertexType>::Render(_vb, _vertexSize, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3D::GetDC()->OMSetBlendState(_alphaBlend[0], nullptr, 0xFF);
}

void cWorldGrid::CreatePlane(float width, float height)
{
	VertexType v;
	v.color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	v.position = D3DXVECTOR3(-width * 0.5f, 0, -height * 0.5f);
	_vertices.push_back(v);
	v.position = D3DXVECTOR3(-width * 0.5f, 0, height * 0.5f);
	_vertices.push_back(v);
	v.position = D3DXVECTOR3(width * 0.5f, 0, -height * 0.5f);
	_vertices.push_back(v);
	v.position = D3DXVECTOR3(width * 0.5f, 0, -height * 0.5f);
	_vertices.push_back(v);
	v.position = D3DXVECTOR3(-width * 0.5f, 0, height * 0.5f);
	_vertices.push_back(v);
	v.position = D3DXVECTOR3(width * 0.5f, 0, height * 0.5f);
	_vertices.push_back(v);
}

void cWorldGrid::HandleMessage(const sTelegram & msg)
{
	msgMap[msg.message](msg);
}

void cWorldGrid::FunctionInitialize()
{
	//액터 등록
	msgMap[eMessageType::REGISTER_ACTOR] = [this](const sTelegram& msg)
	{
		auto actor = (shared_ptr<cActor>*)(msg.extraInfo);
		_directionalLight->AddObject(*actor);
	};
}
