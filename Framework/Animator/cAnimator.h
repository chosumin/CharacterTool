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
	bool Duplicated(const weak_ptr<cAnimClip> & clip);

	//�� Ŭ�� �߰�
	void AddClip(const shared_ptr<cAnimClip> & clip);

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
	UINT GetCurrentFrameCount();

	//������ ��ȣ ����
	void SetCurrentFrameCount(UINT count);

	//Ŭ���� ������ ������ ��ȣ(ũ�� - 1)
	UINT GetLastFrameCount();

	Mode GetMode();
	void SetMode(Mode mode);

	void SetModel(weak_ptr<cModel> model);

	const weak_ptr<cAnimClip> & GetCurrentClip() const;

	//�ִϸ��̼� �ӵ�
	float GetAnimSpeed() const;
	void SetAnimSpeed(float speed);
	void MultiplyAnimSpeed(float multiplied);
private:
	void UpdateTime();

	//���� Ű������ ī��Ʈ ������Ŵ
	void IncreaseKeyFrame(OUT UINT& curFrame, UINT totalFrame, bool isLoop);

	//���� �ð� ���
	void CalculateBlendTime();

	void UpdateBones();

	//����, ���� Ŭ�� ����
	void Blend(UINT size, shared_ptr<cModel>& modelPtr,
			   shared_ptr<cAnimClip>& front,
			   shared_ptr<cAnimClip>& back);

	//���� Ŭ���� ��ȯ ����� �״�� ����
	void GetCurrentClipMatrix(UINT size,
								   shared_ptr<cModel>& modelPtr,
								   shared_ptr<cAnimClip>& curClipPtr);

	//���� �ð� ���� ��� ���� �ʱ�ȭ
	void InitTime();

	//���� Ŭ���� ���� Ŭ������ ��ü
	void ShiftClip();

	//���� Ŭ�� ����
	void DeletePrevClip();
private:
	Mode _mode;
	weak_ptr<cModel> _model;
	vector<shared_ptr<cAnimClip>> _clips; //�ִϸ��̼� �����̳�
	float _frameTime; //���������� ���� �ð�
	float _invFrameRate;

	float _animSpeed;
private:
	struct ClipStruct
	{
		weak_ptr<cAnimClip> Clip;
		UINT CurrentKeyFrame;
		bool IsLoop; //�ִϸ��̼� ���� ����

		void Init()
		{
			CurrentKeyFrame = 0;
		}
	};
	ClipStruct _current;
	ClipStruct _prev;

	float _blendTime; //������ �ð�
	float _elapsedTime; //���� ��� �ð�
	float _interpolationTime; //���� ��
};