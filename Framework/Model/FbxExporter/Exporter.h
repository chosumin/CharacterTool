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
			머터리얼 관련 함수
		***********************/

		//머터리얼 벡터에 정보 삽입
		//@param : 머터리얼 번호
		void PushMaterials(int number);

		//Json 파일에 머터리얼 정보 입력
		//@param : Json루트
		//@param : 폴더명
		//@param : 입력될 머터리얼
		void WriteMaterialDataOnJson(Json::Value& root, wstring saveFolder, unique_ptr<FbxMaterial> material);

		//텍스쳐 파일을 경로에 복사
		//@param : 복사할 파일명
		//@param : 파일이 저장될 경로
		void CopyTextureFile(string& textureFile, wstring saveFolder);
	private:
		/*******************
			메쉬 관련 함수
		********************/

		//씬 트리 순회하며 스켈레톤 노드의 조인트 데이터 읽고 계층 생성
		//@param : 씬 노드
		//@param : 조인트 번호
		//@param : 부모 조인트 번호
		void ReadJointData(FbxNode* node, int index, int parent);

		//씬 트리 순회하며 메쉬 노드의 메쉬 데이터 읽기
		//@param : 씬 노드
		//@param : 메쉬 번호
		//@param : 부모 메쉬 번호
		void ReadMeshData(FbxNode* node, int index, int parent);

		//스키닝을 위한 본 가중치 벡터에 가충지 정보 삽입
		//@param : 디포머를 가져오기 위한 메쉬 노드
		//@param : 삽입될 벡터
		//@param : 디포머 번호
		void PushBoneWeight(FbxMesh *mesh, OUT vector<struct FbxBoneWeights>& boneWeights, UINT i);

		//조인트 번호를 반환
		//@param : 클러스터의 연결 이름
		UINT GetJointIndexByName(string name);

		//폴리곤의 정점 생성
		//@param : 폴리곤을 가져오기 위한 메쉬 노드
		//@param : 폴리곤 번호
		//@param : 정점 번호(0~2, 삼각형)
		//@param : 블렌드 정보 삽입을 위한 본 가중치 벡터
		unique_ptr<FbxVertex> CreatePolygonVertex(FbxMesh *mesh, int p, int vi, vector<struct FbxBoneWeights>& meshBoneWeights);

		//정점 정보를 바탕으로 메쉬 생성
		//@param : 메쉬 노드
		//@param : 부모 메쉬 번호(트리 구조)
		//@param : 메쉬 정점 정보 벡터
		unique_ptr<FbxMeshData> CreateMeshData(FbxNode *node, int parent, const vector<shared_ptr<FbxVertex>>& vertices);

		//메쉬 컨테이너에 저장될 메쉬 파트 데이터 생성
		//@param : 머터리얼 번호(공통 머터리얼을 가진 정점 묶기용)
		//@param : 메쉬 정점 정보 벡터
		unique_ptr<FbxMeshPartData> CreateMeshPart(int i, const vector<shared_ptr<FbxVertex>>& vertices);

		//스켈레톤 정보가 없으면 직접 본 정보 생성
		void CreateBoneFromMeshData();

		//조인트 트리, 메쉬 벡터를 가지는 본 트리 정보 생성
		void CreateBoneFromJointData();

		//본 데이터 생성
		//@param : 본 인덱스
		//@param : 부모 본 인덱스
		//@param : 본 이름
		//@param : 본 로컬 행렬
		//@param : 본 월드 행렬
		unique_ptr<FbxBoneData> CreateBoneData(int index, int parent, string name, const D3DXMATRIX& transform, const D3DXMATRIX& absoluteTransform);

		//바이너리 파일 쓰기 및 저장
		//@param : 저장 폴더
		//@param : 파일명
		void WriteMeshData(wstring saveFolder, wstring saveName);
		
		//파일 안에 본 트리 정보 쓰기 및 저장
		void WriteBoneDataOnBinaryFile(cBinaryWriter *w);

		//파일 안에 메쉬 정보 쓰기 및 저장
		void WriteMeshDataOnBinaryFile(cBinaryWriter *w);
	private:
		/************************
			애니메이션 관련 함수
		*************************/
		
		//애니메이션 정보 읽기
		//@param : 한 프레임의 시간
		void ReadAnimation(float frameRate);

		//애니메이션 정보 읽기 및 삽입
		//@param : 정보 삽입될 애니메이션
		//@param : 정보 얻어낼 씬 노드
		//@param : 프레임 시간
		//@param : 시작 시간
		//@param : 종료 시간
		void ReadAnimation(OUT FbxAnimation& animation, FbxNode *node, float frameRate, float start, float end);

		//키프레임 생성
		//@param : 로컬 트랜스폼을 가져오기 위한 씬 노드
		//@param : 프레임 종료 시간
		//@param : 한 프레임당 시간
		unique_ptr<struct FbxKeyFrame> CreateKeyFrame(FbxNode * node, float end, float frameRate);

		//키프레임데이터 생성
		//@param : 애니메이션 시간을 가져오기 위한 씬 노드
		//@param : 로컬 트랜스폼
		//@param : 애니메이션 시간
		struct FbxKeyFrameData CreateKeyFrameData(FbxNode * node, const D3DXMATRIX& transform, FbxTime animationTime);

		//바이너리 파일 쓰기 및 저장
		//@param : 저장 폴더
		//@param : 파일명
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