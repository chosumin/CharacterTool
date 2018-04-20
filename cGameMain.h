#pragma once

class cGameMain :
	public iClass
{
public:
	cGameMain();
	~cGameMain();
public:
	// iClass��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void ResizeScreen() override;
private:
	vector<iClass*>	m_vecObject;
};

