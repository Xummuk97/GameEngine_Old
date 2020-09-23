///==========================================================================
///	Модуль:		Телепорт
///	Автор:		Андрей Гребенщиков
///	Год:		2019
///==========================================================================

#ifndef TELEPORT_INCLUDE
#define TELEPORT_INCLUDE

#include "messages.h"
#include "physix.h"

// @brief Телепорт
class Teleport
	: public Physix
{
public:
	// @brief Конструктор
	Teleport() = default;

	// @brief Деструктор
	~Teleport() = default;

	// @brief Обновление
	void Update();

	// @brief Пересечения
	// @param coord - По какой оси пересекаться (X или Y)
	void Intersects(char coord = 'y');
};

#endif /// TELEPORT_INCLUDE