#include "stdafx.h"
#include "cGizmo.h"
#include "cMovingGizmo.h"
#include "cScalingGizmo.h"
#include "cRotatingGizmo.h"
#include "./Model/cModel.h"
#include "./Helper/cMath.h"
#include "./Transform/sTransform.h"

cGizmo::cGizmo()
{
	_myLocal = make_unique<sTransform>();
	_myWorld = make_unique<sTransform>();

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

		// �����ռ�
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
	//�̵� ����� ����
	_gizmos.emplace_back(make_unique<cMovingGizmo>(global));
	//ũ�� ����� ����
	_gizmos.emplace_back(make_unique<cScalingGizmo>(global));
	//ȸ�� ����� ����
	_gizmos.emplace_back(make_unique<cRotatingGizmo>(global));
}

void cGizmo::AddTransform(weak_ptr<sTransform> localTransform,
						  weak_ptr<sTransform> worldTransform)
{
	//�ֱٿ� ���õ� Ʈ�������� ��ǥ Ʈ������
	_delegateLocal = localTransform;
	_delegateWorld = worldTransform;

	//����� Ʈ������ ����
	cBasicGizmo::SetTransform(_delegateLocal);

	//�� �߰�
	//_pickedTransforms.emplace_back(_delegateTransform);
}

void cGizmo::SetMyTransform()
{
	auto delegateLocalPtr = _delegateLocal.lock();

	_myLocal->Position = delegateLocalPtr->Position;
	_myLocal->Rotation = delegateLocalPtr->Rotation;
	_myLocal->Quaternion = delegateLocalPtr->Quaternion;
	_myLocal->Update();

	auto delegateWorldPtr = _delegateWorld.lock();
	if (delegateWorldPtr)
	{
		D3DXVECTOR3 scale;
		
		_myWorld->Position = delegateWorldPtr->Position;
		_myWorld->Rotation = delegateWorldPtr->Rotation;
		_myWorld->Quaternion = delegateWorldPtr->Quaternion;
		_myWorld->Update();
	}
}

void cGizmo::SetScaleRate(const D3DXVECTOR3 & camPos)
{
	auto temp = _myLocal->Position - camPos;

	auto worldPtr = _delegateWorld.lock();
	if (worldPtr)
		temp = worldPtr->Position - camPos;

	auto length = D3DXVec3Length(&temp);

	if (length <= 10.0f)
	{
		_myLocal->Scaling = { 0.5f,0.5f,0.5f };
		_myWorld->Scaling = { 0.5f,0.5f,0.5f };
	}
	else
	{
		_myLocal->Scaling = D3DXVECTOR3{ length , length, length } *0.025f;
		_myWorld->Scaling = D3DXVECTOR3{ length , length, length } *0.025f;
	}
}

void cGizmo::SelectMode()
{
	if (cKeyboard::Get()->Down('Z'))
		_selectedNum = 0;
	else if (cKeyboard::Get()->Down('X'))
		_selectedNum = 1;
	else if (cKeyboard::Get()->Down('C'))
		_selectedNum = 2;
}

void cGizmo::Update()
{
	if (_delegateLocal.expired())
		return;

	SelectMode();

	auto globalPtr = _global.lock();
	auto dir = globalPtr->MainCamera->GetDirection(globalPtr->Viewport.get(), globalPtr->Perspective.get());

	D3DXVECTOR3 pos;
	globalPtr->MainCamera->GetPosition(&pos);

	SetScaleRate(pos);

	//���� ������Ʈ
	auto renderingWorldPtr = _delegateWorld.lock();
	if (renderingWorldPtr)
	{
		_gizmos[_selectedNum]->Update(_myWorld->Matrix, _myLocal->Matrix, pos, dir);
	}
	//���� ������Ʈ
	else
		_gizmos[_selectedNum]->Update(_myLocal->Matrix, pos, dir);

	SetMyTransform();
}

void cGizmo::Render()
{
	if (_delegateLocal.expired())
		return;

	//todo : �̰� ����� �� ����
	D3D::GetDC()->OMSetDepthStencilState(_depthStenciler[1], 1);
	D3D::GetDC()->RSSetState(_rasterizer[1]);
	D3D::GetDC()->OMSetBlendState(_blender[1], NULL, 0xFF);
	
	//���� Ʈ������ �������� myTransform, ���� Ʈ�������� renderingWorld
	auto renderingWorldPtr = _delegateWorld.lock();
	if (renderingWorldPtr)
		_myWorld->SetVSBuffer(1);
	else
		_myLocal->SetVSBuffer(1);
	_gizmos[_selectedNum]->Render();

	D3D::GetDC()->OMSetBlendState(_blender[0], NULL, 0xFF);
	D3D::GetDC()->RSSetState(_rasterizer[0]);
	D3D::GetDC()->OMSetDepthStencilState(_depthStenciler[0], 1);
}

void cGizmo::PostRender()
{
	if (_delegateLocal.expired())
		return;

	auto localPtr = _delegateLocal.lock();

	bool isClick = false;

	D3DXVECTOR3 tempRotation = localPtr->Rotation;
	isClick = ImGui::InputFloat3("Position", localPtr->Position, 3);
	isClick |= ImGui::InputFloat3("Rotation", localPtr->Rotation, 3);
	isClick |= ImGui::InputFloat3("Scale", localPtr->Scaling, 3);

	if (isClick)
	{
		localPtr->Rotate(localPtr->Rotation - tempRotation);
		localPtr->Update();
	}
}
