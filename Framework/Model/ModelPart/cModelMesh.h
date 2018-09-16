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

	weak_ptr<struct sTransform> GetParentTransform() const;
	const wstring & GetName() const { return _name; }

	int	GetParentBoneIndex() const { return _parentBoneIndex; }
private:
	// iClonable을(를) 통해 상속됨
	virtual unique_ptr<cModelMesh> Clone() const override;
private:
	void Binding();
private:
	wstring _name;

	int _parentBoneIndex;
	weak_ptr<cModelBone> _parentBone;
	vector<shared_ptr<cModelMeshPart>> _meshParts;
};