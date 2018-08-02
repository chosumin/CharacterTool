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

	void SetCurrentClip(weak_ptr<cAnimClip> clip);
	bool Duplicated(weak_ptr<cAnimClip> clip);
	void AddClip(weak_ptr<cAnimClip> clip);
	ClipIter DeleteClip(weak_ptr<cAnimClip> clip);
public:
	/*******************
		Getter Setter
	********************/
	const vector<shared_ptr<cAnimClip>>& GetClips() const;
	int GetCurrentFrameCount();
	void SetCurrentFrameCount(int count) { _currentKeyFrame = count; }
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

	//���� �ִϸ��̼� Ŭ��
	weak_ptr<cAnimClip> _currentClip;

	//�ִϸ��̼� �����̳�
	vector<shared_ptr<cAnimClip>> _clips;

	int _currentKeyFrame; //���� ������ ��ȣ
	int _nextKeyFrame; //���� ������ ��ȣ
	float _frameTime; //������ ��� �ð�
	float _keyFrameFactor; //���� ����
};