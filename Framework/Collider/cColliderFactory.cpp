#include "stdafx.h"
#include "cColliderFactory.h"
#include "./Transform/sTransform.h"
#include "./Collider/cCylinderCollider.h"
#include "./Collider/cBoxCollider.h"
#include "./Collider/cSphereCollider.h"

vector<const char*> cColliderFactory::_shapeList =
	{ "Box", "Cylinder", "Sphere", "Line", "Quad", "Ray" };
D3DXMATRIX cColliderFactory::_matrix;
weak_ptr<sTransform> cColliderFactory::_parent;

unique_ptr<cCollider> cColliderFactory::Create(eColliderType type,
											   eColliderShape shape,
											   weak_ptr<sTransform> parent,
											   const D3DXMATRIX & matrix)
{
	_matrix = matrix;
	_parent = parent;

	unique_ptr<cCollider> collider;
	switch (shape)
	{
		case eColliderShape::CYLINDER:
			collider = CreateCylinder();
			break;
		case eColliderShape::BOX:
			collider = CreateBox();
			break;
		case eColliderShape::SPHERE:
			collider = CreateSphere();
			break;

	}

	collider->_type = type;
	collider->_shape = shape;

	return move(collider);
}

const vector<const char*>& cColliderFactory::GetList()
{
	return _shapeList;
}

unique_ptr<cCollider> cColliderFactory::CreateCylinder()
{
	auto collider = make_unique<cCylinderCollider>(_parent,
												   1.0f, 0.5f);
	return move(collider);
}

unique_ptr<cCollider> cColliderFactory::CreateBox()
{
	auto collider = make_unique<cBoxCollider>(_parent,
											  D3DXVECTOR3{ -1,-1,-1 },
											  D3DXVECTOR3{ 1,1,1 });
	return move(collider);
}

unique_ptr<cCollider> cColliderFactory::CreateSphere()
{
	auto collider = make_unique<cSphereCollider>(_parent,
											  D3DXVECTOR3{ 0,0,0 },
											  1.0f);
	return move(collider);
}
