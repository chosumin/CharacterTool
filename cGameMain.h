#pragma once

class cGameMain : public iExecutable
{
public:
	cGameMain();
	~cGameMain();
public:
	// iExecutable을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void ResizeScreen() override;
private:
	shared_ptr<struct sGlobalVariable> _globalVariable;
	vector<shared_ptr<iExecutable>> _vecObject;
};