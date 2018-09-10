#pragma once

class cPath
{
public:
	//파일 열기
	//@param : 텍스트에 띄울 문자열(ex : "Open File")
	//@param : 찾을 확장명
	//@param : 시작 폴더 경로(시작시에 이 경로부터 표시됨)
	//@param : 실행할 함수
	static void OpenFileDialog(wstring file, const WCHAR* filter, wstring folder, function<void(wstring)> func);

	//다중 파일 열기
	//@param : 텍스트에 띄울 문자열(ex : "Open File")
	//@param : 찾을 확장명
	//@param : 시작 폴더 경로(시작시에 이 경로부터 표시됨)
	//@param : 실행할 함수
	static void OpenMultiFileDialog(wstring file, const WCHAR* filter, wstring folder, function<void(wstring)> func);
	
	//파일 저장
	//@param : 텍스트에 띄울 문자열(ex : "Save File")
	//@param : 저장할 확장명
	//@param : 시작 폴더 경로(시작시에 이 경로부터 표시됨)
	//@param : 실행할 함수
	static void SaveFileDialog(wstring file, const WCHAR* filter, wstring folder, function<void(wstring)> func);

	//해당 경로가 존재하는지 확인
	static bool ExistFile(string path);
	//해당 경로가 존재하는지 확인
	static bool ExistFile(wstring path);

	//해당 경로가 존재하는지 확인
	static bool ExistDirectory(string path);
	//해당 경로가 존재하는지 확인
	static bool ExistDirectory(wstring path);

	//파일명을 제외한 경로를 반환
	static string GetDirectoryName(string path);
	//파일명을 제외한 경로를 반환
	static wstring GetDirectoryName(wstring path);

	//확장자 반환
	static string GetExtension(string path);
	//확장자 반환
	static wstring GetExtension(wstring path);

	//파일명 반환(확장자 포함)
	static string GetFileName(string path);
	//파일명 반환(확장자 포함)
	static wstring GetFileName(wstring path);

	//확장자 제외한 파일명 반환
	static string GetFileNameWithoutExtension(string path);
	//확장자 제외한 파일명 반환
	static wstring GetFileNameWithoutExtension(wstring path);

	//해당 경로에 단일 경로 폴더 생성
	static void CreateFolder(string path);
	//해당 경로에 단일 경로 폴더 생성
	static void CreateFolder(wstring path);

	//해당 경로에 다중 경로 폴더 생성
	static void CreateFolders(string path);
	//해당 경로에 다중 경로 폴더 생성
	static void CreateFolders(wstring path);
public:
	static const WCHAR* ImageFilter;
	static const WCHAR* ShaderFilter;
	static const WCHAR* ModelFilter;
	static const WCHAR* MaterialFilter;
	static const WCHAR* MeshFilter;
	static const WCHAR* FbxFilter;
	static const WCHAR* AnimFbxFilter;
	static const WCHAR* BehaviorTreeFilter;
	static const WCHAR* TOP_UPPER_FOLDERNAME;
private:
	static BOOL IsThereOneFile(LPWSTR lpstrFilesWithPath);
	static void OpenFiles(LPWSTR lpstrFilesWithPath, function<void(wstring)> func);
};
