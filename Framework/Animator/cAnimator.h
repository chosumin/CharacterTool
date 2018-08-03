#pragma once

class cAnimClip;
class cModel;
class cAnimator
{
private:
	using ClipIter = vector<shared_ptr<cAnimClip>>::iterator;
public:
	enum class Mode
	{
		PLAY, STOP, PAUSE
	};
	
	cAnimator(weak_ptr<cModel> model);
	~cAnimator();

	void Update();

	//현재 실행되는 클립 교체
	void SetCurrentClip(weak_ptr<cAnimClip> clip);

	//중복된 클립이 있는지 확인
	bool Duplicated(weak_ptr<cAnimClip> clip);

	//새 클립 추가
	void AddClip(weak_ptr<cAnimClip> clip);

	//클립 삭제
	ClipIter DeleteClip(weak_ptr<cAnimClip> clip);
public:
	/*******************
		Getter Setter
	********************/
	
	//클립 컨테이너 반환
	const vector<shared_ptr<cAnimClip>>& GetClips() const;

	//현재 프레임 번호
	int GetCurrentFrameCount();

	//프레임 번호 세팅
	void SetCurrentFrameCount(int count) { _currentKeyFrame = count; }

	//클립의 마지막 프레임 번호(크기 - 1)
	int GetLastFrameCount();

	Mode GetMode();
	void SetMode(Mode mode);

	void SetModel(weak_ptr<cModel> model);
private:
	void UpdateTime();
	void UpdateBones();
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