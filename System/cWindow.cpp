#include "stdafx.h"
#include "cWindow.h"
#include "cGameMain.h"
#include "./Command/cKeyboard.h"

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
	//다이렉트 정보 불러오기
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
	//다이렉트 디바이스 생성
	D3DDesc desc;
	D3D::GetDesc(&desc);

	D3D::Get();

	cFrame::Get()->Start();

	//ImGui 생성
	ImGui::Create(desc.Hwnd, D3D::GetDevice(), D3D::GetDC());
	ImGui::StyleColorsClassic();
	SetImGuiColors();

	//객체 초기화
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
			//프레임 계산
			cFrame::Get()->Update(); 

			//ImGui에 마우스 없을 때
			if(!ImGui::GetIO().WantCaptureMouse)
			{
				//마우스, 키보드 입력
				cMouse::Get()->Update();
				cKeyboard::Get()->Update();
			}
			else
			{
				cMouse::Get()->SetMouseValue(D3DXVECTOR3{ 0,0,0 });
			}

			m_pGameMain->Update();
			ImGui::Update();

			m_pGameMain->PreRender();
			D3D::Get()->Clear(0xFF0000cd);
			{
				m_pGameMain->Render();
				m_pGameMain->PostRender(); //ImGui 관련된 코드
				ImGui::Render();
			}
			D3D::Get()->Present();
		}
	}
}

void cWindow::RegisterWindow(const IN D3DDesc & desc)
{
	//개발자 모드 EX
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
	assert(success && "윈도우가 등록되지 못했습니다!");
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

	//화면의 반에 위치시킴
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
	cStates::Delete();

	assert(m_uSingletoneCount <= 0 && "삭제 안된 싱글턴 객체가 있습니다!");
}

void cWindow::SetImGuiColors()
{
	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.57f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.11f, 0.11f, 0.14f, 0.92f);
	colors[ImGuiCol_Border] = ImVec4(0.50f, 0.50f, 0.50f, 0.50f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.81f, 0.51f, 0.00f, 0.29f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.77f, 0.51f, 0.00f, 0.62f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(1.00f, 0.53f, 0.00f, 0.69f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.72f, 0.39f, 0.00f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.79f, 0.48f, 0.00f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.90f, 0.58f, 0.00f, 1.00f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.79f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.40f, 0.40f, 0.41f, 0.51f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.54f, 0.54f, 0.54f, 0.69f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.64f, 0.66f, 0.66f, 0.68f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 0.50f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.47f, 0.50f, 0.45f, 0.30f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.65f, 0.65f, 0.61f, 0.60f);
	colors[ImGuiCol_Button] = ImVec4(1.00f, 0.58f, 0.00f, 0.22f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(1.00f, 0.58f, 0.00f, 0.48f);
	colors[ImGuiCol_ButtonActive] = ImVec4(1.00f, 0.57f, 0.00f, 0.77f);
	colors[ImGuiCol_Header] = ImVec4(1.00f, 0.59f, 0.00f, 0.59f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(1.00f, 0.59f, 0.00f, 0.78f);
	colors[ImGuiCol_HeaderActive] = ImVec4(1.00f, 0.60f, 0.00f, 0.93f);
	colors[ImGuiCol_Separator] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.60f, 0.60f, 0.59f, 1.00f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.71f, 0.71f, 0.71f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.16f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.78f, 0.82f, 1.00f, 0.60f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.78f, 0.82f, 1.00f, 0.90f);
	colors[ImGuiCol_CloseButton] = ImVec4(1.00f, 0.59f, 0.00f, 0.35f);
	colors[ImGuiCol_CloseButtonHovered] = ImVec4(1.00f, 0.58f, 0.00f, 0.52f);
	colors[ImGuiCol_CloseButtonActive] = ImVec4(1.00f, 0.59f, 0.00f, 0.67f);
	colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
	colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(1.00f, 0.51f, 0.00f, 0.67f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);


}
