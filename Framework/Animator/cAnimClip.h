#pragma once

struct sAnimKeyFrame;
class cModelBone;
class cAnimClip
{
public:
	//�ִϸ��̼� Ŭ�� ���丮
	//@param : ���� ���
	//@param : ���ϸ�(ĳ����)
	static shared_ptr<cAnimClip> Create(wstring filePath, wstring fileName);

	~cAnimClip();
public:
	const wstring& GetName() const { return _name; }
	void SetName(wstring name) { _name = name; }

	const wstring& GetFilePath() const { return _filePath; }

	//�ð� �� ������
	float GetFrameRate() const { return _frameRate; }

	//��ü ������ ��
	int GetTotalFrame() const { return _totalFrame; }

	//@brief : �ش� �� Ű�������� SRT ����
	//@param : ã�� �� �̸�
	//@param : Ű ������ ��ȣ
	//@param : ���� ���� t, q, s
	void GetKeyFrameSRT(const wstring& boneName, UINT index,
						OUT D3DXVECTOR3& t,
						OUT D3DXQUATERNION& q,
						OUT D3DXVECTOR3& s);

	//@brief : ��, ������ ��ȣ�� �´� ��� ��ȯ
	//@param : ��ȯ ���� ���
	//@param : �� �̸�
	//@param : ������ ��ȣ
	void GetFrameTransform(OUT D3DXMATRIX& transformMatrix, const wstring& boneName, UINT count);

	//������ ��� ������
	//@param : ������ ���
	//@param : ������ ��
	//@param : ������
	//@param : ���� ������ ��ȣ
	//@param : ���� ������ ��ȣ
	void Interpolate(OUT D3DXMATRIX *pSRT, const wstring& boneName,
					 float keyFrameFactor, UINT current);

	//���� Ű�������� ��ġ�ϴ��� ��
	//@param : ���� �� �����̳�
	bool IsCorrectBones(const vector<shared_ptr<cModelBone>>& bones);

	//�ش� �̸��� ���� Ű�������� �����ϴ��� ��
	//@param : ���� �̸�
	bool IsCorrectKeyFrame(const wstring& boneName);
private:
	cAnimClip(wstring filePath, wstring fileName);

	void ReadAnimation(wstring file);
private:
	wstring _filePath;
	wstring _name;

	UINT _totalFrame; //��ü ������ ��
	float _frameRate; //�ð� �� ������
	float _defaultFrameRate; //������ ����, �ǵ������

	//@first : Bone �̸�
	//@second : KeyFrameData �����̳�
	unordered_map<wstring, unique_ptr<sAnimKeyFrame>> _keyFrames;

	//todo : �𵨸�, �� �̸�, ��Ʈ���� �����ϴ� ����
};