#pragma once
#include "./Interface/iCloneable.h"

class cModelBone;
class cModelMesh;
class cMaterial;
class cModelAnimClip;
struct sTransform;
class cModel : public iCloneable<cModel>
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

	weak_ptr<cModelMesh> GetMesh(UINT index) { return _meshes[index]; }
	weak_ptr<cModelMesh> GetMesh(wstring name);
					   
	weak_ptr<cModelBone> GetBone(UINT index) { return _bones[index]; }
	weak_ptr<cModelBone> GetBone(wstring name);
					   
	weak_ptr<cModelBone> GetRoot() { return _root; }

	weak_ptr<cModelAnimClip> GetClip(UINT index) { return _clips[index]; }

	weak_ptr<class cModelBoneBuffer> GetBuffer()
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
		iClonable을(를) 통해 상속됨
	*********************************/
	virtual std::unique_ptr<cModel> Clone() const override;
private:
	shared_ptr<cModelBone> _root;

	vector<shared_ptr<cMaterial>> _materials;
	vector<shared_ptr<cModelBone>> _bones;
	vector<shared_ptr<cModelMesh>> _meshes;
	vector<shared_ptr<cModelAnimClip>> _clips;

	shared_ptr<class cModelBoneBuffer> _buffer;
};

