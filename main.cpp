#include "stdafx.h"
#include "./System/cWindow.h"

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR param, int command)
{
	D3DDesc desc;
	desc.ProjectName = L"D3D Game";
	desc.Instance = instance;
	desc.FullScreen = false;
	desc.Vsync = true; //���� ����ȭ
	desc.Hwnd = NULL;
	desc.Width = 1280;
	desc.Height = 720;
	D3D::SetDesc(desc);

	auto pWindow = new cWindow();
	pWindow->Run();
	
	SAFE_DELETE(pWindow);

	return 0;
}