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

	//����� Ʈ������ ����
	void SetMyTransform();

	//ī�޶���� �Ÿ��� ���� ũ�� ���� ����
	void GetScaleRate(const D3DXVECTOR3& camPos);

	//����� Ʈ������ ����ȯ�Ͽ� ī�޶��� ��ȯ ��ǥ ����
	void GetInverseVector(OUT D3DXVECTOR3 *transPos, OUT D3DXVECTOR3* transDir, const D3DXVECTOR3& originPos, const D3DXVECTOR3& originDir);

	void SelectMode();
private:
	weak_ptr<sGlobalVariable> _global;

	//��ŷ�� ��ü���� Ʈ������
	vector<weak_ptr<sTransform>> _pickedTransforms;

	//��ü���� ��ǥ Ʈ������
	weak_ptr<sTransform> _delegateTransform;

	//������
	vector<unique_ptr<class cBasicGizmo>> _gizmos;

	//����� Ʈ������
	unique_ptr<sTransform> _myTransform;

	//����� ���
	UINT _selectedNum = 0;

	ID3D11RasterizerState* _rasterizer[2];
	ID3D11DepthStencilState* _depthStenciler[2];
	ID3D11BlendState* _blender[2];
};