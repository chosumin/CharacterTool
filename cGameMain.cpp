#include "stdafx.h"
#include "cGameMain.h"

#include "./Execution/cModels.h"
#include "./Execution/cEnvironment.h"
#include "./Execution/cMenuBar.h"

#include "./Viewer/cFreePointCamera.h"
#include "./Helper/cMath.h"

cGameMain::cGameMain()
{
	cStates::Create();

	D3DDesc desc;
	D3D::GetDesc(&desc);

	_globalVariable = make_shared<sGlobalVariable>();
	_globalVariable->ViewProjection = make_shared<cViewProjectionBuffer>();
	_globalVariable->Perspective = make_shared<cPerspective>(desc.Width, desc.Height);
	_globalVariable->Viewport = make_shared<cViewport>(desc.Width, desc.Height);
	_globalVariable->GlobalLight = make_shared<cLightBuffer>();
	_globalVariable->GuiSettings = make_shared<sGuiSettings>();
	_globalVariable->MainCamera = make_shared<cFreePointCamera>(100.0f);
	_globalVariable->MainCamera->SetPosition(0, 0, -5);
	_globalVariable->MainCamera->SetRotationDegree(0, 0);

	cDebug::SetDebugMode(true);
}

void cGameMain::Init()
{
	_vecObject.push_back(new cModels(_globalVariable));
	_vecObject.push_back(new cEnvironment());
	_vecObject.push_back(new cMenuBar(_globalVariable));
	for (auto& i : _vecObject)
		i->Init();
}

cGameMain::~cGameMain()
{
	for (auto& i : _vecObject)
		SAFE_DELETE(i);
}

void cGameMain::Update()
{
	_globalVariable->MainCamera->Update();
	D3DXVECTOR3 up{ 0, 1, 0 };
	float dot = D3DXVec3Dot(&_globalVariable->GlobalLight->Data.Direction, &up);
	float intensity = cMath::Clamp(dot * 1.5f, 0, 1);
	_globalVariable->GlobalLight->Data.Intensity = intensity;

	for (auto i : _vecObject)
		i->Update();

	cDebug::PrintLogs();
}

void cGameMain::PreRender()
{
	for (auto& i : _vecObject)
		i->PreRender();
}

void cGameMain::Render()
{
	D3DXMATRIX view, projection;
	_globalVariable->MainCamera->GetMatrix(&view);
	_globalVariable->Perspective->GetMatrix(&projection);

	_globalVariable->ViewProjection->SetView(view);
	_globalVariable->ViewProjection->SetProjection(projection);
	_globalVariable->ViewProjection->SetVSBuffer(0);

	_globalVariable->GlobalLight->SetPSBuffer(0);

	for (auto& i : _vecObject)
		i->Render();
}

void cGameMain::PostRender()
{
	for (auto& i : _vecObject)
		i->PostRender();
}

void cGameMain::ResizeScreen()
{
	for (auto& i : _vecObject)
		i->ResizeScreen();
}
