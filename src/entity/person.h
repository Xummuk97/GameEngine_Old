///==========================================================================
///	Модуль:		Персона
///	Автор:		Андрей Гребенщиков
///	Год:		2019
///==========================================================================

#ifndef PERSON_INCLUDE
#define PERSON_INCLUDE

#include <SFML/System/String.hpp>
#include <SFML/Graphics/Color.hpp>

using namespace std;
using namespace sf;

// @brief Персона
struct Person
{
	// @brief Цвет имени
	Color nameColor;
	
	// @brief Цвет текста
	Color textColor;

	// @brief Имя
	String name;

	// @brief Конструктор
	Person()
	{}

	// @brief Конструктор с параметром
	// @param name - Имя 
	// @param nameColor - Цвет имени
	// @param textColor - Цвет текста
	Person(const String& name, const Color& nameColor, const Color& textColor) :
		name(name), nameColor(nameColor), textColor(textColor)
	{}
};

const Person PERSON_PLAYER = Person(L"Игрок", Color::White, Color::Black);

#endif /// PERSON_INCLUDE