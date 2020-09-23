///==========================================================================
///	Модуль:		Меню на базе TGUI
///	Автор:		Андрей Гребенщиков
///	Год:		2019
///==========================================================================

#include "Menu.h"
#include "../core/system.h"

String any_menu(const MenuType& type)
{
	switch (type)
	{
	case MenuType::Main:
		return L"gui_main_menu";
		break;

	case MenuType::Pause:
		return L"gui_pause_menu";
		break;
	}

	return L"Меню не определенно!";
}

Menu::Menu()
{
	window = tgui::ChildWindow::create(L"", tgui::ChildWindow::TitleButton::None);
	window->getRenderer()->setBackgroundColor(tgui::Color(0, 0, 0, 150));
	window->getRenderer()->setTitleBarColor(tgui::Color(0, 0, 0, 0));
	window->setSize("100%", "100%");
	window->setEnabled(false);
	window->setVisible(false);
}

void Menu::SetEnable(bool isEnable)
{
	window->setEnabled(isEnable);
	window->setVisible(isEnable);
}

tgui::ChildWindow::Ptr Menu::Get()
{
	return window;
}

void Menu::OnClick(Delegate func)
{
	onClick = func;
}

MenuManagament::MenuManagament()
	: currentMenu(MenuType::Main)
{
	data[MenuType::Main] = make_shared<MenuMain>();

	data[MenuType::Main]->OnClick([&](Menu* menu, const string& id)
	{
		if (id == "exit")
			System::m_Window->close();
		else if (id == "about_the_project")
			Display(MenuType::About);
		else
			Display(MenuType::Undefined);
	});

	///

	data[MenuType::Pause] = make_shared<MenuPause>();

	data[MenuType::Pause]->OnClick([&](Menu* menu, const string& id)
	{
		if (id == "exit")
			System::m_Window->close();
		Display(MenuType::Undefined);
	});

	///

	data[MenuType::About] = make_shared<MenuAbout>();

	data[MenuType::About]->OnClick([&](Menu * menu, const string & id)
	{
		if (id == "back")
			Display(MenuType::Main);
	});

	///
	for (auto& [type, value] : data)
		System::m_Gui->add(value->Get(), any_menu(type).toAnsiString());

	data[currentMenu]->SetEnable(true);
}

void MenuManagament::Display(const MenuType& type)
{
	if (type == MenuType::Undefined)
	{
		data[currentMenu]->SetEnable(false);
		currentMenu = type;
		return;
	}

	System::m_isPause = true;

	if (currentMenu == type)
		return;

	if (currentMenu != MenuType::Undefined)
		data[currentMenu]->SetEnable(false);
	data[type]->SetEnable(true);

	currentMenu = type;
}

void MenuManagament::WidgetsToVerticale(const vector<tgui::Widget::Ptr>& widgets, float firstX, float firstY, float intervalY)
{
	for (auto widget = widgets.begin(); widget != widgets.end(); widget++)
	{
		if (widget == widgets.begin())
			(*widget)->setPosition("(parent.size - size) / 2", firstY);
		else
		{
			auto pre = *(widget - 1);

			auto pos = pre->getPosition();

			(*widget)->setPosition("(parent.size - size) / 2", pre->getSize().y + pos.y + intervalY);
			//(*widget)->setPosition("(parent.size - size) / 2");
		}
	}
}

MenuMain::MenuMain()
{
	System::m_isPause = true;

	tgui::Label::Ptr labelTitle = tgui::Label::create(L"Игровой движок (beta)");
	tgui::Button::Ptr buttonNewGame = tgui::Button::create(L"Начать игру");
	//tgui::Button::Ptr buttonLoadGame = tgui::Button::create(L"Загрузить игру");
	//tgui::Button::Ptr buttonSettings = tgui::Button::create(L"Настройки");
	tgui::Button::Ptr buttonAboutTheProject = tgui::Button::create(L"О проекте");
	tgui::Button::Ptr buttonExit = tgui::Button::create(L"Выход");

	auto labelTitleRenderer = labelTitle->getRenderer();
	labelTitleRenderer->setTextColor(tgui::Color::White);
	labelTitle->setTextSize(42);

	buttonNewGame->setSize("20%", 50.0);
	buttonAboutTheProject->setSize("20%", 50.0);
	buttonExit->setSize("20%", 50.0);

	buttonNewGame->connect("pressed", [&]()
	{
		onClick(this, "new_game");
		System::m_isPause = false;
		System::m_IsRunGame = true;
	});

	/*buttonLoadGame->connect("pressed", [&]()
	{
		onClick(this, "load_game");
		System::m_isPause = false;
	});

	buttonSettings->connect("pressed", [&]()
	{
		onClick(this, "settings");
		System::m_isPause = false;
	});*/

	buttonAboutTheProject->connect("pressed", [&]()
	{
		onClick(this, "about_the_project");
		//System::m_isPause = false;
	});

	buttonExit->connect("pressed", [&]()
	{
		onClick(this, "exit");
		System::m_isPause = false;
	});

	window->add(labelTitle, "main_title");
	//tgui::Group::Ptr group = tgui::Group::create({ "40%", "100%" });
	window->add(buttonNewGame, "main_new_game");
	/*group->add(buttonLoadGame, "load_game");
	group->add(buttonSettings, "settings");*/
	window->add(buttonAboutTheProject, "main_about_the_project");
	window->add(buttonExit, "main_exit");

	MenuManagament::WidgetsToVerticale(window->getWidgets(), 50.0f, 50.0f, 30.0f);

	//window->add(group, "group");
}

MenuPause::MenuPause()
{
	System::m_isPause = true;

	tgui::Label::Ptr labelTitle = tgui::Label::create(L"Меню паузы");
	tgui::Button::Ptr buttonResume = tgui::Button::create(L"Продолжить");
	tgui::Button::Ptr buttonExit = tgui::Button::create(L"Выход");

	auto labelTitleRenderer = labelTitle->getRenderer();
	labelTitleRenderer->setTextColor(tgui::Color::White);
	labelTitle->setTextSize(42);

	buttonResume->setSize("20%", 50.0);
	buttonExit->setSize("20%", 50.0);

	buttonResume->connect("pressed", [&]()
	{
		onClick(this, "resume");
		System::m_isPause = false;
	});

	buttonExit->connect("pressed", [&]()
	{
		onClick(this, "exit");
		System::m_isPause = false;
	});

	//tgui::Group::Ptr group = tgui::Group::create({ "40%", "100%" });
	window->add(labelTitle, "pause_title");
	window->add(buttonResume, "pause_resume");
	window->add(buttonExit, "pause_exit");

	MenuManagament::WidgetsToVerticale(window->getWidgets(), 50.0f, 50.0f, 30.0f);

	//window->add(group, "group");
}

MenuAbout::MenuAbout()
{
	System::m_isPause = true;

	tgui::Label::Ptr labelTitle = tgui::Label::create(L"Меню паузы");
	tgui::Label::Ptr labelText = tgui::Label::create(System::GetFormat(L"Клавищи: \nВверх: {0} \nВниз: {1} \nВлево: {2} \nВправо: {3} \nПрыжок: {4} \nДействия: {5} \nОбзор: мышь \nСтрельба: ЛКМ \n\nАвтор: Андрей Гребенщиков, Техникум Автоматика.", GetStrKeyBoard("up"), GetStrKeyBoard("down"), GetStrKeyBoard("left"), GetStrKeyBoard("right"), GetStrKeyBoard("jump"), GetStrKeyBoard("action")));
	tgui::Button::Ptr buttonBack = tgui::Button::create(L"Назад");

	buttonBack->setSize("20%", 50.0);

	auto labelTitleRenderer = labelTitle->getRenderer();
	labelTitleRenderer->setTextColor(tgui::Color::White);
	labelTitle->setTextSize(42);

	labelText->getRenderer()->setTextColor(tgui::Color::White);

	buttonBack->connect("pressed", [&]()
	{
		onClick(this, "back");
		//System::m_isPause = false;
	});

	window->add(labelTitle, "pause_title");
	window->add(labelText, "pause_text");
	window->add(buttonBack, "pause_back");

	MenuManagament::WidgetsToVerticale(window->getWidgets(), 50.0f, 50.0f, 30.0f);
}
