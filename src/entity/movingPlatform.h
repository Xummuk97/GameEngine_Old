///==========================================================================
///	Модуль:		Передвигающаяся платформа
///	Автор:		Андрей Гребенщиков
///	Год:		2019
///==========================================================================

#ifndef MOVING_PLATFORM_INCLUDE
#define MOVING_PLATFORM_INCLUDE

#include "entity.h"

// @brief Передвигающаяся платформа
class MovingPlatform : public Entity
{
public:
	// @brief Конструктор
	MovingPlatform();

	// @brief Деструктор
	virtual ~MovingPlatform();

	// @brief Обновление
	void Update();

	// @brief Передвижение
	void DynamicMove();

	// @brief Пересечения
	// @param coord - По какой оси пересекаться (X или Y)
	void Intersects(char coord = 'y');

	// @brief Установка слоёв
	// @param layers - Слои
	void SetLayers(Layers* layers);

private:
	// @brief Слои
	Layers* m_Layers;
};

#endif /// MOVING_PLATFORM_INCLUDE