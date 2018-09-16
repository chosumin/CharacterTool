#pragma once

enum class eIdGroup;
class cEntityManager : public cSingletone<cEntityManager>
{
private:
	friend class cSingletone<cEntityManager>;
public:
	//��Ʈ ����ŷ
	static const UINT idBits;

	//���� id���� �׷��̳� id�� �ɰ��� ��ȯ
	//@param : �ɰ� id(IN)
	//@param : ��ȯ�� �׷�(OUT)
	//@param : ��ȯ�� id(OUT)
	void GetGroupId(IN const UINT id, UINT* pGroup, UINT* pId) const;

	weak_ptr<cBaseGameEntity> GetEntityFromId(UINT id) const;
	
	UINT GetId(const weak_ptr<cBaseGameEntity> entity) const;

	//�ش� �׷쿡 ���� ��� ��ƼƼ���� ���Ϳ� ����
	//@param : �ĺ� �׷�
	//@param : �׷쿡 ���� ��ƼƼ�� ���� �����̳�
	void GetIdentityGroup(eIdGroup group, vector<shared_ptr<cBaseGameEntity>>& entities);

	//�ش� �׷쿡 ���� ��� ��ƼƼ�� id�� ���Ϳ� ����
	//@param : �ĺ� �׷�
	//@param : �׷쿡 ���� id�� ���� �����̳�
	void GetIdentityGroup(eIdGroup group, vector<UINT>& entities);

	//��ƼƼ�� ���� �����̳ʿ� ����
	//@param : ���� �׷�
	//@param : ���� ��ü
	void RegisterEntity(const eIdGroup group, weak_ptr<cBaseGameEntity> object);

	//��ƼƼ�� ���� �����̳ʿ��� ����
	//@param : ������ ��ü
	void RemoveEntity(UINT id);

	void Clear();
private:
	cEntityManager();
	~cEntityManager();

	//��ƼƼ �����̳�
	unordered_map<UINT, weak_ptr<cBaseGameEntity>> _entityMap;

	//todo : ��ƼƼ�ʿ��� expired�� ��ü�� �����ִ� �����尡 �ʿ��ҵ�
};