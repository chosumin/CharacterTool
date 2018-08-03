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

	//���� ����Ǵ� Ŭ�� ��ü
	void SetCurrentClip(weak_ptr<cAnimClip> clip);

	//�ߺ��� Ŭ���� �ִ��� Ȯ��
	bool Duplicated(weak_ptr<cAnimClip> clip);

	//�� Ŭ�� �߰�
	void AddClip(weak_ptr<cAnimClip> clip);

	//Ŭ�� ����
	ClipIter DeleteClip(weak_ptr<cAnimClip> clip);
public:
	/*******************
		Getter Setter
	********************/
	
	//Ŭ�� �����̳� ��ȯ
	const vector<shared_ptr<cAnimClip>>& GetClips() const;

	//���� ������ ��ȣ
	int GetCurrentFrameCount();

	//������ ��ȣ ����
	void SetCurrentFrameCount(int count) { _currentKeyFrame = count; }

	//Ŭ���� ������ ������ ��ȣ(ũ�� - 1)
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