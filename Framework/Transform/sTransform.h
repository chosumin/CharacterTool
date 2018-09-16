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
	void Rotate(const D3DXVECTOR3& deltaAngle);

	//기저축을 중심으로 변화량만큼 회전
	void RotateToFixedMatrix(const D3DXMATRIX& fixedMat, const D3DXVECTOR3& deltaAngle);

	const D3DXMATRIX& GetRotationMatrix() const;
	const D3DXMATRIX& GetScaleMatrix() const;

	void GetDirection(OUT D3DXVECTOR3& direction);
	void SetDirection(IN const D3DXVECTOR3& direction);

	//@brief : 지정 방향으로 이동
	//@param : 속도
	//@param : 방향
	void Move(float speed, const D3DXVECTOR3& direction);

	//@brief : 현재 방향으로 이동
	//@param : 속도
	void Move(float speed);

	void SetMatrix(const D3DXMATRIX& world);
	void SetVSBuffer(UINT slot);
private:
	void Scale();
	void Rotate();
	void Rotate(const D3DXVECTOR3& axis, float deltaRadian);
	void Translation();
	void GetRadian(D3DXVECTOR3 *radian, const D3DXVECTOR3& angle);
	void GetAxis(D3DXVECTOR3 *axis, const D3DXVECTOR3& angle);
public:
	float Velocity;

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