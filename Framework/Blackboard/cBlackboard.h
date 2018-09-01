#pragma once

class cActor;
class cBlackboard
{
public:
	cBlackboard(weak_ptr<cActor> actor);
	~cBlackboard();

	bool IsChange();

	const map<string, bool>& GetBools() const { return _bools; }
	bool GetBool(const string& name) { return _bools[name]; }
	void SetBool(const string& name, bool val)
	{ _bools[name] = val; }

	const map<string, float>& GetFloats() const { return _floats; }
	float GetFloat(const string& name) { return _floats[name]; }
	void SetFloat(const string& name, float val)
	{ _floats[name] = val; }

	void GetDirection(OUT D3DXVECTOR3& direction);
private:
	bool OnMoveEvent();
private:
	float _hp;
	float _searchRange;
	float _speed;
	D3DXVECTOR3 _direction;

	map<string, bool> _bools;
	map<string, float> _floats;
};