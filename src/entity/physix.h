///==========================================================================
///	Модуль:		Физика
///	Автор:		Андрей Гребенщиков
///	Год:		2019
///==========================================================================

#ifndef PHYSIX_INCLUDE
#define PHYSIX_INCLUDE

#include "entity.h"

// @brief Физика
class Physix : public Entity
{
public:
	// @brief Конструктор
	Physix();

	// @brief Деструктор
	virtual ~Physix();

	// @brief Движение влево
	// @param speed - Скорость
	// @param isCheckIntersects - Совершить проверку на пересечения
	virtual void GoLeft(float speed = 0.0f, bool isCheckIntersects = true);

	// @brief Движение вправо
	// @param speed - Скорость
	// @param isCheckIntersects - Совершить проверку на пересечения
	virtual void GoRight(float speed = 0.0f, bool isCheckIntersects = true);

	// @brief Движение вверх
	// @param speed - Скорость
	// @param isCheckIntersects - Совершить проверку на пересечения
	virtual void GoUp(float speed = 0.0f);

	// @brief Движение Вниз
	// @param speed - Скорость
	// @param isCheckIntersects - Совершить проверку на пересечения
	virtual void GoDown(float speed = 0.0f);

	// @brief Установка слоёв
	// @param layers - Слои
	void SetLayers(Layers* layers);

	// @return На земле ли объект
	void IsGround();

	// @param rect - Область
	// @return На земле ли область
	bool IsGround(const FloatRect& rect);

protected:
	// @brief Динамическое передвижение
	virtual void DynamicMove();

	// @brief Прыжок
	virtual void GoJump();

	// @brief Смена значение direction на up
	virtual void DirectionToUp();

	// @brief Смена значения direction на down
	virtual void DirectionToDown();

	// @brief Пересечения
	// @param coord - По какой оси пересекаться (X или Y)
	virtual void Intersects(char coord = 'y');

	// @brief Столкновение
	// @param _rect - Область
	virtual void Collision(char coord, const FloatRect& _rect);

	// @brief Перемещение по лестнице
	virtual void Stair();

	// @brief Обнуление перемещения по лестнице
	virtual void StairClear();

	// @brief Обнуление прыжка
	virtual void JumpClear();

	// @brief Пересечение с объектом скрипта
	// @param layer - Слой
	// @param entity - Объект
	// @return Было ли совершено пересечение
	bool IsIntersectScripts(const string& layer, const shared_ptr<Entity>& entity);

	// @brief Пересечение с объектом столкновение
	// @param layer - Слой
	// @param entity - Объект
	// @param coord - По какой оси пересекаться (X или Y)
	// @return Было ли совершено пересечение
	bool IsIntersectCollision(const string& layer, const shared_ptr<Entity>& entity, char coord);

	// @brief Пересечение с объектом мгновенно убивающего слоя
	// @param layer - Слой
	// @return Было ли совершено пересечение
	bool IsIntersectDamageAll(const string& layer);

	// @brief Пересечение с объектом постепенно убивающего слоя
	// @param layer - Слой
	// @return Было ли совершено пересечение
	bool IsIntersectDamage(const string& layer);

	// @brief Пересечение с объектом столкновения только по полу
	// @param layer - Слой
	// @param entity - Объект
	// @return Было ли совершено пересечение
	bool IsIntersectCollisionUp(const string& layer, const shared_ptr<Entity>& entity);

	// @brief Пересечение с объектом лестницы
	// @param entity - Объект
	// @param direction - Движение
	// @return Было ли совершено пересечение
	bool IsIntersectStair(const shared_ptr<Entity>& entity, const string& direction);

	// @brief Пересечение с объектом передвигающаяся платформа
	// @param entity - Объект
	// @return Было ли совершено пересечение
	bool IsIntersectMovingPlatform(const shared_ptr<Entity>& entity);

	// @brief Пересечение с объектом передвигающаяся платформа, если она направляется вниз
	// @param entity - Объект
	// @return Было ли совершено пересечение
	bool IsIntersectMovingPlatformBugFix(const shared_ptr<Entity>& entity);

	// @brief Пересечение с объектом угловое столкновение, вверх находится слева
	// @param entity - Объект
	// @return Было ли совершено пересечение
	bool IsIntersectCollisionAngleLeft(const shared_ptr<Entity>& entity);

	// @brief Пересечение с объектом угловое столкновение, вверх находится справа
	// @param entity - Объект
	// @return Было ли совершено пересечение
	bool IsIntersectCollisionAngleRight(const shared_ptr<Entity>& entity);

	// @brief Было ли пересечение бота с игроком
	// @param entity - Объект
	// @return Было ли совершено пересечение
	bool IsPlayerIntersectBot(const shared_ptr<Entity>& entity);

	// @brief Было ли пересечение телепорта с игроком
	// @param entity - Объект
	// @return Было ли совершено пересечение
	bool IsPlayerTeleport(const shared_ptr<Entity>& entity);

	// @brief Было ли пересечение игрока с дружественным ботом, для запуска диалога
	// @param entity - Объект
	// @return Было ли совершено пересечение
	bool IsPlayerDialog(const shared_ptr<Entity>& entity);

	// @brief Пересечение с бонусами.
	bool IsPlayerBonus(const string& layer, const shared_ptr<Entity>& entity);

	// @brief Урон от падения
	// @return Было ли совершено действие
	bool IsFallDamage();

	// @brief Пересечение с объектом скрипта
	// @param script - Объект скрипта
	void Scripts(Entity* script);

	// @brief Слои
	Layers* m_Layers;
};

#endif /// PHYSIX_INCLUDE