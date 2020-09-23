///==========================================================================
///	������:		���� �� ���� TGUI
///	�����:		������ �����������
///	���:		2019
///==========================================================================

#ifndef MENU_INCLUDE
#define MENU_INCLUDE

#include <TGUI/TGUI.hpp>
#include <SFML/System.hpp>
#include <iostream>

using namespace std;
using namespace sf;

// @brief ��� ����
enum class MenuType
{
	Undefined,
	Main,
	About,
	Pause
};

// @brief Enum ���� ���� � String
String any_menu(const MenuType& type);

class Menu
{
public:
	using Delegate = function<void (Menu* menu, const string& id)>;

	Menu();
	~Menu() = default;

	// ��������/��������� ����
	void SetEnable(bool isEnable);

	// ���������� ����
	tgui::ChildWindow::Ptr Get();

	// ������� �����
	void OnClick(Delegate func);

protected:
	Delegate onClick;
	tgui::ChildWindow::Ptr window;
};

// ������� ����
class MenuMain : public Menu
{
public:
	MenuMain();
	virtual ~MenuMain() = default;
};

// ���� �����
class MenuPause : public Menu
{
public:
	MenuPause();
	virtual ~MenuPause() = default;
};

// ���� � �������
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

	// �������� ����
	void Display(const MenuType& type);

	// ��������� ������� �� ���������
	static void WidgetsToVerticale(const vector<tgui::Widget::Ptr>& widgets, float firstX, float firstY, float intervalY);

private:
	MenuType currentMenu;
	map<MenuType, shared_ptr<Menu>> data;
};

#endif /// MENU_INCLUDE