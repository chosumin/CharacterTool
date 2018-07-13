#pragma once
#include "./Interface/iCloneable.h"

class cModelBone;
class cModelMesh;
class cMaterial;
class cModelAnimClip;
struct sTransform;
class cModel :
	public iCloneable<cModel>,
	public enable_shared_from_this<cModel>
{
private:
	friend class cModelFactory;
public:
	~cModel();

	void Update();
	void Render();

	void ShowHierarchy();
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
				
	UINT GetBoneCount() const { return _bones.size(); }
	weak_ptr<cModelBone> GetBone(UINT index) const { return _bones[index]; }
	weak_ptr<cModelBone> GetBone(wstring name);
					   
	weak_ptr<cModelBone> GetRoot() const { return _root; }

	weak_ptr<cModelAnimClip> GetClip(UINT index) const { return _clips[index]; }

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

	/********************************
		iCloneable을(를) 통해 상속됨
	*********************************/
	virtual std::unique_ptr<cModel> Clone() const override;
private:
	shared_ptr<cModelBone> _root;

	vector<shared_ptr<cMaterial>> _materials;
	vector<shared_ptr<cModelBone>> _bones;
	vector<shared_ptr<cModelMesh>> _meshes;
	vector<shared_ptr<cModelAnimClip>> _clips;

	shared_ptr<class cModelBoneBuffer> _buffer;
	//todo : 모델 본 내부에 본, 메쉬 가지게하고 업데이트는 루트본만 이뤄지게 하기
};