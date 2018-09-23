#pragma once

class iChangeInspector
{
public:
	virtual ~iChangeInspector() {};

	virtual bool IsChange() = 0;
};

enum class eKeyEventType;
class cActor;
class cBlackboard : public enable_shared_from_this<cBlackboard>
{
public:
	enum eInspectorType
	{
		USER, AI
	};
public:
	cBlackboard(weak_ptr<cActor> actor);
	~cBlackboard();

	void Init();

	//������ ������� �ǵ���
	void SetJsonPath(const wstring& jsonPath)
	{
		_jsonPath = jsonPath;
	}

	void ResetData();

	//������ �����Ͱ� ����Ǿ����� ����
	//����Ǿ����� �ൿƮ���� ��ȸ�Ѵ�.
	virtual bool IsChange();

public:
	const weak_ptr<cActor> & GetActor() const
	{
		return _me;
	}

	void ClearEnemies()
	{
		_enemies.clear();
	}

	const vector<weak_ptr<cActor>> & GetEnemies() const
	{
		return _enemies;
	}

	//�� ����
	void SetEnemy(const weak_ptr<cActor>& enemy)
	{
		_enemies.emplace_back(enemy);
	}

	//@brief : ��ȭ�� �˻��ϴ� ��ü ����
	//@param : ������ ��ü
	void SetChangeInspector(eInspectorType type);
	eInspectorType GetInspectorType() { return _type; }
public:
	//Bool ����

	const unordered_map<string, bool>& GetBools() const
	{
		return _bools;
	}
	const bool & GetBool(const string& name) 
	{ 
		return _bools.at(name);
	}
	void SetBool(const string& name, bool val)
	{
		_bools.at(name) = val;
	}
public:
	//Float ����

	const unordered_map<string, float>& GetFloats() const
	{
		return _floats;
	}
	const float & GetFloat(const string& name)
	{
		return _floats.at(name);
	}
	void SetFloat(const string& name, float val)
	{
		_floats.at(name) = val;
	}
public:
	//����3 ����

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
public:
	//Int ����

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
protected:
	wstring _jsonPath;

	eInspectorType _type;
	unique_ptr<iChangeInspector> _changeInspector;
	unordered_map<string, D3DXVECTOR3> _vector3s;
	unordered_map<string, bool> _bools;
	unordered_map<string, float> _floats;
	unordered_map<string, int> _ints;

	weak_ptr<cActor> _me;

	vector<weak_ptr<cActor>> _enemies;
};