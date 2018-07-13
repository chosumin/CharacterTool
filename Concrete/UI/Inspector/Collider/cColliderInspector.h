#pragma once

class cActor;
class cModelMesh;
class cCollider;
class cColliderInspector
{
public:
	cColliderInspector(weak_ptr<cActor> actor);
	~cColliderInspector();

	void Update();
	void Render();
	void PostRender();

	void SetMesh(weak_ptr<cModelMesh> mesh);
private:
	weak_ptr<cActor> _actor;
	weak_ptr<cModelMesh> _mesh;
	vector<weak_ptr<cCollider>> _damageCol;
	vector<weak_ptr<cCollider>> _attackCol;
};