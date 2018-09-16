#pragma once

struct D3DDesc;
class cGameMain;
class cWindow
{
public:
	cWindow();
	virtual ~cWindow();
	void Run();
private:
	//���� �Լ�
	static LRESULT CALLBACK WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
private:
	void RegisterWindow(const IN D3DDesc& desc);
	void CreateWndEx(OUT D3DDesc& desc);
	void SetDisplay(const OUT D3DDesc& desc);
	void DeleteSingletone();

	void SetImGuiColors();
public:
	static UINT			m_uSingletoneCount;
private:
	static cGameMain*	m_pGameMain;
};

