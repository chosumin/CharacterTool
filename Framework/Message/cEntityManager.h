#pragma once

enum class eIdGroup;
class cEntityManager : public cSingletone<cEntityManager>
{
private:
	friend class cSingletone<cEntityManager>;
public:
	//비트 마스킹
	static const UINT idBits;

	//현재 id에서 그룹이나 id를 쪼개서 반환
	//@param : 쪼갤 id(IN)
	//@param : 반환할 그룹(OUT)
	//@param : 반환할 id(OUT)
	void GetGroupId(IN const UINT id, UINT* pGroup, UINT* pId) const;

	weak_ptr<cBaseGameEntity> GetEntityFromId(UINT id) const;
	
	UINT GetId(const weak_ptr<cBaseGameEntity> entity) const;

	//해당 그룹에 속한 모든 엔티티들을 벡터에 삽입
	//@param : 식별 그룹
	//@param : 그룹에 속한 엔티티를 넣을 컨테이너
	void GetIdentityGroup(eIdGroup group, vector<shared_ptr<cBaseGameEntity>>& entities);

	//해당 그룹에 속한 모든 엔티티의 id를 벡터에 삽입
	//@param : 식별 그룹
	//@param : 그룹에 속한 id를 넣을 컨테이너
	void GetIdentityGroup(eIdGroup group, vector<UINT>& entities);

	//엔티티를 관리 컨테이너에 삽입
	//@param : 속할 그룹
	//@param : 넣을 객체
	void RegisterEntity(const eIdGroup group, weak_ptr<cBaseGameEntity> object);

	//엔티티를 관리 컨테이너에서 해제
	//@param : 해제할 객체
	void RemoveEntity(UINT id);

	void Clear();
private:
	cEntityManager();
	~cEntityManager();

	//엔티티 컨테이너
	unordered_map<UINT, weak_ptr<cBaseGameEntity>> _entityMap;

	//todo : 엔티티맵에서 expired된 객체는 지워주는 스레드가 필요할듯
};