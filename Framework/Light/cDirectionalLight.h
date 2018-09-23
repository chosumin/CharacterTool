#pragma once

class cRenderTarget;
class cDirectionalLight
{
public:
	cDirectionalLight(weak_ptr<sGlobalVariable> global);
	~cDirectionalLight();

	void AddObject(shared_ptr<class cActor> & actor);

	D3DXMATRIX View() { return _view; }
	D3DXMATRIX Projection() { return _projection; }

	ID3D11ShaderResourceView* SRV();

	void Update();
	void Render();
private:
	weak_ptr<sGlobalVariable> _global;

	shared_ptr<cShader> _shader;

	vector<weak_ptr<class cActor>> _objects;
	unique_ptr<cRenderTarget> _renderTarget;

	D3DXVECTOR3 _position;
	D3DXVECTOR3 _lookAt;
	D3DXMATRIX _view;
	D3DXMATRIX _projection;

	unique_ptr<cViewProjectionBuffer> _lightVPBuffer;
};