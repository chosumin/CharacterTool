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
	class Exporter
	{
	public:
		Exporter(wstring fbxFile);
		~Exporter();
	public:
		void ExportMaterial(wstring saveFolder, wstring saveName);
		void ExportMesh(wstring saveFolder, wstring saveName);
	private:
		void CopyTextureFile(string& textureFile, wstring saveFolder);

		void ReadMeshData(FbxNode* node, int index, int parent);
		void ReadJointData(FbxNode* node, int index, int parent);

		void CreateBoneFromMeshData();
		void CreateBoneFromJointData();

		//머터리얼 벡터에 정보 삽입
		void PushMaterials(int number);
		void PushBoneWeight(FbxMesh *mesh, OUT vector<struct FbxBoneWeights>& boneWeights, UINT i);

		void WriteMeshData(wstring saveFolder, wstring saveName);
		//Json파일에 머터리얼 정보 삽입
		void WriteMaterialDataOnJson(Json::Value& root, wstring saveFolder, unique_ptr<FbxMaterial> material);

		//삼각형 정점 생성
		void WriteBoneDataOnBinaryFile(cBinaryWriter *w);
		void WriteMeshDataOnBinaryFile(cBinaryWriter *w);

		unique_ptr<FbxVertex> CreatePolygonVertex(FbxMesh *mesh, int p, int vi, vector<struct FbxBoneWeights>& meshBoneWeights);
		unique_ptr<FbxMeshData> CreateMeshData(FbxNode *node, int parent, const vector<shared_ptr<FbxVertex>>& vertices);
		unique_ptr<FbxMeshPartData> CreateMeshPart(int i, const vector<shared_ptr<FbxVertex>>& vertices);

		UINT GetJointIndexByName(string name);
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
	};
}