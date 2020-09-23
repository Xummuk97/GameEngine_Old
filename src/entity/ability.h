///==========================================================================
///	Модуль:		Способности
///	Автор:		Андрей Гребенщиков
///	Год:		2019
///==========================================================================

#ifndef ABILITY_INCLUDE
#define ABILITY_INCLUDE

#include "entity.h"

// @brief Тип способности
enum struct AbilityType
{
	Speed, // @brief Добавление скорости
	Damage,
	God,

	WeaponSuper, // @brief Одновременный выстрел 3-мя пулями
	WeaponHoming // @brief Автонаводка пуль
};

// @brief Способность
struct Ability
{
	// @brief Счётчик времени
	float time = 0.0f;

	// @brief Время жизни способности
	float maxTime;

	// @brief Конструктор
	// @brief maxTime - Время жизни способности
	Ability(float maxTime);
};

// @brief Контейнер способностей
class AbilityContainer
{
public:
	// @brief Ф-и для работы с объектом и способностей
	using Delegate = function<void (Entity* entity, const AbilityType& type, bool isNextEnd)>;

	// @brief Конструктор
	AbilityContainer() = default;

	// @brief Деструктор
	~AbilityContainer() = default;

	// @brief Добавление способности
	// @param type - Тип способности
	// @param maxTime - Время жизни способности
	// Если добавляется способность, которая уже есть, то она игнорируется
	void Add(const AbilityType& type, float maxTime);

	// @param type - Тип способности
	// @return Возвращение способности по его типу
	shared_ptr<Ability>& Get(const AbilityType& type);

	// @return Возвращение способностей
	map<AbilityType, shared_ptr<Ability>> Get();

	// @brief Установка ф-и для работы с объектом и способностей
	// @param delegate - Функция
	void SetDelegate(Delegate&& delegate);

	// @brief Обновление
	// @param entity - Объект
	void Update(Entity* entity);

private:
	// @brief Способности
	map<AbilityType, shared_ptr<Ability>> m_data;

	// Функция
	Delegate m_delegate;
};

#endif /// ABILITY_INCLUDE