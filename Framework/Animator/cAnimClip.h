#pragma once

struct sAnimKeyFrame;
class cModelBone;
class cAnimClip
{
public:
	//애니메이션 클립 팩토리
	//@param : 파일 경로
	//@param : 파일명(캐쉬명)
	static shared_ptr<cAnimClip> Create(wstring filePath, wstring fileName);

	~cAnimClip();
public:
	const wstring& GetName() const { return _name; }
	void SetName(wstring name) { _name = name; }

	const wstring& GetFilePath() const { return _filePath; }

	//시간 당 프레임
	float GetFrameRate() const { return _frameRate; }

	//전체 프레임 수
	int GetTotalFrame() const { return _totalFrame; }

	//@brief : 해당 본 키프레임의 SRT 전달
	//@param : 찾을 본 이름
	//@param : 키 프레임 번호
	//@param : 전달 받을 t, q, s
	void GetKeyFrameSRT(const wstring& boneName, UINT index,
						OUT D3DXVECTOR3& t,
						OUT D3DXQUATERNION& q,
						OUT D3DXVECTOR3& s);

	//@brief : 본, 프레임 번호에 맞는 행렬 반환
	//@param : 반환 받을 행렬
	//@param : 본 이름
	//@param : 프레임 번호
	void GetFrameTransform(OUT D3DXMATRIX& transformMatrix, const wstring& boneName, UINT count);

	//보간한 행렬 내보냄
	//@param : 저장할 행렬
	//@param : 보간할 본
	//@param : 보간률
	//@param : 현재 프레임 번호
	//@param : 다음 프레임 번호
	void Interpolate(OUT D3DXMATRIX *pSRT, const wstring& boneName,
					 float keyFrameFactor, UINT current);

	//본과 키프레임이 일치하는지 비교
	//@param : 비교할 본 컨테이너
	bool IsCorrectBones(const vector<shared_ptr<cModelBone>>& bones);

	//해당 이름을 가진 키프레임이 존재하는지 비교
	//@param : 비교할 이름
	bool IsCorrectKeyFrame(const wstring& boneName);
private:
	cAnimClip(wstring filePath, wstring fileName);

	void ReadAnimation(wstring file);
private:
	wstring _filePath;
	wstring _name;

	UINT _totalFrame; //전체 프레임 수
	float _frameRate; //시간 당 프레임
	float _defaultFrameRate; //프레임 비율, 되돌리기용

	//@first : Bone 이름
	//@second : KeyFrameData 컨테이너
	unordered_map<wstring, unique_ptr<sAnimKeyFrame>> _keyFrames;

	//todo : 모델명, 본 이름, 매트릭스 저장하는 버퍼
};