///==========================================================================
///	������:		�����������
///	�����:		������ �����������
///	���:		2019
///==========================================================================

#ifndef ABILITY_INCLUDE
#define ABILITY_INCLUDE

#include "entity.h"

// @brief ��� �����������
enum struct AbilityType
{
	Speed, // @brief ���������� ��������
	Damage,
	God,

	WeaponSuper, // @brief ������������� ������� 3-�� ������
	WeaponHoming // @brief ����������� ����
};

// @brief �����������
struct Ability
{
	// @brief ������� �������
	float time = 0.0f;

	// @brief ����� ����� �����������
	float maxTime;

	// @brief �����������
	// @brief maxTime - ����� ����� �����������
	Ability(float maxTime);
};

// @brief ��������� ������������
class AbilityContainer
{
public:
	// @brief �-� ��� ������ � �������� � ������������
	using Delegate = function<void (Entity* entity, const AbilityType& type, bool isNextEnd)>;

	// @brief �����������
	AbilityContainer() = default;

	// @brief ����������
	~AbilityContainer() = default;

	// @brief ���������� �����������
	// @param type - ��� �����������
	// @param maxTime - ����� ����� �����������
	// ���� ����������� �����������, ������� ��� ����, �� ��� ������������
	void Add(const AbilityType& type, float maxTime);

	// @param type - ��� �����������
	// @return ����������� ����������� �� ��� ����
	shared_ptr<Ability>& Get(const AbilityType& type);

	// @return ����������� ������������
	map<AbilityType, shared_ptr<Ability>> Get();

	// @brief ��������� �-� ��� ������ � �������� � ������������
	// @param delegate - �������
	void SetDelegate(Delegate&& delegate);

	// @brief ����������
	// @param entity - ������
	void Update(Entity* entity);

private:
	// @brief �����������
	map<AbilityType, shared_ptr<Ability>> m_data;

	// �������
	Delegate m_delegate;
};

#endif /// ABILITY_INCLUDE