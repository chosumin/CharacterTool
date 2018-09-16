#pragma once

namespace UI
{
	class cToolWindow;
	class cActorMenu;
	class cCameraMenu;
}

class cUI : public iExecutable
{
public:
	cUI(weak_ptr<sGlobalVariable> global);
	~cUI();
public:
	// iExecutable을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void PreRender() override {}
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void ResizeScreen() override {}
private:
	weak_ptr<sGlobalVariable> _global;

	shared_ptr<UI::cToolWindow> _toolWindow;
	shared_ptr<UI::cActorMenu> _actorMenu;
	shared_ptr<UI::cCameraMenu> _cameraMenu;
};