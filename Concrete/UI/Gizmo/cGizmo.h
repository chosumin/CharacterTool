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
	void AddTransform(weak_ptr<sTransform> localTransform,
					  weak_ptr<sTransform> worldTransform = weak_ptr<sTransform>());
	void Update();
	void Render();
	void PostRender();
private:
	cGizmo();
	~cGizmo();

	//����� Ʈ������ ����
	void SetMyTransform();

	//ī�޶���� �Ÿ��� ���� ũ�� ���� ����
	void SetScaleRate(const D3DXVECTOR3& camPos);

	void SelectMode();
private:
	weak_ptr<sGlobalVariable> _global;

	//��ŷ�� ��ü���� Ʈ������
	vector<weak_ptr<sTransform>> _pickedTransforms;

	//��ü���� ��ǥ Ʈ������
	weak_ptr<sTransform> _delegateLocal;
	weak_ptr<sTransform> _delegateWorld;

	//������
	vector<unique_ptr<class cBasicGizmo>> _gizmos;

	//����� Ʈ������
	unique_ptr<sTransform> _myLocal;
	unique_ptr<sTransform> _myWorld;

	//����� ���
	UINT _selectedNum = 0;

	ID3D11RasterizerState* _rasterizer[2];
	ID3D11DepthStencilState* _depthStenciler[2];
	ID3D11BlendState* _blender[2];
};