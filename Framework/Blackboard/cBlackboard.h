#pragma once

enum class eKeyEventType;
class cActor;
class cBlackboard
{
public:
	cBlackboard(weak_ptr<cActor> actor);
	~cBlackboard();

	bool IsChange();

	const unordered_map<string, bool>& GetBools() const
	{
		return _bools;
	}
	bool GetBool(const string& name) 
	{ 
		return _bools.at(name);
	}
	void SetBool(const string& name, bool val)
	{
		_bools.at(name) = val;
	}

	const unordered_map<string, float>& GetFloats() const
	{
		return _floats;
	}
	float GetFloat(const string& name)
	{
		return _floats.at(name);
	}
	void SetFloat(const string& name, float val)
	{
		_floats.at(name) = val;
	}

	const unordered_map<string, D3DXVECTOR3>& GetVector3s() const
	{
		return _vector3s;
	}
	const D3DXVECTOR3& GetVector3(const string& name) const
	{
		return _vector3s.at(name);
	}
	void SetVector3(const string& name, D3DXVECTOR3& val)
	{
		_vector3s.at(name) = val;
	}

	const unordered_map<string, int>& GetInts() const
	{
		return _ints;
	}
	const int& GetInt(const string& name) const
	{
		return _ints.at(name);
	}
	void SetInt(const string& name, int val)
	{
		_ints.at(name) = val;
	}
private:
	bool OnMoveEvent();
	void OnMoveKeyEvent(D3DXVECTOR3& direction, float value, eKeyEventType type);
	
	bool OnDashEvent();

	bool OnAttackEvent();
private:
	float _hp;
	float _searchRange;
	float _speed;

	unordered_map<string, D3DXVECTOR3> _vector3s;
	unordered_map<string, bool> _bools;
	unordered_map<string, float> _floats;
	unordered_map<string, int> _ints;
};