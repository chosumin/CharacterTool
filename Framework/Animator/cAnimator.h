#pragma once

class cAnimClip;
class cModel;
class cAnimator
{
public:
	enum class Mode
	{
		PLAY, STOP, PAUSE
	};
	
	cAnimator(weak_ptr<cModel> model);
	~cAnimator();

	void Update();

	void SetCurrentClip(weak_ptr<cAnimClip> clip);
	void AddClip(weak_ptr<cAnimClip> clip);
	void DeleteClip(weak_ptr<cAnimClip> clip);
public:
	/*******************
		Getter Setter
	********************/
	const vector<shared_ptr<cAnimClip>>& GetClips() const;
	int GetCurrentFrameCount();
	int GetLastFrameCount();

	void SetMode(Mode mode)
	{
		_mode = mode;
	}

	void SetModel(weak_ptr<cModel> model);
private:
	void UpdateTime();
	void UpdateBones();

	void SetIdentityAnimation();
private:
	Mode _mode;

	weak_ptr<cModel> _model;

	//현재 애니메이션 클립
	weak_ptr<cAnimClip> _currentClip;

	//애니메이션 컨테이너
	vector<shared_ptr<cAnimClip>> _clips;

	int _currentKeyFrame; //현재 프레임 번호
	int _nextKeyFrame; //다음 프레임 번호
	float _frameTime; //프레임 경과 시간
	float _keyFrameFactor; //보간 비율
};