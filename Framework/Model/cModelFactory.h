#pragma once

class cModelBone;
class cModelMesh;
class cModelMeshPart;
class cBinaryReader;
class cModel;
class cModelFactory
{
public:
	//Model ���丮
	//@param : �� ���� ���
	//@param : �𵨸�
	static unique_ptr<cModel> Create(wstring filePath, wstring fileName);

	//Model ���� ����
	//@param : ������ ��
	static unique_ptr<cModel> Clone(weak_ptr<cModel> model);
private:
	cModelFactory();
	~cModelFactory();
private:
	static void ReadMaterials(wstring file);
	static void ReadMesh(wstring file);
private:
	static void BindMeshData();

	//�ؽ��ĸ� �о����
	//@param : �ؽ��ĸ� ������ ���͸���
	//@param : �ؽ��� ����
	//@param : ���
	static void ReadTextureMap(class cMaterial *material, enum class TextureType type, string directory, string fileName);

	//�� ������ �ҷ�����
	static void ReadModelBoneData(weak_ptr<cBinaryReader> r, weak_ptr<cModelBone> bone);

	//�޽� ������ �ҷ�����
	static void ReadMeshData(weak_ptr<cBinaryReader> r, weak_ptr<cModelMesh> mesh);

	//�޽� ��Ʈ ������ �ҷ�����
	static void ReadMeshPartData(weak_ptr<cBinaryReader> r, weak_ptr<cModelMeshPart> meshPart);

	//�޽��� ���� ����
	static void LinkMeshToBone(weak_ptr<cModelMesh> mesh, int parentBoneIndex);
private:
	static unique_ptr<cModel> _model;
};