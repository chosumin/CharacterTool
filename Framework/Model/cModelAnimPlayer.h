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

	//루트만 모으기 위한 함수
	void GatherBones(vector<weak_ptr<cModelBone>>& bones, weak_ptr<cModelBone> bone);
private:
	shared_ptr<cShader> shader;

	weak_ptr<cModel> model;
	weak_ptr<class cModelAnimClip> currentClip;

	Mode mode;

	int currentKeyFrame;
	float frameTime; //현재 프레임부터 경과시간
	float keyFrameFactor; //프레임 사이의 보간 값

	int nextKeyFrame;

	bool bUseSlerp;

	vector<weak_ptr<cModelBone>> bones;

	vector<D3DXMATRIX> boneAnimation;
	vector<D3DXMATRIX> skinTransform; //실제 렌더링될 행렬
};