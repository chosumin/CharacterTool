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

	//������ ����� ��ȯ�� ��� ���
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
		������
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