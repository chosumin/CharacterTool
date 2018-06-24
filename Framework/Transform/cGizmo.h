#pragma once

struct sTransform;
class cModel;
class cGizmo : public cSingletone<cGizmo>
{
private:
	friend class cSingletone<cGizmo>;
public:
	void SetGlobalVariable(weak_ptr<sGlobalVariable> global);
public:
	void AddTransform(weak_ptr<sTransform> transform);
	void Update();
	void Render();
	void PostRender();
private:
	cGizmo();
	~cGizmo();

	//기즈모 트랜스폼 갱신
	void SetMyTransform();

	//카메라와의 거리에 따른 크기 비율 조절
	void GetScaleRate(const D3DXVECTOR3& camPos);

	//기즈모 트랜스폼 역변환하여 카메라의 변환 좌표 구함
	void GetInverseVector(OUT D3DXVECTOR3 *transPos, OUT D3DXVECTOR3* transDir, const D3DXVECTOR3& originPos, const D3DXVECTOR3& originDir);

	void SelectMode();
private:
	weak_ptr<sGlobalVariable> _global;

	//피킹된 객체들의 트랜스폼
	vector<weak_ptr<sTransform>> _pickedTransforms;

	//객체들의 대표 트랜스폼
	weak_ptr<sTransform> _delegateTransform;

	//기즈모들
	vector<unique_ptr<class cBasicGizmo>> _gizmos;

	//기즈모 트랜스폼
	unique_ptr<sTransform> _myTransform;

	//기즈모 모드
	UINT _selectedNum = 0;

	ID3D11RasterizerState* _rasterizer[2];
	ID3D11DepthStencilState* _depthStenciler[2];
	ID3D11BlendState* _blender[2];
};