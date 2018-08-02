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

	//�ð� �� ������
	float GetFrameRate() const { return _frameRate; }

	//��ü ������ ��
	int GetTotalFrame() const { return _totalFrame; }

	//������ ��� ������
	//@param : ������ ���
	//@param : ������ ��
	//@param : ������
	//@param : ���� ������ ��ȣ
	//@param : ���� ������ ��ȣ
	void Interpolate(OUT D3DXMATRIX *pSRT, const wstring& boneName,
					 float keyFrameFactor, UINT current, UINT next);

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
	wstring _name;

	int _totalFrame; //��ü ������ ��
	float _frameRate; //�ð� �� ������
	float _defaultFrameRate; //������ ����, �ǵ������

	//@first : Bone �̸�
	//@second : KeyFrameData �����̳�
	map<wstring, unique_ptr<sAnimKeyFrame>> _keyFrames;
};