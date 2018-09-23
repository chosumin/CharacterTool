#include "stdafx.h"
#include "sTransform.h"
#include "./Helper/cMath.h"

sTransform::sTransform()
	: Position(0, 0, 0)
	, Scaling(1, 1, 1)
	, Rotation(0, 0, 0)
	, Velocity(0.0f)
{
	D3DXQuaternionIdentity(&Quaternion);

	D3DXMatrixIdentity(&_rotationMatrix);
	D3DXMatrixIdentity(&_scaleMatrix);
	D3DXMatrixIdentity(&_positionMatrix);

	D3DXMatrixIdentity(&Matrix);

	_worldBuffer = new cWorldBuffer();
}

sTransform::sTransform(const sTransform & world)
{
	Position = world.Position;
	Scaling = world.Scaling;
	Rotation = world.Rotation;
	Quaternion = world.Quaternion;
	Matrix = world.Matrix;

	_worldBuffer = new cWorldBuffer();
	_rotationMatrix = world._rotationMatrix;
	_scaleMatrix = world._scaleMatrix;
	_positionMatrix = world._positionMatrix;
}

sTransform::~sTransform()
{
	SAFE_DELETE(_worldBuffer);
}

void sTransform::Decompose()
{
	D3DXMatrixDecompose(&Scaling, &Quaternion, &Position, &Matrix);
	Rotation = cMath::ToEulerAngles(Quaternion);

	Update();
}

void sTransform::Update()
{
	Scale();
	Rotate();
	Translation();

	D3DXMatrixMultiply(&Matrix, &_scaleMatrix, &_rotationMatrix);
	//Matrix = _scaleMatrix * _rotationMatrix;
	Matrix._41 = _positionMatrix._41, Matrix._42 = _positionMatrix._42, Matrix._43 = _positionMatrix._43;

	_worldBuffer->SetMatrix(Matrix);
}

sTransform & sTransform::operator=(const sTransform & other)
{
	Position = other.Position;
	Scaling = other.Scaling;
	Rotation = other.Rotation;
	Quaternion = other.Quaternion;
	Update();

	return *this;
}

sTransform sTransform::operator*(const sTransform & other)
{
	sTransform t;

	D3DXMatrixMultiply(&t.Matrix, &Matrix, &other.Matrix);
	//t.Matrix = Matrix * other.Matrix;
	t.Decompose();

	return t;
}

void sTransform::Rotate(const D3DXVECTOR3 & axis, float deltaRadian)
{
	//회전 축으로 쿼터니언 회전
	D3DXQUATERNION q;
	D3DXQuaternionRotationAxis(&q, &axis, deltaRadian);

	//현재 쿼터니언에 더함
	Quaternion *= q;

	//-360 ~ 360도 조절
	for (UINT i = 0; i < 3; i++)
	{
		if (abs(Rotation[i]) > 360.0f)
			Rotation[i] = 0.0f;
	}
}

void sTransform::Rotate(const D3DXVECTOR3& deltaAngle)
{
	//회전 축 구하기(x,y,z)
	D3DXVECTOR3 axis;
	GetAxis(&axis, deltaAngle);
	D3DXVec3Normalize(&axis, &axis);

	D3DXVECTOR3 radian;
	GetRadian(&radian, deltaAngle);
	float delta = radian[0] + radian[1] + radian[2];

	Rotate(axis, delta);
}

void sTransform::RotateToFixedMatrix(const D3DXMATRIX& fixedMat, const D3DXVECTOR3 & deltaAngle)
{
	//회전 축 구하기(x,y,z)
	D3DXVECTOR3 axis = { 0,0,0 };
	for (int i = 0; i < 3; i++)
	{
		if (abs(deltaAngle[i]) > D3DX_16F_EPSILON)
		{
			axis[0] = fixedMat(i, 0);
			axis[1] = fixedMat(i, 1);
			axis[2] = fixedMat(i, 2);
			D3DXVec3Normalize(&axis, &axis);

			D3DXVECTOR3 radian;
			GetRadian(&radian, deltaAngle);
			float delta = radian[0] + radian[1] + radian[2];

			Rotate(axis, delta);
		}
	}
}

void sTransform::Move(float speed)
{
	Velocity = speed;

	D3DXVECTOR3 dir;
	GetDirection(dir);

	Position += dir * speed * cFrame::Delta();
}

void sTransform::GetDirection(OUT D3DXVECTOR3 & direction)
{
	direction.x = _rotationMatrix._31;
	direction.y = _rotationMatrix._32;
	direction.z = _rotationMatrix._33;
	D3DXVec3Normalize(&direction, &direction);
}

void sTransform::SetDirection(IN const D3DXVECTOR3 & direction)
{
	D3DXVECTOR3 dir = direction;
	D3DXVec3Normalize(&dir, &dir);

	_rotationMatrix._31 = dir.x;
	_rotationMatrix._32 = dir.y;
	_rotationMatrix._33 = dir.z;

	D3DXQuaternionRotationMatrix(&Quaternion, &_rotationMatrix);
}

void sTransform::GetAxis(D3DXVECTOR3 * axis, const D3DXVECTOR3 & angle)
{
	for (int i = 0; i < 3; i++)
	{
		if (abs(angle[i]) > D3DX_16F_EPSILON)
		{
			(*axis)[0] = _rotationMatrix(i, 0);
			(*axis)[1] = _rotationMatrix(i, 1);
			(*axis)[2] = _rotationMatrix(i, 2);
		}
	}
}

void sTransform::GetRadian(D3DXVECTOR3 * radian, const D3DXVECTOR3 & angle)
{
	radian->x = angle.x * RADIAN;
	radian->y = angle.y * RADIAN;
	radian->z = angle.z * RADIAN;
}
