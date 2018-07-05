#pragma once

class cModel;
class cHierarchy
{
public:
	cHierarchy();
	~cHierarchy();
public:
	void Init();
	void Update();
	void PostRender();

	void PicksModel(weak_ptr<cModel> model);
private:
	weak_ptr<cModel> _model;
};