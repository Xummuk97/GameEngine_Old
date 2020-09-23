///==========================================================================
///	Модуль:		Бот
///	Автор:		Андрей Гребенщиков
///	Год:		2019
///==========================================================================

#ifndef BOT_INCLUDE
#define BOT_INCLUDE

#include "player.h"
#include "messages.h"

// @brief Бот
class Bot : public Player
{
public:
	// @brief Тип бота
	enum struct BotType
	{
		Friend, // @brief Друг
		Enemy // @brief Враг
	};

	// @brief Действие бота
	enum struct Action
	{
		Stop, // @brief Стоять
		Move // @brief Передвигаться
	};

	// @brief Конструктор
	Bot();

	// @brief Деструктор
	virtual ~Bot();

	void Load();

	// @brief Обновление
	void Update();

	// @brief Пересечения
	// @param coord - По какой оси пересекаться (X или Y)
	void Intersects(char coord = 'y');

	// @brief Огонь
	void Fire();

	// @brief Установка размера видимой области игрока
	// Данная область будет таким образом взаимодействовать, что если игрок
	// с ней пересёкся и враг был повёрнут на игрока, то происходят определённые действия
	void SetSizeVisiblePlayerProperty();

	// @brief Проверка на пересечение пули с объектами столкновения
	// @param angle - Угол
	// @param player - Область игрока (Не видимая область игрока для бота)
	bool IsBulletCollision(float angle, const FloatRect& player);

	// @return Находится ли впереди объекта пропасть
	bool IsAbyss();

	// @brief Установка типа бота
	// @param type - Тип
	void SetBotType(const BotType& type);

	// @return Тип бота
	BotType GetBotType() const;

	// @brief Установка действия боту
	// @param action - Действие
	void SetAction(const Action& action);

	// @return Действие бота
	Bot::Action GetAction() const;

private:
	// @brief Слежение области видимости игрока за врагом
	void SetRectPlayer();

	// @brief Тип объекта
	BotType m_type = BotType::Enemy;

	// @brief Действие объекта
	Bot::Action m_action = Bot::Action::Move;
};

#endif /// BOT_INCLUDE