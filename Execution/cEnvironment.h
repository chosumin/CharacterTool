#pragma once

class cEnvironment : public iExecutable
{
public:
	cEnvironment();
	~cEnvironment();
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
	unique_ptr<class cBasicMap> _map;
};