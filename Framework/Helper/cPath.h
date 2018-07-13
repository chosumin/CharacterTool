#pragma once

class cPath
{
public:
	static void OpenFileDialog(wstring file, const WCHAR* filter, wstring folder, function<void(wstring)> func);
	static void SaveFileDialog(wstring file, const WCHAR* filter, wstring folder, function<void(wstring)> func);

	static bool ExistFile(string path);
	static bool ExistFile(wstring path);

	static bool ExistDirectory(string path);
	static bool ExistDirectory(wstring path);

	static string GetDirectoryName(string path);
	static wstring GetDirectoryName(wstring path);

	static string GetExtension(string path);
	static wstring GetExtension(wstring path);

	static string GetFileName(string path);
	static wstring GetFileName(wstring path);

	static string GetFileNameWithoutExtension(string path);
	static wstring GetFileNameWithoutExtension(wstring path);
public:
	static const WCHAR* ImageFilter;
	static const WCHAR* ShaderFilter;
	static const WCHAR* ModelFilter;
	static const WCHAR* MaterialFilter;
	static const WCHAR* MeshFilter;
	static const WCHAR* FbxFilter;
};
