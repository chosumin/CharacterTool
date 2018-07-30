#pragma once
#include "./Interface/iCloneable.h"

class cModelBone;
class cModelMesh;
class cMaterial;
struct sTransform;
class cModel :
	public iCloneable<cModel>,
	public enable_shared_from_this<cModel>
{
private:
	friend class cModelFactory;
public:
	~cModel();

	void Update(weak_ptr<sTransform> rootTransform);
	void Render();

	//iCloneable을(를) 통해 상속됨
	virtual std::unique_ptr<cModel> Clone() const override;

	//본 애니메이팅
	//@param : 갱신할 본 번호
	//@param : 애니메이션 키 프레임으로 계산된 행렬
	void AnimateBone(UINT index, const D3DXMATRIX& pAnimated);

	//로컬을 월드로 변환할 행렬 계산
	void UpdateWorld();
public:
	/*******************
		Getter Setter
	********************/
	vector<shared_ptr<cMaterial>>& GetMaterials();

	weak_ptr<cMaterial> GetMaterial(wstring name) const;

	vector<shared_ptr<cModelMesh>>& GetMeshes() { return _meshes; }

	UINT GetMeshCount() const { return _meshes.size(); }
	weak_ptr<cModelMesh> GetMesh(UINT index) const { return _meshes[index]; }
	weak_ptr<cModelMesh> GetMesh(wstring name);
	
	const vector<shared_ptr<cModelBone>>& GetBones() const;
	UINT GetBoneCount() const { return _originalBones.size(); }
	weak_ptr<cModelBone> GetBone(UINT index) const { return _originalBones[index]; }
   
	weak_ptr<cModelBone> GetRoot() const { return _rootBone; }

	weak_ptr<class cModelBoneBuffer> GetBuffer() const
	{
		return _buffer;
	}
	
public:
	void CopyAbsoluteBoneTo(vector<D3DXMATRIX>& transforms);
protected:
	/************
		생성자
	*************/
	cModel();
private:
	weak_ptr<sTransform> _rootTransform;

	weak_ptr<cModelBone> _rootBone;

	vector<shared_ptr<cMaterial>> _materials;
	vector<shared_ptr<cModelMesh>> _meshes;

	vector<shared_ptr<cModelBone>> _originalBones;
	vector<D3DXMATRIX> _animatedTM;
	vector<D3DXMATRIX> _skinnedTM;

	shared_ptr<class cModelBoneBuffer> _buffer;
};