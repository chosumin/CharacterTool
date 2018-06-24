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

	//텍스쳐맵 읽어오기
	//@param : 텍스쳐를 저장할 머터리얼
	//@param : 텍스쳐 종류
	//@param : 경로
	void ReadTextureMap(class cMaterial *material, enum class TextureType type, string directory, string fileName);

	//본 데이터 불러오기
	void ReadModelBoneData(weak_ptr<cBinaryReader> r, weak_ptr<cModelBone> bone);

	//메쉬 데이터 불러오기
	void ReadMeshData(weak_ptr<cBinaryReader> r, weak_ptr<cModelMesh> mesh);

	//메쉬 파트 데이터 불러오기
	void ReadMeshPartData(weak_ptr<cBinaryReader> r, weak_ptr<cModelMeshPart> meshPart);

	//메쉬와 본을 연결
	void LinkMeshToBone(weak_ptr<cModelMesh> mesh, int parentBoneIndex);
private:
	//todo : 머터리얼, 메쉬, 애니메이션 캐시 만들기
	unique_ptr<cModel> _model;
};