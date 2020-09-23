///==========================================================================
///	Модуль:		Настройка шаблонов
///	Автор:		Андрей Гребенщиков
///	Год:		2019
///==========================================================================

#ifndef SETTINGS_INCLUDE
#define SETTINGS_INCLUDE

#include "system.h"
#include "animation.h"
#include "level.h"
#include "..\gui\widgets\image.h"
#include "..\entity\weapon.h"

// @brief Настройки шаблонов
class Settings 
{
public:
	// @brief Конструктор
	Settings() = default;
	
	// @brief Деструктор
	~Settings() = default;

	// @brief Инициализация текстур
	static void InitTextures();

	// @brief Инициализация шрифтов
	static void InitFonts();

	// @brief Инициализация шаблонов
	static void InitTemplates();

	// @brief Инициализация оружия
	// @param templatesWeapons - Шаблоны оружия
	static void InitWeapons(map<string, shared_ptr<Weapon>>&& templatesWeapons);

	// @brief Инииализация анимаций
	static void InitAnimations();

	// @brief Инициализация уровней
	static void InitLevels();

	// @brief Инициализация GUI
	// @param level - Объект уровня
	static void InitGui(Level&& level);

	// @brief Инициализация скриптов
	// @param level - Объект уровня
	static void InitScripts(Level&& level);

	// @brief Инициализация курсора
	// @param cursor - Объект курсора
	static void InitCursor(Widget::Image* cursor);

	// @brief Инициализация клавиатуры
	static void InitKeyBoard();

	// @brief Инициализация диалогов
	// @param level - Объект уровня
	static void InitDialogs(Level&& level);
};

#endif /// SETTINGS_INCLUDE