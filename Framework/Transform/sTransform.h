#pragma once

struct sTransform
{
public:
	sTransform();
	sTransform(const sTransform& world);
	~sTransform();

	void Decompose();
	void Update();
	
	sTransform& operator=(const sTransform& other);
	sTransform operator*(const sTransform& other);

	//변화량만큼 회전
	void Rotate(D3DXVECTOR3 deltaAngle);

	//기저축을 중심으로 변화량만큼 회전
	void RotateToFixedMatrix(const D3DXMATRIX& fixedMat, const D3DXVECTOR3& deltaAngle);

	void SetMatrix(const D3DXMATRIX& world);
	void SetVSBuffer(UINT slot);

	const D3DXMATRIX& GetRotationMatrix() const;
	const D3DXMATRIX& GetScaleMatrix() const;
private:
	void Scale();
	void Rotate();
	void Rotate(const D3DXVECTOR3& axis, float deltaRadian);
	void Translation();
	void GetRadian(D3DXVECTOR3 *radian, const D3DXVECTOR3& angle);
	void GetAxis(D3DXVECTOR3 *axis, const D3DXVECTOR3& angle);
public:
	//todo : Velocity, Direction
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