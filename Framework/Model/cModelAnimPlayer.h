#pragma once

struct sTransform;
class cModel;
class cModelBone;
class cModelAnimPlayer
{
public:
	cModelAnimPlayer(weak_ptr<cModel> model);
	~cModelAnimPlayer();

	enum class Mode
	{
		Play = 0, Pause, Stop
	};

	UINT GetClip() const;
	void SetClip(UINT clipNumber);

	void Update(weak_ptr<sTransform> actorWorld);
	void Render();
	void PostRender();
private:
	void UpdateTime();
	void UpdateBone(weak_ptr<sTransform> actorWorld);
private:
	shared_ptr<cShader> shader;

	weak_ptr<cModel> model;
	weak_ptr<class cModelAnimClip> currentClip;
	UINT currentClipNumber;

	Mode mode;

	int currentKeyFrame;
	int nextKeyFrame;

	float frameTime; //���� �����Ӻ��� ����ð�
	float keyFrameFactor; //������ ������ ���� ��

	bool bUseSlerp;

	vector<D3DXMATRIX> boneAnimation;
	vector<D3DXMATRIX> skinTransform; //���� �������� ���
};