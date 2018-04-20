#pragma once

#include <Windows.h>
#include <assert.h>
#include <time.h>

#include <string>
#include <vector>
#include <functional>
#include <list>
#include <map>
using namespace std;

//DX11 헤더, 라이브러리
#include <dxgi1_2.h>
#include <d3dcommon.h>
#include <d3dcompiler.h>
#include <d3d11shader.h>
#include <d3d11.h>
#include <D3DX10math.h>
#include <D3DX11async.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

//ImGui 헤더 라이브러리
#include <imgui.h>
#include <imguiDx11.h>
#pragma comment(lib, "ImGui.lib")

#define SAFE_RELEASE(p){ if(p){ (p)->Release(); (p) = NULL; } }
#define SAFE_DELETE(p){ if(p){ delete (p); (p) = NULL; } }
#define SAFE_DELETE_ARRAY(p){ if(p){ delete [] (p); (p) = NULL; } }

const wstring Shader = L"../Shader/";

#include "./Common/iClass.h"
#include "./Common/cSingletone.h"

#include "./System/D3D.h"
#include "./System/cKeyboard.h"
#include "./System/cMouse.h"
#include "./System/cFrame.h"
