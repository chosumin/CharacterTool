#pragma once
#include "./Interface/iCloneable.h"

class cModelBone;
class cModelMeshPart;
class cModelMesh : public iCloneable<cModelMesh>
{
private:
	friend class cModel;
	friend class cModelFactory;
public:
	cModelMesh();
	~cModelMesh();

	void Render();

	//todo : 콜라이더 부모 테스트
	weak_ptr<struct sTransform> GetParentTransform() const;

	D3DXVECTOR3 GetMeshPoint() const;
private:
	// iClonable을(를) 통해 상속됨
	virtual unique_ptr<cModelMesh> Clone() const override;
private:
	/*******************
		Getter Setter
	********************/
	wstring	GetName() const { return _name; }
	int	GetParentBoneIndex() const { return _parentBoneIndex; }

	void SetWorld(const D3DXMATRIX& world) { _worldBuffer->SetMatrix(world); }

	
private:
	void Binding();
private:
	wstring _name;

	int _parentBoneIndex;
	weak_ptr<cModelBone> _parentBone;

	unique_ptr<cWorldBuffer> _worldBuffer;

	vector<shared_ptr<cModelMeshPart>> _meshParts;
};