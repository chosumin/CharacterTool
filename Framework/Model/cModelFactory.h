#pragma once

class cModelBone;
class cModelMesh;
class cModelMeshPart;
class cAnimation;
class cBinaryReader;
class cModel;
class cModelFactory : public cSingletone<cModelFactory>
{
private:
	friend class cSingletone<cModelFactory>;
public:
	shared_ptr<cModel> Create(wstring file);
	shared_ptr<cModel> Clone(weak_ptr<cModel> model);
private:
	cModelFactory();
	~cModelFactory();
private:
	void ReadMaterials(wstring file);
	void ReadMesh(wstring file);
	void ReadAnimation(wstring file);
private:
	void BindMeshData();

	//�ؽ��ĸ� �о����
	//@param : �ؽ��ĸ� ������ ���͸���
	//@param : �ؽ��� ����
	//@param : ���
	void ReadTextureMap(class cMaterial *material, enum class TextureType type, string directory, string fileName);

	//�� ������ �ҷ�����
	void ReadModelBoneData(weak_ptr<cBinaryReader> r, weak_ptr<cModelBone> bone);

	//�޽� ������ �ҷ�����
	void ReadMeshData(weak_ptr<cBinaryReader> r, weak_ptr<cModelMesh> mesh);

	//�޽� ��Ʈ ������ �ҷ�����
	void ReadMeshPartData(weak_ptr<cBinaryReader> r, weak_ptr<cModelMeshPart> meshPart);

	//�޽��� ���� ����
	void LinkMeshToBone(weak_ptr<cModelMesh> mesh, int parentBoneIndex);
private:
	//todo : ���͸���, �޽�, �ִϸ��̼� ĳ�� �����
	unique_ptr<cModel> _model;
};