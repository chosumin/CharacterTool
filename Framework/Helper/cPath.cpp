#include "stdafx.h"
#include "cPath.h"
#include "cString.h"

const WCHAR* cPath::ImageFilter = L"Image\0*.png;*.bmp;*.jpg";
const WCHAR* cPath::ShaderFilter = L"HLSL file\0*.hlsl";
const WCHAR* cPath::ModelFilter = L"Model\0*.material;*.fbx";
const WCHAR* cPath::MaterialFilter = L"Model\0*.material";
const WCHAR* cPath::MeshFilter = L"Model\0*.mesh";
const WCHAR* cPath::FbxFilter = L"Model\0*.fbx";
const WCHAR* cPath::AnimFbxFilter = L"Animation\0*.anim;*.fbx";
const WCHAR* cPath::BehaviorTreeFilter= L"BehaviorTree\0*.bt";
const WCHAR* cPath::TOP_FOLDERNAME_WCHAR = L"/DX11_3D";
const CHAR* cPath::TOP_FOLDERNAME_CHAR = "/DX11_3D";

void cPath::OpenFileDialog(wstring file, const WCHAR* filter, wstring folder, function<void(wstring)> func)
{
	WCHAR name[255];
	wcscpy_s(name, file.c_str());

	wstring tempFolder = folder;
	cString::Replace(&tempFolder, L"/", L"\\");

	OPENFILENAME ofn;
	memset(&ofn, 0, sizeof(OPENFILENAME));
	//ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = name;
	ofn.lpstrFileTitle = L"파일 불러오기";
	ofn.nMaxFile = 255;
	ofn.lpstrInitialDir = tempFolder.c_str();
	ofn.Flags = OFN_NOCHANGEDIR;

	if (GetOpenFileName(&ofn) == TRUE)
	{
		if (func != nullptr)
		{
			wstring loadName = name;
			cString::Replace(&loadName, L"\\", L"/");

			func(loadName);
		}
	}
}

void cPath::OpenMultiFileDialog(wstring file, const WCHAR * filter, wstring folder, function<void(wstring)> func)
{
	WCHAR name[512];
	wcscpy_s(name, file.c_str());

	wstring tempFolder = folder;
	cString::Replace(&tempFolder, L"/", L"\\");

	OPENFILENAME ofn;
	memset(&ofn, 0, sizeof(OPENFILENAME));
	//ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = name;
	ofn.lpstrFileTitle = L"파일 불러오기";
	ofn.nMaxFile = 512;
	ofn.lpstrInitialDir = tempFolder.c_str();
	ofn.Flags = OFN_EXPLORER | OFN_ALLOWMULTISELECT;

	if (GetOpenFileName(&ofn) != 0)
	{
		if (func != nullptr)
			OpenFiles(name, func);
	}
}

void cPath::OpenFiles(LPWSTR lpstrFilesWithPath, function<void(wstring)> func)
{
	UINT        i;
	UINT        nInsertedItems = 0;
	const UINT    nMaxLen = 512;
	WCHAR        szPath[nMaxLen];
	WCHAR        szFile[nMaxLen];

	// 파일의 경로를 szPath에 복사
	wcscpy(szPath, lpstrFilesWithPath);
	if (NULL == szPath[0])
		return;

	// 선택된 파일이 1개인 경우
	if (IsThereOneFile(lpstrFilesWithPath))
	{
		wstring loadName = lpstrFilesWithPath;
		cString::Replace(&loadName, L"\\", L"/");
		func(loadName);

		return;
	}

	// 선택된 파일이 2개 이상인 경우
	for (i = wcslen(lpstrFilesWithPath) + 1; NULL != lpstrFilesWithPath[i]; i += (wcslen(lpstrFilesWithPath + i) + 1))
	{
		wcscpy(szFile, lpstrFilesWithPath + i);
		wstring fileName = szFile;
		wstring loadName = lpstrFilesWithPath;
		loadName += L"/" + fileName;

		cString::Replace(&loadName, L"\\", L"/");
		func(loadName);
	}
}

void cPath::SaveFileDialog(wstring file, const WCHAR * filter, wstring folder, function<void(wstring)> func)
{
	WCHAR name[255];
	wcscpy_s(name, file.c_str());

	wstring tempFolder = folder;
	cString::Replace(&tempFolder, L"/", L"\\");

	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = name;
	ofn.lpstrFileTitle = L"저장하기";
	ofn.nMaxFile = 255;
	ofn.lpstrInitialDir = tempFolder.c_str();
	ofn.Flags = OFN_NOCHANGEDIR;

	if (GetSaveFileName(&ofn) == TRUE)
	{
		if (func != nullptr)
		{
			wstring loadName = name;
			cString::Replace(&loadName, L"\\", L"/");

			func(loadName);
		}
	}
}

/************************
	유효한 파일인지 확인
*************************/
bool cPath::ExistFile(string path)
{
	return ExistFile(cString::Wstring(path));
}

bool cPath::ExistFile(wstring path)
{
	DWORD fileValue = GetFileAttributes(path.c_str());

	return fileValue < 0xFFFFFFFF;
}

/*************************
	유효한 경로인지 확인
**************************/
bool cPath::ExistDirectory(string path)
{
	return ExistDirectory(cString::Wstring(path));
}

bool cPath::ExistDirectory(wstring path)
{
	DWORD attribute = GetFileAttributes(path.c_str());

	BOOL temp = (attribute != INVALID_FILE_ATTRIBUTES &&
		(attribute & FILE_ATTRIBUTE_DIRECTORY));

	return temp == TRUE;
}

string cPath::GetDirectoryName(string path)
{
	cString::Replace(&path, "\\", "/");
	size_t index = path.find_last_of('/');

	return path.substr(0, index + 1);
}

wstring cPath::GetDirectoryName(wstring path)
{
	cString::Replace(&path, L"\\", L"/");
	size_t index = path.find_last_of('/');

	return path.substr(0, index + 1);
}

string cPath::GetRelativePath(string path, string startString)
{
	return cString::String(GetRelativePath(cString::Wstring(path), cString::Wstring(startString)));
}

wstring cPath::GetRelativePath(wstring path, wstring startString)
{
	size_t index = path.find(startString);

	size_t maxVal = -1;
	if (index == maxVal)
		return path;

	size_t startStrSize = startString.size();
	path = path.substr(index);
	path.erase(0, startStrSize);
	path.insert(0, L"..");

	return path;
}

string cPath::GetExtension(string path)
{
	cString::Replace(&path, "\\", "/");
	size_t index = path.find_last_of('.');

	return path.substr(index + 1, path.length());
}

wstring cPath::GetExtension(wstring path)
{
	cString::Replace(&path, L"\\", L"/");
	size_t index = path.find_last_of('.');

	return path.substr(index + 1, path.length());
}

string cPath::GetFileName(string path)
{
	cString::Replace(&path, "\\", "/");
	size_t index = path.find_last_of('/');

	return path.substr(index + 1, path.length());
}

wstring cPath::GetFileName(wstring path)
{
	cString::Replace(&path, L"\\", L"/");
	size_t index = path.find_last_of('/');

	return path.substr(index + 1, path.length());
}

string cPath::GetFileNameWithoutExtension(string path)
{
	string fileName = GetFileName(path);

	size_t index = fileName.find_last_of('.');
	return fileName.substr(0, index);
}

wstring cPath::GetFileNameWithoutExtension(wstring path)
{
	wstring fileName = GetFileName(path);

	size_t index = fileName.find_last_of('.');
	return fileName.substr(0, index);
}

void cPath::CreateFolder(string path)
{
	CreateFolder(cString::Wstring(path));
}

void cPath::CreateFolder(wstring path)
{
	if (ExistDirectory(path) == false)
		CreateDirectory(path.c_str(), nullptr);
}

void cPath::CreateFolders(string path)
{
	CreateFolders(cString::Wstring(path));
}

void cPath::CreateFolders(wstring path)
{
	cString::Replace(&path, L"\\", L"/");

	vector<wstring> folders;
	cString::Split(&folders, path, L"/");

	wstring temp = L"";
	for (auto& folder : folders)
	{
		temp += folder + L"/";
		if (ExistDirectory(temp) == false)
			CreateFolder(temp);
	}
}

BOOL cPath::IsThereOneFile(LPWSTR lpstrFilesWithPath)
{
	UINT i = wcslen(lpstrFilesWithPath) + 1;
	return (lpstrFilesWithPath[i] == NULL) ? TRUE : FALSE;
}