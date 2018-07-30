#pragma once

class cModelBone;
class cModelMesh;
class cModelMeshPart;
class cBinaryReader;
class cModel;
class cModelFactory
{
public:
	//Model 팩토리
	//@param : 모델 생성 경로
	//@param : 모델명
	static unique_ptr<cModel> Create(wstring filePath, wstring fileName);

	//Model 원형 복사
	//@param : 복사할 모델
	static unique_ptr<cModel> Clone(weak_ptr<cModel> model);
private:
	cModelFactory();
	~cModelFactory();
private:
	static void ReadMaterials(wstring file);
	static void ReadMesh(wstring file);
private:
	static void BindMeshData();

	//텍스쳐맵 읽어오기
	//@param : 텍스쳐를 저장할 머터리얼
	//@param : 텍스쳐 종류
	//@param : 경로
	static void ReadTextureMap(class cMaterial *material, enum class TextureType type, string directory, string fileName);

	//본 데이터 불러오기
	static void ReadModelBoneData(weak_ptr<cBinaryReader> r, weak_ptr<cModelBone> bone);

	//메쉬 데이터 불러오기
	static void ReadMeshData(weak_ptr<cBinaryReader> r, weak_ptr<cModelMesh> mesh);

	//메쉬 파트 데이터 불러오기
	static void ReadMeshPartData(weak_ptr<cBinaryReader> r, weak_ptr<cModelMeshPart> meshPart);

	//메쉬와 본을 연결
	static void LinkMeshToBone(weak_ptr<cModelMesh> mesh, int parentBoneIndex);
private:
	static unique_ptr<cModel> _model;
};