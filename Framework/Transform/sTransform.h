#pragma once

struct sTransform
{
public:
	sTransform();
	sTransform(const sTransform& world);
	~sTransform();

	void Update();
	
	void Rotate(D3DXVECTOR3 deltaAngle);
	void RotateToFixedMatrix(const D3DXMATRIX& fixedMat, const D3DXVECTOR3& deltaAngle);

	void SetMatrix(const D3DXMATRIX& world);
	void SetVSBuffer(UINT slot);

	D3DXMATRIX GetRotationMatrix() const
	{
		return _rotationMatrix;
	}
private:
	void Scale();
	void Rotate();
	void Rotate(const D3DXVECTOR3& axis, float deltaRadian);
	void Translation();
	void GetRadian(D3DXVECTOR3 *radian, const D3DXVECTOR3& angle);
	void GetAxis(D3DXVECTOR3 *axis, const D3DXVECTOR3& angle);
public:
	D3DXVECTOR3		Position;
	D3DXVECTOR3		Scaling;
	D3DXVECTOR3		Rotation;
	D3DXQUATERNION	Quaternion;
	D3DXMATRIX		Matrix;
private:
	cWorldBuffer*	_worldBuffer;
	D3DXMATRIX		_rotationMatrix;
	D3DXMATRIX		_scaleMatrix;
	D3DXMATRIX		_positionMatrix;
};