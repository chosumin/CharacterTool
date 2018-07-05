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
		void WriteMaterialData(Json::Value& root, wstring saveFolder, unique_ptr<FbxMaterial> material);

		//�ؽ��� ������ ��ο� ����
		//@param : ������ ���ϸ�
		//@param : ������ ����� ���
		void CopyTextureFile(string& textureFile, wstring saveFolder);
	private:
		/*******************
			�޽� ���� �Լ�
		********************/

		//�� Ʈ�� ��ȸ�ϸ� �� ���� ���� ��忡�� ������ �а� ���� ����
		//@param : �� ���
		//@param : �� ��ȣ
		//@param : �θ� �� ��ȣ
		void ReadBoneData(FbxNode* node, int index, int parent);

		//���� ������ �ִ� ��� Ÿ������ Ȯ��
		//@param : Ȯ���� ��� Ÿ��
		bool IsBoneType(FbxNodeAttribute::EType nodeType);

		//�� ������ ����
		//@param : �� �ε���
		//@param : �θ� �� �ε���
		//@param : �� �̸�
		//@param : �� ���� ���
		//@param : �� ���� ���
		unique_ptr<FbxBoneData> CreateBoneData(int index, int parent, string name, const D3DXMATRIX& transform, const D3DXMATRIX& absoluteTransform);

		//�� Ʈ�� ��ȸ�ϸ� �޽� ����� �޽� ������ �б�
		//@param : �� ���
		//@param : �θ� �� ��ȣ
		void ReadMeshData(FbxNode* node, int parentBone);

		//�������� ���� ����
		//@param : �������� �������� ���� �޽� ���
		//@param : ������ ��ȣ
		//@param : ���� ��ȣ(0~2, �ﰢ��)
		unique_ptr<FbxVertex> CreatePolygonVertex(FbxMesh *mesh, int p, int vi);

		//���� ������ �������� �޽� ����
		//@param : �޽� ���
		//@param : �θ� �޽� ��ȣ(Ʈ�� ����)
		//@param : �޽� ���� ���� ����
		unique_ptr<FbxMeshData> CreateMeshData(FbxNode *node, int parent, const vector<shared_ptr<FbxVertex>>& vertices);

		//��Ų ������ �б�
		void ReadSkinData();

		//��Ű���� ���� �� ����ġ ���Ϳ� ������ ���� ����
		//@param : �����Ӹ� �������� ���� �޽� ���
		//@param : ���Ե� ����
		//@param : ������ ��ȣ
		void PushBoneWeight(FbxMesh *mesh, OUT vector<struct FbxBoneWeights>& boneWeights, UINT i);

		//�޽��� �θ� �� ����
		void PushMeshParent(unique_ptr<FbxMeshData>& meshData, int i);

		//�� ��ȣ�� ��ȯ
		//@param : Ŭ�������� ���� �̸�
		UINT GetBoneIndexByName(string name);

		//�� ������ ����ġ, ����ġ �ε����� ����
		//@param : ���� �����̳�
		//@param : ����ġ ���� �����̳�
		void PushWeightInVertices(OUT vector<shared_ptr<FbxVertex>>& vertices, const vector<FbxBoneWeights>& boneWeights);

		//�޽� �����̳ʿ� ����� �޽� ��Ʈ ������ ����
		//@param : ���͸��� ��ȣ(���� ���͸����� ���� ���� �����)
		//@param : �޽� ���� ���� ����
		unique_ptr<FbxMeshPartData> CreateMeshPart(int i, const vector<shared_ptr<FbxVertex>>& vertices);

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
		//@param : ���� ������ ��ȣ
		//@param : ���� ������ ��ȣ
		void ReadAnimation(OUT FbxAnimation& animation, FbxNode *node, int start, int end);

		//Ű������ ����
		//@param : ���� Ʈ�������� �������� ���� �� ���
		//@param : ���� ������
		//@param : ���� ������
		unique_ptr<struct FbxKeyFrame> CreateKeyFrame(FbxNode * node, int start, int end);

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
		vector<unique_ptr<FbxMaterial>> _materials;
		vector<unique_ptr<FbxBoneData>> _bones;
		vector<unique_ptr<FbxMeshData>> _meshes;
		vector<unique_ptr<FbxAnimation>> _animations;
	};
}