///==========================================================================
///	Модуль:		Меню на базе TGUI
///	Автор:		Андрей Гребенщиков
///	Год:		2019
///==========================================================================

#ifndef MENU_INCLUDE
#define MENU_INCLUDE

#include <TGUI/TGUI.hpp>
#include <SFML/System.hpp>
#include <iostream>

using namespace std;
using namespace sf;

// @brief Тип меню
enum class MenuType
{
	Undefined,
	Main,
	About,
	Pause
};

// @brief Enum типа меню в String
String any_menu(const MenuType& type);

class Menu
{
public:
	using Delegate = function<void (Menu* menu, const string& id)>;

	Menu();
	~Menu() = default;

	// Включить/выключить меню
	void SetEnable(bool isEnable);

	// Возвратить меню
	tgui::ChildWindow::Ptr Get();

	// Событие клика
	void OnClick(Delegate func);

protected:
	Delegate onClick;
	tgui::ChildWindow::Ptr window;
};

// Главное меню
class MenuMain : public Menu
{
public:
	MenuMain();
	virtual ~MenuMain() = default;
};

// Меню паузы
class MenuPause : public Menu
{
public:
	MenuPause();
	virtual ~MenuPause() = default;
};

// Меню о проекте
class MenuAbout : public Menu
{
public:
	MenuAbout();
	virtual ~MenuAbout() = default;
};

class MenuManagament
{
public:
	MenuManagament();
	~MenuManagament() = default;

	// Показать меню
	void Display(const MenuType& type);

	// Поместить виджеты по вертикали
	static void WidgetsToVerticale(const vector<tgui::Widget::Ptr>& widgets, float firstX, float firstY, float intervalY);

private:
	MenuType currentMenu;
	map<MenuType, shared_ptr<Menu>> data;
};

#endif /// MENU_INCLUDE