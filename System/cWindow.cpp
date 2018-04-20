#include "stdafx.h"
#include "cWindow.h"
#include "cGameMain.h"

cGameMain* cWindow::m_pGameMain = nullptr;
UINT cWindow::m_uSingletoneCount = 0;

LRESULT CALLBACK cWindow::WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	cMouse::Get()->WndProc(message, wParam, lParam);

	if (ImGui::WndProc((UINT*)handle, message, wParam, lParam))
		return true;

	if (message == WM_SIZE)
	{
		ImGui::Invalidate();
		{
			if (m_pGameMain != NULL)
			{
				float width = (float)LOWORD(lParam);
				float height = (float)HIWORD(lParam);

				if (D3D::Get() != NULL)
					D3D::Get()->ResizeScreen(width, height);

				m_pGameMain->ResizeScreen();
			}
		}
		ImGui::Validate();
	}

	if (message == WM_CLOSE || message == WM_DESTROY)
	{
		PostQuitMessage(0);

		return 0;
	}

	return DefWindowProc(handle, message, wParam, lParam);
}

cWindow::cWindow()
{
	//���̷�Ʈ ���� �ҷ�����
	D3DDesc desc;
	D3D::GetDesc(&desc);

	RegisterWindow(desc);
	CreateWndEx(desc);
	SetDisplay(desc);
}

cWindow::~cWindow()
{
	SAFE_DELETE(m_pGameMain);
	DeleteSingletone();

	D3DDesc desc;
	D3D::GetDesc(&desc);

	if (desc.FullScreen == true)
		ChangeDisplaySettings(NULL, 0);

	DestroyWindow(desc.Hwnd);
	UnregisterClass(desc.ProjectName.c_str(), desc.Instance);
}

void cWindow::Run()
{
	//���̷�Ʈ ����̽� ����
	D3DDesc desc;
	D3D::GetDesc(&desc);

	//TODO : ���� �ߵƳ� ã�ƺ���
	D3D::Get();

	//ImGui ����
	ImGui::Create(desc.Hwnd, D3D::GetDevice(), D3D::GetDC());
	ImGui::StyleColorsDark();

	//��ü �ʱ�ȭ
	m_pGameMain = new cGameMain();
	m_pGameMain->Init();

	MSG msg = { 0 };
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//������ ���
			cFrame::Get()->Update(); 

			//ImGui�� ���콺 ���� ��
			if(!ImGui::IsMouseHoveringAnyWindow())
			{
				//���콺, Ű���� �Է�
				cMouse::Get()->Update();
				cKeyboard::Get()->Update();
			}

			m_pGameMain->Update();
			ImGui::Update();

			m_pGameMain->PreRender();
			D3D::Get()->Clear();
			{
				m_pGameMain->Render();
				m_pGameMain->PostRender(); //ImGui ���õ� �ڵ�
			}
			D3D::Get()->Present();
		}
	}
}

void cWindow::RegisterWindow(const IN D3DDesc & desc)
{
	//������ ��� EX
	WNDCLASSEX wndClass;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wndClass.hIconSm = wndClass.hIcon;
	wndClass.hInstance = desc.Instance;
	wndClass.lpfnWndProc = (WNDPROC)WndProc;
	wndClass.lpszClassName = desc.ProjectName.c_str();
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndClass.cbSize = sizeof(WNDCLASSEX);

	WORD success = RegisterClassEx(&wndClass);
	assert(success && "�����찡 ��ϵ��� ���߽��ϴ�!");
}

void cWindow::CreateWndEx(OUT D3DDesc & desc)
{
	desc.Hwnd = CreateWindowEx
	(
		WS_EX_APPWINDOW
		, desc.ProjectName.c_str()
		, desc.ProjectName.c_str()
		, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW
		, CW_USEDEFAULT
		, CW_USEDEFAULT
		, CW_USEDEFAULT
		, CW_USEDEFAULT
		, NULL
		, (HMENU)NULL
		, desc.Instance
		, NULL
	);

	assert(desc.Hwnd != NULL);
	D3D::SetDesc(desc);
}

void cWindow::SetDisplay(const OUT D3DDesc & desc)
{
	if (desc.FullScreen)
	{
		DEVMODE devMode = { 0 };
		devMode.dmSize = sizeof(DEVMODE);
		devMode.dmPelsWidth = (DWORD)desc.Width;
		devMode.dmPelsHeight = (DWORD)desc.Height;
		devMode.dmBitsPerPel = 32;
		devMode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&devMode, CDS_FULLSCREEN);
	}

	RECT rect = { 0, 0, (LONG)desc.Width, (LONG)desc.Height };

	//ȭ���� �ݿ� ��ġ��Ŵ
	UINT centerX = (GetSystemMetrics(SM_CXSCREEN) - (UINT)desc.Width) / 2;
	UINT centerY = (GetSystemMetrics(SM_CYSCREEN) - (UINT)desc.Height) / 2;

	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
	MoveWindow
	(
		desc.Hwnd
		, centerX, centerY
		, rect.right - rect.left, rect.bottom - rect.top
		, TRUE
	);
	ShowWindow(desc.Hwnd, SW_SHOWNORMAL);
	SetForegroundWindow(desc.Hwnd);
	SetFocus(desc.Hwnd);

	ShowCursor(true);
}

void cWindow::DeleteSingletone()
{
	ImGui::Delete();
	cFrame::Delete();
	cKeyboard::Delete();
	cMouse::Delete();
	D3D::Delete();

	assert(m_uSingletoneCount <= 0 && "���� �ȵ� �̱��� ��ü�� �ֽ��ϴ�!");
}
