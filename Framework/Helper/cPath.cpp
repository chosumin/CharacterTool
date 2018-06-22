#include "stdafx.h"
#include "cPath.h"
#include "cString.h"

const WCHAR* cPath::ImageFilter = L"Image\0*.png;*.bmp;*.jpg";
const WCHAR* cPath::ModelFilter = L"Model\0*.model";
const WCHAR* cPath::ShaderFilter = L"HLSL file\0*.hlsl";

void cPath::OpenFileDialog(wstring file, const WCHAR* filter, wstring folder, function<void(wstring)> func)
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
