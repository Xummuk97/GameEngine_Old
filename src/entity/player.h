///==========================================================================
///	Модуль:		Игрок
///	Автор:		Андрей Гребенщиков
///	Год:		2019
///==========================================================================

#ifndef PLAYER_INCLUDE
#define PLAYER_INCLUDE

#include "physix.h"
#include "..\core\animation.h"
#include "weapon.h"
#include "ability.h"

// @brief Игрок
class Player : public Physix
{
public:
	// @brief Конструктор
	Player();

	// @brief Деструктор
	virtual ~Player();

	// @brief Загрузка
	// Вызывается при полной загрузке объекта на карте
	virtual void Load();

	// @brief Обновление
	virtual void Update();

	// @brief Рисование
	virtual void Draw();

	// @return Оружия
	Weapons* GetWeapons();

	// @brief Установка оружия из шаблона
	// @param templatesWeapons - Шаблон оружия
	void SetWeaponsProperty(map<string, shared_ptr<Weapon>>& templatesWeapons);

	// @brief Установка способностей
	// @param delegate - Функция способностей
	void SetAbility(function<void (Entity* entity, const AbilityType& type, bool isNextEnd)>&& delegate);

	// @return Способностей
	AbilityContainer* GetAbility();

protected:
	// @brief Способности
	AbilityContainer* m_Ability;

	// @brief Оружия
	Weapons m_Weapons;
};

#endif /// PLAYER_INCLUDE