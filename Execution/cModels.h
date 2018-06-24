#pragma once

class cModel;
class cModels : public iExecutable
{
public:
	cModels(weak_ptr<sGlobalVariable> global);
	~cModels();

	/**********************************
		iExecutable을(를) 통해 상속됨
	***********************************/
	virtual void Init() override;
	virtual void Update() override;
	virtual void PreRender() override {}
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void ResizeScreen() override {}
private:
	shared_ptr<cModel> model;

	weak_ptr<sGlobalVariable> _global;

	//class cMoveGizmo *gizmo;

	class cModelAnimPlayer *animPlayer;
	class cHierarchy *hierarchy;
};