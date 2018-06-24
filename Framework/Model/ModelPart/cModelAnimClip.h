#pragma once
#include "./Interface/iCloneable.h"

struct sModelKeyFrame;
class cModelAnimClip : public iCloneable<cModelAnimClip>
{
private:
	friend class cModel;
	friend class cModelFactory;
public:
	cModelAnimClip();
	~cModelAnimClip();

	float GetFrameRate() { return _frameRate; }
	int GetTotalFrame() { return _totalFrame; }

	sModelKeyFrame* GetKeyframe(wstring name);
public:
	// iCloneable��(��) ���� ��ӵ�
	virtual std::unique_ptr<cModelAnimClip> Clone() const override;
private:
	wstring _name;

	int _totalFrame; //��ü ������ ��
	float _frameRate; //�� �����Ӵ� �ð�
	float _defaultFrameRate; //������ ����, �ǵ������

	unordered_map<wstring, unique_ptr<sModelKeyFrame>> _keyFrames;
};