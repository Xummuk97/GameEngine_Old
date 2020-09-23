///==========================================================================
///	Модуль:		Структура уровня
///	Автор:		Андрей Гребенщиков
///	Год:		2019
///==========================================================================

#ifndef STRUCT_LEVEL_INCLUDE
#define STRUCT_LEVEL_INCLUDE

#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

// @brief Структура уровня
struct sLevel
{
	// @brief Цвет глобальной тени
	Color colorAmbient;

	// @brief Цвет света игрока
	Color colorPlayer;

	// @brief Область текстуры фона
	IntRect rect;

	// @brief Название карты (с расширением .tmx)
	string mapName;
		
	// @brief Название текстуры фона
	string textureName;
};

#endif /// STRUCT_LEVEL_INCLUDE