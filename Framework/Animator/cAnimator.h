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
	void SetCurrentClip(const weak_ptr<cAnimClip> & clip, bool overwrite = true, float blendTime = 0.1f, bool isLoop = true);

	//�ߺ��� Ŭ���� �ִ��� Ȯ��
	bool Duplicated(weak_ptr<cAnimClip> clip);

	//�� Ŭ�� �߰�
	void AddClip(weak_ptr<cAnimClip> clip);

	//Ŭ�� ����
	ClipIter DeleteClip(weak_ptr<cAnimClip> clip);

	bool IsEndAnimation();
public:
	/*******************
		Getter Setter
	********************/
	
	//Ŭ�� �����̳� ��ȯ
	const vector<shared_ptr<cAnimClip>>& GetClips() const;

	//���� ������ ��ȣ
	int GetCurrentFrameCount();

	//������ ��ȣ ����
	void SetCurrentFrameCount(int count);

	//Ŭ���� ������ ������ ��ȣ(ũ�� - 1)
	int GetLastFrameCount();

	Mode GetMode();
	void SetMode(Mode mode);

	void SetModel(weak_ptr<cModel> model);

	const weak_ptr<cAnimClip> & GetCurrentClip() const;
private:
	void UpdateTime();

	//���� Ű������ ī��Ʈ ������Ŵ
	void IncreaseKeyFrame(OUT int& curFrame, int totalFrame, bool isLoop);

	//���� �ð� ���
	void CalculateBlendTime();

	void UpdateBones();

	//����, ���� Ŭ�� ����
	void Blend(UINT size, shared_ptr<cModel>& modelPtr,
			   shared_ptr<cAnimClip>& curClipPtr,
			   shared_ptr<cAnimClip>& nextClipPtr);

	//���� Ŭ���� ��ȯ ����� �״�� ����
	void GetCurrentClipMatrix(UINT size,
								   shared_ptr<cModel>& modelPtr,
								   shared_ptr<cAnimClip>& curClipPtr);

	//���� �ð� ���� ��� ���� �ʱ�ȭ
	void InitTime();

	//���� Ŭ���� ���� Ŭ������ ��ü
	void ShiftClip();
private:
	Mode _mode;
	weak_ptr<cModel> _model;
	vector<shared_ptr<cAnimClip>> _clips; //�ִϸ��̼� �����̳�
	float _frameTime; //���������� ���� �ð�
	float _invFrameRate;

private:
	struct ClipStruct
	{
		weak_ptr<cAnimClip> Clip;
		int CurrentKeyFrame;
		bool IsLoop; //�ִϸ��̼� ���� ����

		void Init()
		{
			CurrentKeyFrame = 0;
		}
	};
	ClipStruct _current;
	ClipStruct _next;

	float _blendTime; //������ �ð�
	float _elapsedTime; //���� ��� �ð�
	float _interpolationTime; //���� ��
};