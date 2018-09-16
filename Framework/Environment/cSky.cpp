#include "stdafx.h"
#include "cSky.h"
#include "./Model/cModel.h"
#include "./Model/FbxExporter/Exporter.h"
#include "./Model/cModelFactory.h"
#include "./Model/ModelPart/cModelMesh.h"
#include "./Model/ModelPart/cModelBone.h"
#include "./Graphic/cMaterial.h"

cSky::cSky(weak_ptr<sGlobalVariable> global)
	: _global(global)
	, shaderFile(Shader + L"World_Sky.hlsl")
{
	model = cModelFactory::Create(Model, L"Sky");
	for (auto&& mat : model->GetMaterials())
	{
		mat->SetShader(shaderFile);
	}

	buffer = make_unique<Buffer>();

	{
		D3D11_RASTERIZER_DESC desc;
		cStates::GetRasterizerDesc(&desc);
		cStates::CreateRasterizer(&desc, &rasterizer[0]);

		desc.FrontCounterClockwise = true;
		cStates::CreateRasterizer(&desc, &rasterizer[1]);
	}

	{
		D3D11_DEPTH_STENCIL_DESC desc;
		cStates::GetDepthStencilDesc(&desc);
		cStates::CreateDepthStencil(&desc, &depthStencilState[0]);

		desc.DepthEnable = false;
		cStates::CreateDepthStencil(&desc, &depthStencilState[1]);
	}
}

cSky::~cSky()
{
	SAFE_RELEASE(rasterizer[0]);
	SAFE_RELEASE(rasterizer[1]);

	SAFE_RELEASE(depthStencilState[0]);
	SAFE_RELEASE(depthStencilState[1]);
}

void cSky::Update()
{
	D3DXVECTOR3 position;
	sGlobalVariable::MainCamera.lock()->GetPosition(&position);

	D3DXMATRIX world;
	D3DXMatrixTranslation(&world, position.x, position.y, position.z);

	D3DXMATRIX root;
	D3DXMatrixIdentity(&root);
	model->GetBone(L"Sphere").lock()->Animate(world, &root);
	model->Update(weak_ptr<sTransform>());
}

void cSky::Render()
{
	D3D::GetDC()->OMSetDepthStencilState(depthStencilState[1], 1);
	D3D::GetDC()->RSSetState(rasterizer[1]);

	buffer->SetPSBuffer(2);
	model->Render();

	D3D::GetDC()->RSSetState(rasterizer[0]);
	D3D::GetDC()->OMSetDepthStencilState(depthStencilState[0], 1);
}

void cSky::PostRender()
{
	if (_global.lock()->GuiSettings->bShowEnvironmentWindow)
	{
		ImGui::Begin("Environment");
		ImGui::ColorEdit4("Center", buffer->Data.Center);
		ImGui::ColorEdit4("Apex", buffer->Data.Apex);
		ImGui::SliderFloat("Height", &buffer->Data.Height, 0, 5);
		ImGui::End();
	}
}
