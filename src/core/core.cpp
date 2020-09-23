///==========================================================================
///	Модуль:		Ядро
///	Автор:		Андрей Гребенщиков
///	Год:		2019
///==========================================================================

#include "core.h"

Core::Core()
{
	// Инициализация шаблонов
	Settings::InitTextures();
	Settings::InitFonts();
	Settings::InitTemplates();
	Settings::InitAnimations();
	Settings::InitLevels();
	Settings::InitWeapons(std::move(m_templatesWeapons));
	Settings::InitKeyBoard();
	Messages::Init();

	// Первоначальная настройка игры
	CreateBaseWindow();
	OtherInit();

	// Переход на первый уровень
	NextLevel();
}

void Core::NextLevel()
{
	// Очистка интерфейса от виджетов
	auto widgets = &System::m_Gui->getWidgets();
	for (auto it = widgets->begin(); it != widgets->end();)
	{
		if (!(System::m_Gui->getWidgetName((*it)).find("gui_") != string::npos))
		{
			System::m_Gui->remove(*it);
		}
		else
			it++;
	}


	// Очистка функций при обновлении виджетов
	System::m_WidgetsUpdate.clear();

	// Если массив света не пуст, удалить его содержимое
	if (!m_Light->IsEmpty()) m_Light->Reset();

	// Установка цвета глобального освещения
	System::m_LightSystem._ambientColor = System::m_Levels[m_LevelsScore]->colorAmbient;

	// Установка области света для игрока
	shared_ptr<ltbl::LightPointEmission> lightPlayer = m_Light->Get("light_player");

	if (lightPlayer)
		lightPlayer->_emissionSprite.setColor(System::m_Levels[m_LevelsScore]->colorPlayer);
	else
	{
		lightPlayer = std::make_shared<ltbl::LightPointEmission>();
		lightPlayer->_emissionSprite.setScale(sf::Vector2f(15, 15));
		lightPlayer->_sourceRadius = 10;
		lightPlayer->_shadowOverExtendMultiplier = 1;
		lightPlayer->_emissionSprite.setColor(System::m_Levels[m_LevelsScore]->colorPlayer);
		m_Light->Add("point", "light_player", std::move(lightPlayer));
	}

	// Установка фона
	m_BackGround->GetSprite()->setTexture(System::m_Textures[System::m_Levels[m_LevelsScore]->textureName]);
	m_BackGround->GetSprite()->setTextureRect(System::m_Levels[m_LevelsScore]->rect);
	
	// Если уровень создан, удалить
	if (m_Level) delete m_Level;

	// Переход на новый уровень
	m_Level = new Level(System::m_Levels[m_LevelsScore]->mapName, m_Light, &m_templatesWeapons);

	// Пока не закончились карты, инкрементируем счётчик уровней
	if (m_LevelsScore + 1 < System::m_Levels.size()) m_LevelsScore++;

	// Динамическая инициализация
	Settings::InitScripts(std::move(*m_Level));
	Settings::InitDialogs(std::move(*m_Level));

	Settings::InitGui(std::move(*m_Level));
	System::m_Dialogs->SetGui(System::m_Gui);
}

void Core::CreateBaseWindow()
{
	// Нахождение разрешение экрана
	System::m_Permission.x = (float)VideoMode::getDesktopMode().width;
	System::m_Permission.y = (float)VideoMode::getDesktopMode().height;

	// Создание и инициализация окна
	System::m_Window = new RenderWindow({ (unsigned)System::m_Permission.x, (unsigned)System::m_Permission.y }, "Platformer");
	System::m_Window->setFramerateLimit(60);
	System::m_Window->setVerticalSyncEnabled(false);
	System::m_Window->setMouseCursorVisible(false);

	// Установка камеры
	System::m_Camera = System::m_Window->getDefaultView();
}

void Core::OtherInit()
{
	// Инициализация фона
	m_BackGround = new Widget::Image(Sprite(), FloatRect(0, 0, System::m_Permission.x, System::m_Permission.y), Vector2f(0.0f, 0.0f));

	// Инициализация курсора мыши
	m_Cursor = new Widget::Image(Sprite(), FloatRect(0, 0, System::m_Permission.x, System::m_Permission.y), Vector2f(0.0f, 0.0f));
	Settings::InitCursor(m_Cursor);

	// Инициализация света
	m_Light = new Light;

	// Инициализация интерфейса
	System::m_Gui = new tgui::Gui{ *System::m_Window };

	// Инициализация диалогов
	System::m_Dialogs = new Dialog::Core();

	// Убирать паузу, когда вышел из меню диалогов
	System::m_Dialogs->OnClose([&](Dialog::Core* core)
	{ 
		System::m_isPause = false;
	});

	// Инициализация потока
	m_thread = new Thread(&Core::ThreadProcess, this);

	// Инициализация эффектов
	// Blur (Размытие)
	System::m_Effects["blur"] = new Effects::Blur();
	// Color (Установка цвета)
	System::m_Effects["color"] = new Effects::Color();
}

void Core::Run()
{
	// Инициазация меню
	System::m_Menu = new MenuManagament();

	System::m_Window->setActive(false);
	m_thread->launch();

	while (System::m_Window->isOpen())
	{
		while (System::m_Window->pollEvent(m_Event))
		{
			if (m_Event.type == Event::Closed) 
			{
				//System::m_Window->close();
				exit(0);
			}

			if (m_Event.type == Event::KeyPressed)
			{
				switch (m_Event.key.code)
				{
				case Keyboard::Escape:
					if (!System::m_isPause)
						System::m_Menu->Display(MenuType::Pause);
					break;
				}
			}

			System::m_Gui->handleEvent(m_Event);
		}

		sleep(milliseconds(60));
	}
}

void Core::ThreadProcess()
{
	while (System::m_Window->isOpen())
	{
		// Нахождение прошедшего времени
		System::m_Time = m_Clock.getElapsedTime().asSeconds();
		m_Clock.restart();

		if (System::m_Time > 0.5f) System::m_Time = 0.0f;

		auto player = m_Level->GetEntityByType("player");

		// Обновление
		if (!System::m_isPause)
		{
			Messages::Reset();
			m_Level->Update();
		}
		System::DelegateWidgetUpdate();
		m_BackGround->Update();

		// Переместить положение света игрока на позицию игрока
		m_Light->Get("light_player")->_emissionSprite.setPosition(*player);
		
		System::m_Window->setView(System::m_Camera);

		// Рисование
		System::m_Window->clear();
		
		m_BackGround->Draw();
		m_Level->Draw();
		if (!System::m_isPause || System::m_IsRunGame)
			m_Light->Draw();
		Messages::Run();
		System::m_Gui->draw();

		System::LocationMouse();

		m_Cursor->SetPos(System::m_MousePosition);
		m_Cursor->Update();
		m_Cursor->Draw();
		
		System::m_Window->display();
		
		// Если игрок не жив, закончить игру
		if (!player->GetProperty<bool>("isLife")) System::m_Window->close();;

		// Переход на следующий уровень
		if (m_Level->IsNextLevel()) NextLevel();
	}
}
