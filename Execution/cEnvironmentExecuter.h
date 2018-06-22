#pragma once

class cEnvironmentExecuter : public iExecutable
{
public:
	cEnvironmentExecuter();
	~cEnvironmentExecuter();
public:
	/**********************************
		iExecutable��(��) ���� ��ӵ�
	***********************************/
	virtual void Init() override;
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void ResizeScreen() override;
private:
	unique_ptr<class cWorldGrid> _grid;
};