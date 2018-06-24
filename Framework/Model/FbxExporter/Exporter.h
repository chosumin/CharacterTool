#pragma once

class cModel;
class cBinaryWriter;
namespace Fbx
{
	struct FbxMaterial;
	struct FbxVertex;
	struct FbxMeshData;
	struct FbxMeshPartData;
	struct FbxJoint;
	struct FbxBoneData;
	struct FbxAnimation;
	class Exporter
	{
	public:
		Exporter(wstring fbxFile);
		~Exporter();
	public:
		void ExportMaterial(wstring saveFolder, wstring saveName);
		void ExportMesh(wstring saveFolder, wstring saveName);
		void ExportAnimation(wstring saveFolder, wstring saveName);
	private:
		/**********************
			���͸��� ���� �Լ�
		***********************/

		//���͸��� ���Ϳ� ���� ����
		//@param : ���͸��� ��ȣ
		void PushMaterials(int number);

		//Json ���Ͽ� ���͸��� ���� �Է�
		//@param : Json��Ʈ
		//@param : ������
		//@param : �Էµ� ���͸���
		void WriteMaterialDataOnJson(Json::Value& root, wstring saveFolder, unique_ptr<FbxMaterial> material);

		//�ؽ��� ������ ��ο� ����
		//@param : ������ ���ϸ�
		//@param : ������ ����� ���
		void CopyTextureFile(string& textureFile, wstring saveFolder);
	private:
		/*******************
			�޽� ���� �Լ�
		********************/

		//�� Ʈ�� ��ȸ�ϸ� ���̷��� ����� ����Ʈ ������ �а� ���� ����
		//@param : �� ���
		//@param : ����Ʈ ��ȣ
		//@param : �θ� ����Ʈ ��ȣ
		void ReadJointData(FbxNode* node, int index, int parent);

		//�� Ʈ�� ��ȸ�ϸ� �޽� ����� �޽� ������ �б�
		//@param : �� ���
		//@param : �޽� ��ȣ
		//@param : �θ� �޽� ��ȣ
		void ReadMeshData(FbxNode* node, int index, int parent);

		//��Ű���� ���� �� ����ġ ���Ϳ� ������ ���� ����
		//@param : �����Ӹ� �������� ���� �޽� ���
		//@param : ���Ե� ����
		//@param : ������ ��ȣ
		void PushBoneWeight(FbxMesh *mesh, OUT vector<struct FbxBoneWeights>& boneWeights, UINT i);

		//����Ʈ ��ȣ�� ��ȯ
		//@param : Ŭ�������� ���� �̸�
		UINT GetJointIndexByName(string name);

		//�������� ���� ����
		//@param : �������� �������� ���� �޽� ���
		//@param : ������ ��ȣ
		//@param : ���� ��ȣ(0~2, �ﰢ��)
		//@param : ���� ���� ������ ���� �� ����ġ ����
		unique_ptr<FbxVertex> CreatePolygonVertex(FbxMesh *mesh, int p, int vi, vector<struct FbxBoneWeights>& meshBoneWeights);

		//���� ������ �������� �޽� ����
		//@param : �޽� ���
		//@param : �θ� �޽� ��ȣ(Ʈ�� ����)
		//@param : �޽� ���� ���� ����
		unique_ptr<FbxMeshData> CreateMeshData(FbxNode *node, int parent, const vector<shared_ptr<FbxVertex>>& vertices);

		//�޽� �����̳ʿ� ����� �޽� ��Ʈ ������ ����
		//@param : ���͸��� ��ȣ(���� ���͸����� ���� ���� �����)
		//@param : �޽� ���� ���� ����
		unique_ptr<FbxMeshPartData> CreateMeshPart(int i, const vector<shared_ptr<FbxVertex>>& vertices);

		//���̷��� ������ ������ ���� �� ���� ����
		void CreateBoneFromMeshData();

		//����Ʈ Ʈ��, �޽� ���͸� ������ �� Ʈ�� ���� ����
		void CreateBoneFromJointData();

		//�� ������ ����
		//@param : �� �ε���
		//@param : �θ� �� �ε���
		//@param : �� �̸�
		//@param : �� ���� ���
		//@param : �� ���� ���
		unique_ptr<FbxBoneData> CreateBoneData(int index, int parent, string name, const D3DXMATRIX& transform, const D3DXMATRIX& absoluteTransform);

		//���̳ʸ� ���� ���� �� ����
		//@param : ���� ����
		//@param : ���ϸ�
		void WriteMeshData(wstring saveFolder, wstring saveName);
		
		//���� �ȿ� �� Ʈ�� ���� ���� �� ����
		void WriteBoneDataOnBinaryFile(cBinaryWriter *w);

		//���� �ȿ� �޽� ���� ���� �� ����
		void WriteMeshDataOnBinaryFile(cBinaryWriter *w);
	private:
		/************************
			�ִϸ��̼� ���� �Լ�
		*************************/
		
		//�ִϸ��̼� ���� �б�
		//@param : �� �������� �ð�
		void ReadAnimation(float frameRate);

		//�ִϸ��̼� ���� �б� �� ����
		//@param : ���� ���Ե� �ִϸ��̼�
		//@param : ���� �� �� ���
		//@param : ������ �ð�
		//@param : ���� �ð�
		//@param : ���� �ð�
		void ReadAnimation(OUT FbxAnimation& animation, FbxNode *node, float frameRate, float start, float end);

		//Ű������ ����
		//@param : ���� Ʈ�������� �������� ���� �� ���
		//@param : ������ ���� �ð�
		//@param : �� �����Ӵ� �ð�
		unique_ptr<struct FbxKeyFrame> CreateKeyFrame(FbxNode * node, float end, float frameRate);

		//Ű�����ӵ����� ����
		//@param : �ִϸ��̼� �ð��� �������� ���� �� ���
		//@param : ���� Ʈ������
		//@param : �ִϸ��̼� �ð�
		struct FbxKeyFrameData CreateKeyFrameData(FbxNode * node, const D3DXMATRIX& transform, FbxTime animationTime);

		//���̳ʸ� ���� ���� �� ����
		//@param : ���� ����
		//@param : ���ϸ�
		void WriteAnimation(wstring savaFolder, wstring saveName);
	private:
		FbxManager *_manager;
		FbxImporter *_importer;
		FbxScene *_scene;
		FbxIOSettings *_ios;
		FbxGeometryConverter *_converter;
	private:
		vector<unique_ptr<FbxJoint>> _joints;
		vector<unique_ptr<FbxMaterial>> _materials;
		vector<unique_ptr<FbxBoneData>> _bones;
		vector<unique_ptr<FbxMeshData>> _meshes;
		vector<unique_ptr<FbxAnimation>> _animations;
	};
}