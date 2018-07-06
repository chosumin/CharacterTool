#pragma once

class cModel;
class cActor;
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
	vector<weak_ptr<cActor>> _actor;
};