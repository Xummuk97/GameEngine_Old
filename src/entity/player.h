///==========================================================================
///	������:		�����
///	�����:		������ �����������
///	���:		2019
///==========================================================================

#ifndef PLAYER_INCLUDE
#define PLAYER_INCLUDE

#include "physix.h"
#include "..\core\animation.h"
#include "weapon.h"
#include "ability.h"

// @brief �����
class Player : public Physix
{
public:
	// @brief �����������
	Player();

	// @brief ����������
	virtual ~Player();

	// @brief ��������
	// ���������� ��� ������ �������� ������� �� �����
	virtual void Load();

	// @brief ����������
	virtual void Update();

	// @brief ���������
	virtual void Draw();

	// @return ������
	Weapons* GetWeapons();

	// @brief ��������� ������ �� �������
	// @param templatesWeapons - ������ ������
	void SetWeaponsProperty(map<string, shared_ptr<Weapon>>& templatesWeapons);

	// @brief ��������� ������������
	// @param delegate - ������� ������������
	void SetAbility(function<void (Entity* entity, const AbilityType& type, bool isNextEnd)>&& delegate);

	// @return ������������
	AbilityContainer* GetAbility();

protected:
	// @brief �����������
	AbilityContainer* m_Ability;

	// @brief ������
	Weapons m_Weapons;
};

#endif /// PLAYER_INCLUDE