#pragma once
#include "./Interface/iCloneable.h"

class cModelBone;
class cModelMesh;
class cMaterial;
class cModelBoneBuffer;
struct sTransform;
class cModel :
	public iCloneable<cModel>,
	public enable_shared_from_this<cModel>
{
private:
	friend class cModelFactory;
public:
	~cModel();

	void Update(const weak_ptr<sTransform> & rootTransform);
	void Render(const bool & onShader = true);

	//iCloneable을(를) 통해 상속됨
	virtual std::unique_ptr<cModel> Clone() const override;

	//본 애니메이팅
	//@param : 갱신할 본 번호
	//@param : 애니메이션 키 프레임으로 계산된 행렬
	void AnimateBone(UINT index, const D3DXMATRIX& pAnimated);

	void ResetBones();
public:
	/*******************
		Getter Setter
	********************/
	const wstring& GetFilePath() { return _modelPath; }

	vector<shared_ptr<cMaterial>>& GetMaterials()
	{
		return _materials;
	}

	const shared_ptr<cMaterial> & GetMaterial(wstring name) const;

	vector<shared_ptr<cModelMesh>>& GetMeshes() { return _meshes; }

	UINT GetMeshCount() const { return _meshes.size(); }
	const shared_ptr<cModelMesh> & GetMesh(UINT index) const { return _meshes[index]; }
	const shared_ptr<cModelMesh> & GetMesh(wstring name);
	
	const vector<shared_ptr<cModelBone>>& GetBones() const
	{
		return _bones;
	}
	UINT GetBoneCount() const { return _bones.size(); }
	const shared_ptr<cModelBone> & GetBone(UINT index) const { return _bones[index]; }
	const shared_ptr<cModelBone> & GetBone(const wstring& name) const;
   
	const weak_ptr<cModelBone> & GetRoot() const { return _rootBone; }

	void SetPlayedBuffer(bool isPlayAnim);
protected:
	/************
		생성자
	*************/
	cModel();
private:
	wstring _modelPath;

	weak_ptr<sTransform> _rootTransform;

	weak_ptr<cModelBone> _rootBone;

	vector<shared_ptr<cMaterial>> _materials;
	vector<shared_ptr<cModelMesh>> _meshes;

	vector<shared_ptr<cModelBone>> _bones;
	vector<D3DXMATRIX> _skinnedTM;

	shared_ptr<cModelBoneBuffer> _buffer;

	static ID3D11RasterizerState* _rasterizer[2];
	static bool _init;
};