///==========================================================================
///	Модуль:		Ядро
///	Автор:		Андрей Гребенщиков
///	Год:		2019
///==========================================================================

#ifndef CORE_INCLUDE
#define CORE_INCLUDE

#include "system.h"
#include "level.h"
#include "light.h"
#include "Settings.h"
#include "..\gui\widgets\image.h"
#include "..\entity\weapon.h"

// @brief Ядро
class Core
{
public:
	// @brief Конструктор
	Core();

	// @brief Деструктор
	~Core() = default;

	// @brief Запуск приложения
	void Run();

	// @brief Переход на следующий уровень
	void NextLevel();

private:
	// @brief Создание базового окна
	void CreateBaseWindow();

	// @brief Остальные настройки
	void OtherInit();

	// @brief Поток игрового процесса
	void ThreadProcess();

	// @brief Счётчик уровней	
	size_t m_LevelsScore = 0;																			

	// @brief Фон
	Widget::Image* m_BackGround;	

	// @brief Курсор
	Widget::Image* m_Cursor;																	

	// @brief Контейнер над светом
	Light* m_Light;																				

	// @brief Текущий уровень	
	Level* m_Level;																						

	// @brief Поток
	Thread* m_thread;

	// @brief Шаблоны оружия
	map<string, shared_ptr<Weapon>> m_templatesWeapons;											

	// @brief Часы		
	Clock m_Clock;																					

	// @brief События		
	Event m_Event;
};

#endif /// CORE_INCLUDE