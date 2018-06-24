#include "stdafx.h"
#include "cGizmo.h"
#include "cMovingGizmo.h"
#include "cScalingGizmo.h"
#include "cRotatingGizmo.h"
#include "./Model/cModel.h"
#include "./Helper/cMath.h"
#include "sTransform.h"

cGizmo::cGizmo()
{
	_myTransform = make_unique<sTransform>();

	{
		D3D11_RASTERIZER_DESC desc;
		cStates::GetRasterizerDesc(&desc);
		cStates::CreateRasterizer(&desc, &_rasterizer[0]);

		desc.CullMode = D3D11_CULL_NONE;
		cStates::CreateRasterizer(&desc, &_rasterizer[1]);
	}

	{
		D3D11_DEPTH_STENCIL_DESC desc;
		cStates::GetDepthStencilDesc(&desc);
		cStates::CreateDepthStencil(&desc, &_depthStenciler[0]);

		desc.DepthEnable = false;
		cStates::CreateDepthStencil(&desc, &_depthStenciler[1]);
	}

	{
		D3D11_BLEND_DESC desc;
		cStates::GetBlendDesc(&desc);
		cStates::CreateBlend(&desc, &_blender[0]);

		desc.RenderTarget[0].BlendEnable = true;

		// 덧셈합성
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		cStates::CreateBlend(&desc, &_blender[1]);
	}
}

cGizmo::~cGizmo()
{
	SAFE_RELEASE(_rasterizer[0]);
	SAFE_RELEASE(_rasterizer[1]);

	SAFE_RELEASE(_depthStenciler[0]);
	SAFE_RELEASE(_depthStenciler[1]);
}

void cGizmo::SetGlobalVariable(weak_ptr<sGlobalVariable> global)
{
	_global = global;
	//이동 기즈모 생성
	_gizmos.emplace_back(make_unique<cMovingGizmo>(global));
	//크기 기즈모 생성
	_gizmos.emplace_back(make_unique<cScalingGizmo>(global));
	//회전 기즈모 생성
	_gizmos.emplace_back(make_unique<cRotatingGizmo>(global));
}

void cGizmo::AddTransform(weak_ptr<sTransform> transform)
{
	//최근에 선택된 트랜스폼이 대표 트랜스폼
	_delegateTransform = transform.lock();

	//기즈모 트랜스폼 갱신
	cBasicGizmo::SetTransform(_delegateTransform);

	//모델 추가
	_pickedTransforms.emplace_back(_delegateTransform);
}

void cGizmo::SetMyTransform()
{
	auto modelPtr = _delegateTransform.lock();

	_myTransform->Position = modelPtr->Position;
	_myTransform->Rotation = modelPtr->Rotation;
	_myTransform->Quaternion = modelPtr->Quaternion;
	_myTransform->Update();
}

void cGizmo::GetScaleRate(const D3DXVECTOR3 & camPos)
{
	auto temp = _myTransform->Position - camPos;
	auto length = D3DXVec3Length(&temp);

	if (length <= 10.0f)
		_myTransform->Scaling = { 0.5f,0.5f,0.5f };
	else
	{
		_myTransform->Scaling = D3DXVECTOR3{ length , length, length } * 0.025f;
	}
}

void cGizmo::GetInverseVector(OUT D3DXVECTOR3 *transPos, OUT D3DXVECTOR3* transDir, const D3DXVECTOR3& originPos, const D3DXVECTOR3& originDir)
{
	D3DXMATRIX invWorld;
	D3DXMatrixInverse(&invWorld, nullptr, &_myTransform->Matrix);
	D3DXVec3TransformCoord(transPos, &originPos, &invWorld);
	D3DXVec3TransformNormal(transDir, &originDir, &invWorld);
}

void cGizmo::SelectMode()
{
	if (cKeyboard::Get()->Down('Z'))
		_selectedNum = 0;
	else if (cKeyboard::Get()->Down('X'))
		_selectedNum = 1;
	else if(cKeyboard::Get()->Down('C'))
		_selectedNum = 2;
}

void cGizmo::Update()
{
	if (_delegateTransform.expired())
		return;

	SelectMode();

	auto globalPtr = _global.lock();
	auto dir = globalPtr->MainCamera->GetDirection(globalPtr->Viewport.get(), globalPtr->Perspective.get());

	D3DXVECTOR3 pos;
	globalPtr->MainCamera->GetPosition(&pos);

	GetScaleRate(pos);

	//역변환 행렬로 위치, 방향 구함
	D3DXVECTOR3 transPos, transDir;
	GetInverseVector(&transPos, &transDir, pos, dir);

	_gizmos[_selectedNum]->Update(_myTransform->Matrix, transPos, transDir);

	SetMyTransform();
}

void cGizmo::Render()
{
	if (_delegateTransform.expired())
		return;

	//todo : 이걸 지우면 잘 나옴
	D3D::GetDC()->OMSetDepthStencilState(_depthStenciler[1], 1);
	D3D::GetDC()->RSSetState(_rasterizer[1]);
	D3D::GetDC()->OMSetBlendState(_blender[1], NULL, 0xFF);

	_myTransform->SetVSBuffer(1);
	_gizmos[_selectedNum]->Render();

	D3D::GetDC()->OMSetBlendState(_blender[0], NULL, 0xFF);
	D3D::GetDC()->RSSetState(_rasterizer[0]);
	D3D::GetDC()->OMSetDepthStencilState(_depthStenciler[0], 1);
}

void cGizmo::PostRender()
{
	if (_delegateTransform.expired())
		return;

	auto modelPtr = _delegateTransform.lock();
	ImGui::Begin("Inspector");
	{
		float minFloat = cMath::FloatMinValue;
		float maxFloat = cMath::FloatMaxValue;
		bool isClick = false;

		D3DXVECTOR3 tempRotation = modelPtr->Rotation;
		ImGui::Separator();

		ImGui::Text("Transform");
		isClick = ImGui::InputFloat3("Position", modelPtr->Position, 3);
		isClick |= ImGui::InputFloat3("Rotation", modelPtr->Rotation, 3);
		isClick |= ImGui::InputFloat3("Scale", modelPtr->Scaling, 3);

		ImGui::Separator();

		if (isClick)
		{
			modelPtr->Rotate(modelPtr->Rotation - tempRotation);
			modelPtr->Update();
		}
	}
	ImGui::End();
}
