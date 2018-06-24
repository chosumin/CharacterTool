#pragma once

class cModel;
class cModelBone;
class cModelAnimPlayer
{
public:
	enum class Mode
	{
		Play = 0, Pause, Stop
	};

	cModelAnimPlayer(weak_ptr<cModel> model);
	~cModelAnimPlayer();

	void Update();
	void Render();
private:
	void UpdateTime();
	void UpdateBone();

	//��Ʈ�� ������ ���� �Լ�
	void GatherBones(vector<weak_ptr<cModelBone>>& bones, weak_ptr<cModelBone> bone);
private:
	shared_ptr<cShader> shader;

	weak_ptr<cModel> model;
	weak_ptr<class cModelAnimClip> currentClip;

	Mode mode;

	int currentKeyFrame;
	float frameTime; //���� �����Ӻ��� ����ð�
	float keyFrameFactor; //������ ������ ���� ��

	int nextKeyFrame;

	bool bUseSlerp;

	vector<weak_ptr<cModelBone>> bones;

	vector<D3DXMATRIX> boneAnimation;
	vector<D3DXMATRIX> skinTransform; //���� �������� ���
};