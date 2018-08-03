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

	//iCloneable��(��) ���� ��ӵ�
	virtual std::unique_ptr<cModel> Clone() const override;

	//�� �ִϸ�����
	//@param : ������ �� ��ȣ
	//@param : �ִϸ��̼� Ű ���������� ���� ���
	void AnimateBone(UINT index, const D3DXMATRIX& pAnimated);

	void ResetBones();
public:
	/*******************
		Getter Setter
	********************/
	const wstring& GetFilePath() { return _modelPath; }

	vector<shared_ptr<cMaterial>>& GetMaterials();

	weak_ptr<cMaterial> GetMaterial(wstring name) const;

	vector<shared_ptr<cModelMesh>>& GetMeshes() { return _meshes; }

	UINT GetMeshCount() const { return _meshes.size(); }
	weak_ptr<cModelMesh> GetMesh(UINT index) const { return _meshes[index]; }
	weak_ptr<cModelMesh> GetMesh(wstring name);
	
	const vector<shared_ptr<cModelBone>>& GetBones() const;
	UINT GetBoneCount() const { return _bones.size(); }
	weak_ptr<cModelBone> GetBone(UINT index) const { return _bones[index]; }
	weak_ptr<cModelBone> GetBone(const wstring& name) const;
   
	weak_ptr<cModelBone> GetRoot() const { return _rootBone; }

	void SetPlayedBuffer(bool isPlayAnim);
public:
	void CopyAbsoluteBoneTo(vector<D3DXMATRIX>& transforms);
protected:
	/************
		������
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

	shared_ptr<class cModelBoneBuffer> _buffer;
};