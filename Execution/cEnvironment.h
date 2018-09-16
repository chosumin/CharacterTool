#pragma once

class cEnvironment : public iExecutable
{
public:
	cEnvironment(weak_ptr<sGlobalVariable> global);
	~cEnvironment();
public:
	/**********************************
		iExecutable을(를) 통해 상속됨
	***********************************/
	virtual void Init() override;
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void ResizeScreen() override;
private:
	weak_ptr<sGlobalVariable> _global;
	unique_ptr<class cWorldGrid> _grid;
	unique_ptr<class cBasicMap> _map;
	unique_ptr<class cSky> _sky;
};