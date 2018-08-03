#pragma once

#include <Windows.h>
#include <assert.h>
#include <time.h>

#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <forward_list>
#include <list>
#include <map>
#include <unordered_map>
#include <set>
#include <memory>
#include <algorithm>
#include <fstream>
#include <thread>
#include <typeinfo.h>
using namespace std;

#pragma comment(lib, "winmm.lib")

//디버그 콘솔
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console") 

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

//Json Cpp 라이브러리
#include <json/json.h>
#pragma comment(lib, "jsoncpp.lib")
#pragma warning( disable : 4996 )

//ImGui 헤더 라이브러리
#include <imgui.h>
#include <imguiDx11.h>
#pragma comment(lib, "ImGui.lib")

//Fbx SDK 라이브러리
#define FBXSDK_SHARED
#include <fbxsdk.h>
#pragma comment(lib, "libfbxsdk.lib")
using namespace fbxsdk;

//DirectX Tex 라이브러리
#include <DirectXTex.h>
#pragma comment(lib, "directxtex.lib")

#define SAFE_RELEASE(p){ if(p){ (p)->Release(); (p) = NULL; } }
#define SAFE_DELETE(p){ if(p){ delete (p); (p) = NULL; } }
#define SAFE_DELETE_ARRAY(p){ if(p){ delete [] (p); (p) = NULL; } }

const wstring Shader = L"../Shader/";
const wstring Asset = L"../Asset/";
const wstring Content = L"../Content/";
const wstring Model = Content + L"Model/";

#include "./Common/iExecutable.h"
#include "./Common/cSingletone.h"
#include "./Common/GlobalData.h"
#include "./Common/inheritable_enable_shared_from_this.h"

#include "./Command/cKeyboard.h"
#include "./Command/cMouse.h"

#include "./System/cDebug.h"
#include "./System/D3D.h"
#include "./System/cFrame.h"

#include "./Helper/cBinary.h"
#include "./Helper/Json.h"

#include "./Viewer/cPerspective.h"
#include "./Viewer/cViewport.h"

#include "./Graphic/cStates.h"
#include "./Graphic/VertexLayouts.h"
#include "./Graphic/cCBuffer.h"
#include "./Graphic/GlobalBuffers.h"
#include "./Graphic/cShader.h"

#include "./Mesh/MeshUtility.h"

#include "./Collider/cColliderFactory.h"
//#include "./Component/Transform/sTransform.h"

using ModelVertexType = VertexTNB;

const float PI = 3.1415926f;
const float RADIAN = (PI / 180.0f);