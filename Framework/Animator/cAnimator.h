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
	void SetCurrentClip(const weak_ptr<cAnimClip> & clip, bool overwrite = true, float blendTime = 0.1f, bool isLoop = true);

	//중복된 클립이 있는지 확인
	bool Duplicated(const weak_ptr<cAnimClip> & clip);

	//새 클립 추가
	void AddClip(const shared_ptr<cAnimClip> & clip);

	//클립 삭제
	ClipIter DeleteClip(weak_ptr<cAnimClip> clip);

	bool IsEndAnimation();
public:
	/*******************
		Getter Setter
	********************/
	
	//클립 컨테이너 반환
	const vector<shared_ptr<cAnimClip>>& GetClips() const;

	//현재 프레임 번호
	UINT GetCurrentFrameCount();

	//프레임 번호 세팅
	void SetCurrentFrameCount(UINT count);

	//클립의 마지막 프레임 번호(크기 - 1)
	UINT GetLastFrameCount();

	Mode GetMode();
	void SetMode(Mode mode);

	void SetModel(weak_ptr<cModel> model);

	const weak_ptr<cAnimClip> & GetCurrentClip() const;

	//애니메이션 속도
	float GetAnimSpeed() const;
	void SetAnimSpeed(float speed);
	void MultiplyAnimSpeed(float multiplied);
private:
	void UpdateTime();

	//현재 키프레임 카운트 증가시킴
	void IncreaseKeyFrame(OUT UINT& curFrame, UINT totalFrame, bool isLoop);

	//블렌드 시간 계산
	void CalculateBlendTime();

	void UpdateBones();

	//현재, 다음 클립 블렌드
	void Blend(UINT size, shared_ptr<cModel>& modelPtr,
			   shared_ptr<cAnimClip>& front,
			   shared_ptr<cAnimClip>& back);

	//현재 클립의 변환 행렬을 그대로 전달
	void GetCurrentClipMatrix(UINT size,
								   shared_ptr<cModel>& modelPtr,
								   shared_ptr<cAnimClip>& curClipPtr);

	//블렌드 시간 관련 멤버 변수 초기화
	void InitTime();

	//현재 클립을 이전 클립으로 교체
	void ShiftClip();

	//이전 클립 삭제
	void DeletePrevClip();
private:
	Mode _mode;
	weak_ptr<cModel> _model;
	vector<shared_ptr<cAnimClip>> _clips; //애니메이션 컨테이너
	float _frameTime; //프레임율에 맞춘 시간
	float _invFrameRate;

	float _animSpeed;
private:
	struct ClipStruct
	{
		weak_ptr<cAnimClip> Clip;
		UINT CurrentKeyFrame;
		bool IsLoop; //애니메이션 루프 여부

		void Init()
		{
			CurrentKeyFrame = 0;
		}
	};
	ClipStruct _current;
	ClipStruct _prev;

	float _blendTime; //블렌딩될 시간
	float _elapsedTime; //블렌딩 경과 시간
	float _interpolationTime; //보간 값
};