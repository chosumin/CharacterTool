#pragma once

struct sTransform;
class cBasicGizmo
{
public:
	static void SetTransform(weak_ptr<sTransform> transform);
public:
	cBasicGizmo(weak_ptr<sGlobalVariable> global);
	~cBasicGizmo();

	virtual void Update(const D3DXMATRIX& gizmoMatrix, const D3DXVECTOR3& mousePos, const D3DXVECTOR3& mouseDir) = 0;
	void Render();
protected:
	bool IsIntersect(const D3DXVECTOR3& pos, const D3DXVECTOR3& dir);
	D3DXVECTOR3 GetMouseDelta();
private:
	D3DXVECTOR3 GetDirection(UINT num);
protected:
	static weak_ptr<sTransform> _transform;
	static bool _isClick;
	static float rate;
	static float _length;
protected:
	weak_ptr<sGlobalVariable> _global;

	D3DXVECTOR3 _direction;

	vector<unique_ptr<class iRenderable>> _meshes;
	vector<unique_ptr<class cBoxCollider>> _axises;
	vector<unique_ptr<class cQuadCollider>> _quads;

	D3DXVECTOR3 _prevMousePos, _curMousePos;
	
	D3DXVECTOR3 axis[3] = { { 1,0,0 },{ 0,1,0 },{ 0,0,1 } };
};