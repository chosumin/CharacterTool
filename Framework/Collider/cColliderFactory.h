#pragma once

struct sTransform;
class cCollider;
enum class eColliderType;
enum class eColliderShape;
class cColliderFactory
{
public:
	static unique_ptr<cCollider> Create(eColliderType type,
										eColliderShape shape,
										weak_ptr<sTransform> parent,
										const D3DXMATRIX& matrix);
	static const vector<const char*>& GetList();
private:
	static unique_ptr<cCollider> CreateCylinder();
	static unique_ptr<cCollider> CreateBox();
	static unique_ptr<cCollider> CreateSphere();
private:
	static vector<const char*> _shapeList;
	static weak_ptr<sTransform> _parent;
	static D3DXMATRIX _matrix;
};