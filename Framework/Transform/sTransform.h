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

	//��ȭ����ŭ ȸ��
	void Rotate(const D3DXVECTOR3& deltaAngle);

	//�������� �߽����� ��ȭ����ŭ ȸ��
	void RotateToFixedMatrix(const D3DXMATRIX& fixedMat, const D3DXVECTOR3& deltaAngle);

	const D3DXMATRIX& GetRotationMatrix() const
	{
		return _rotationMatrix;
	}
	const D3DXMATRIX& GetScaleMatrix() const
	{
		return _scaleMatrix;
	}

	void GetDirection(OUT D3DXVECTOR3& direction);
	void SetDirection(IN const D3DXVECTOR3& direction);

	//@brief : ���� �������� �̵�
	//@param : �ӵ�
	//@param : ����
	void Move(float speed, const D3DXVECTOR3& direction)
	{
		Velocity = speed;
		Position += direction * speed * cFrame::Delta();
	}

	//@brief : ���� �������� �̵�
	//@param : �ӵ�
	void Move(float speed);

	void SetMatrix(const D3DXMATRIX& world)
	{
		_worldBuffer->SetMatrix(world);

	}
	void SetVSBuffer(UINT slot)
	{
		_worldBuffer->SetVSBuffer(slot);
	}
private:
	void Scale()
	{
		D3DXMatrixScaling(&_scaleMatrix, Scaling.x, Scaling.y, Scaling.z);
	}
	void Rotate()
	{
		D3DXMatrixRotationQuaternion(&_rotationMatrix, &Quaternion);
	}
	void Translation()
	{
		_positionMatrix._41 = Position.x;
		_positionMatrix._42 = Position.y;
		_positionMatrix._43 = Position.z;
	}
	void Rotate(const D3DXVECTOR3& axis, float deltaRadian);

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