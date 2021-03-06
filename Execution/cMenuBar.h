#pragma once

class cMenuBar : public iExecutable
{
public:
	cMenuBar(weak_ptr<sGlobalVariable> global);
	~cMenuBar();
public:
	/**********************************
		iExecutable을(를) 통해 상속됨
	***********************************/
	virtual void Init() override;
	virtual void Update() override;
	virtual void PreRender() override {}
	virtual void Render() override {}
	virtual void PostRender() override;
	virtual void ResizeScreen() override {}
private:
	void ShowMenuBar();
	void ShowGlobalLight();
	void ShowDemo();
	void ShowSystemInfo();
private:
	void ChangeShowGlobalLightWindow();
	void ChangeShowEnvironmentWindow();
	void ChangeShowLandscapeWindow();
	void ChangeShowSystemInfoWindow();
	void ChangeShowDemoWindow();
private:
	weak_ptr<sGlobalVariable> _global;
};