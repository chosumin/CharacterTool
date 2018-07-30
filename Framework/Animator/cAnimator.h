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

	//���� �ִϸ��̼� Ŭ��
	weak_ptr<cAnimClip> _currentClip;

	//�ִϸ��̼� �����̳�
	vector<shared_ptr<cAnimClip>> _clips;

	int _currentKeyFrame; //���� ������ ��ȣ
	int _nextKeyFrame; //���� ������ ��ȣ
	float _frameTime; //������ ��� �ð�
	float _keyFrameFactor; //���� ����
};