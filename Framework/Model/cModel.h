#pragma once
#include "./Interface/iClonable.h"

class cModelBone;
class cModelMesh;
class cMaterial;
struct sTransform;
class cModel : public iClonable<cModel>
{
private:
	friend class cModelFactory;
public:
	~cModel();

	void Update();
	void Render();
public:
	/*******************
		Getter Setter
	********************/
	weak_ptr<sTransform> GetTransform() { return _transform; }
	
	weak_ptr<cMaterial> GetMaterial(wstring name) const;

	vector<shared_ptr<cModelMesh>>& GetMeshes() { return _meshes; }

	weak_ptr<cModelMesh> GetMesh(UINT index) { return _meshes[index]; }
	weak_ptr<cModelMesh> GetMesh(wstring name);
					   
	weak_ptr<cModelBone> GetBone(UINT index) { return _bones[index]; }
	weak_ptr<cModelBone> GetBone(wstring name);
					   
	weak_ptr<cModelBone> GetRoot() { return _root; }
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

	vector<shared_ptr<cMaterial>>	_materials;
	vector<shared_ptr<cModelBone>>	_bones;
	vector<shared_ptr<cModelMesh>>	_meshes;

	//todo : 복합체 패턴으로 변경
	shared_ptr<sTransform> _transform;
};

