#pragma once

class cUI : public iExecutable
{
public:
	cUI(weak_ptr<sGlobalVariable> global);
	~cUI();
public:
	// iExecutable��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void Update() override;
	virtual void PreRender() override {}
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void ResizeScreen() override {}
private:
	weak_ptr<sGlobalVariable> _global;
};