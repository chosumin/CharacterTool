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
	// iCloneable을(를) 통해 상속됨
	virtual std::unique_ptr<cModelAnimClip> Clone() const override;
private:
	wstring _name;

	int _totalFrame; //전체 프레임 수
	float _frameRate; //한 프레임당 시간
	float _defaultFrameRate; //프레임 비율, 되돌리기용

	unordered_map<wstring, unique_ptr<sModelKeyFrame>> _keyFrames;
};