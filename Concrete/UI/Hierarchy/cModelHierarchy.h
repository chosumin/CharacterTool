#pragma once

class cActor;
class cModel;
class cModelMesh;
struct sTransform;
class cModelHierarchy
{
public:
	cModelHierarchy(weak_ptr<cActor> actor, ImGuiTreeNodeFlags open);
	~cModelHierarchy();

	void SetActor(weak_ptr<cActor> actor);
	void Update();
	void PostRender();
private:
	void ShowTransform();
	void ShowModel();
	void ShowMeshes(const vector<shared_ptr<cModelMesh>>& meshes);

	bool AlertActor();
	bool AlertModel();
private:
	weak_ptr<cActor> _actor;
	weak_ptr<cModel> _model;
	ImGuiTreeNodeFlags _open;

	//bool 
};