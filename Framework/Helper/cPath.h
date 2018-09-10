#pragma once

class cPath
{
public:
	//���� ����
	//@param : �ؽ�Ʈ�� ��� ���ڿ�(ex : "Open File")
	//@param : ã�� Ȯ���
	//@param : ���� ���� ���(���۽ÿ� �� ��κ��� ǥ�õ�)
	//@param : ������ �Լ�
	static void OpenFileDialog(wstring file, const WCHAR* filter, wstring folder, function<void(wstring)> func);

	//���� ���� ����
	//@param : �ؽ�Ʈ�� ��� ���ڿ�(ex : "Open File")
	//@param : ã�� Ȯ���
	//@param : ���� ���� ���(���۽ÿ� �� ��κ��� ǥ�õ�)
	//@param : ������ �Լ�
	static void OpenMultiFileDialog(wstring file, const WCHAR* filter, wstring folder, function<void(wstring)> func);
	
	//���� ����
	//@param : �ؽ�Ʈ�� ��� ���ڿ�(ex : "Save File")
	//@param : ������ Ȯ���
	//@param : ���� ���� ���(���۽ÿ� �� ��κ��� ǥ�õ�)
	//@param : ������ �Լ�
	static void SaveFileDialog(wstring file, const WCHAR* filter, wstring folder, function<void(wstring)> func);

	//�ش� ��ΰ� �����ϴ��� Ȯ��
	static bool ExistFile(string path);
	//�ش� ��ΰ� �����ϴ��� Ȯ��
	static bool ExistFile(wstring path);

	//�ش� ��ΰ� �����ϴ��� Ȯ��
	static bool ExistDirectory(string path);
	//�ش� ��ΰ� �����ϴ��� Ȯ��
	static bool ExistDirectory(wstring path);

	//���ϸ��� ������ ��θ� ��ȯ
	static string GetDirectoryName(string path);
	//���ϸ��� ������ ��θ� ��ȯ
	static wstring GetDirectoryName(wstring path);

	//Ȯ���� ��ȯ
	static string GetExtension(string path);
	//Ȯ���� ��ȯ
	static wstring GetExtension(wstring path);

	//���ϸ� ��ȯ(Ȯ���� ����)
	static string GetFileName(string path);
	//���ϸ� ��ȯ(Ȯ���� ����)
	static wstring GetFileName(wstring path);

	//Ȯ���� ������ ���ϸ� ��ȯ
	static string GetFileNameWithoutExtension(string path);
	//Ȯ���� ������ ���ϸ� ��ȯ
	static wstring GetFileNameWithoutExtension(wstring path);

	//�ش� ��ο� ���� ��� ���� ����
	static void CreateFolder(string path);
	//�ش� ��ο� ���� ��� ���� ����
	static void CreateFolder(wstring path);

	//�ش� ��ο� ���� ��� ���� ����
	static void CreateFolders(string path);
	//�ش� ��ο� ���� ��� ���� ����
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
