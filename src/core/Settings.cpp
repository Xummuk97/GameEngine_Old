///==========================================================================
///	Модуль:		Настройка шаблонов
///	Автор:		Андрей Гребенщиков
///	Год:		2019
///==========================================================================

#include "Settings.h"
#include "../gui/dialogs.h"
#include "system.h"

void Settings::InitGui(Level&& level)
{
	// Время появления подсказок (Tooltip)
	tgui::ToolTip::setTimeToDisplay(sf::milliseconds(0));
	
	auto hpPlayer = tgui::Label::create();
	auto renderer =	hpPlayer->getRenderer();
	renderer->setTextColor(tgui::Color::White);

	System::m_WidgetsUpdate["label_hpPlayer"] = [&](tgui::Widget::Ptr widget) 
	{
		widget->setVisible(!System::m_isPause);

		if (level.GetEntityByType("player") != nullptr)
			widget->cast<tgui::Label>()->setText(System::GetFormat(L"Здоровья: {0}", level.GetEntityByType("player")->GetProperty<int>("hp")));
	};

	System::m_Gui->add(hpPlayer, "label_hpPlayer");
}

void Settings::InitScripts(Level&& level)
{
	// Устанавливаем скрипты игроку
	level.GetEntityByType("player")->SetScripts([&](Entity* entity, string scriptName)
	{
		if (scriptName == "1")
		{
			cout << "Sergey petuch!" << endl;
			return true;
		}
		else if (scriptName == "2")
		{
			cout << "Andrey krasavchik!" << endl;
			return true;
		}

		return false;
	});

	// Устанавливаем скрипты ботам
	for (auto& bot : level.GetEntitiesByType("bot"))
	{
		bot->SetScripts([&](Entity* entity, string scriptName)
		{
			if (scriptName == "bot1")
			{
				cout << "BOT1" << endl;
				return true;
			}

			return false;
		});
	}
}

void Settings::InitDialogs(Level&& level)
{
	{
		auto dialog = Dialog::Base::Create();
		dialog->SetText(L"Тестовый диалог!");

		dialog->AddOption(Dialog::Option::Create(L"Перейти в другой диалог", [](Dialog::Core* core)
		{
			core->SetCurrent("dialog_new");
			core->Create();
		}));

		dialog->AddOption(Dialog::Option::Create(L"Выйти", [&](Dialog::Core* core)
		{
			core->Close();
		}));

		System::m_Dialogs->Add("dialog_A", std::move(dialog));
	}

	// 2.
	{
		auto dialog = Dialog::Base::Create();
		dialog->SetText(L"Новый тестовый диалог");

		dialog->AddOption(Dialog::Option::Create(L"Выход", [](Dialog::Core* core)
		{
			core->Close();
		}));


		System::m_Dialogs->Add("dialog_new", std::move(dialog));
	}
}

void Settings::InitCursor(Widget::Image* cursor)
{
	// Получаем область картинки для курсора с шаблона
	const auto& rect = System::StringArrayToArray<int>(System::m_Templates["cursor"]["rect"]);

	// Инициализируем спрайт
	auto spriteCursor = cursor->GetSprite();
	spriteCursor->setTexture(System::m_Textures[System::m_Templates["cursor"]["textureName"]]);
	spriteCursor->setTextureRect({ rect[0], rect[1], rect[2], rect[3] });
	spriteCursor->setOrigin({ rect[2] / 2.0f, rect[3] / 2.0f });

	// Делаем позицию абсолютной (с (0,0) координаты)
	cursor->SetAbsolute(true);
}

void Settings::InitKeyBoard()
{
	// Инициализируем объект XML документа
	TiXmlDocument doc;
	doc.LoadFile((PATH_XMLS + "keyboard.xml").c_str());

	// Главный элемент - Game
	const auto& root = doc.FirstChildElement();

	// Проходимся по всем элементам - Setting
	for (auto element = root->FirstChildElement(); element; element = element->NextSiblingElement())
	{
		// Добавляем объект в глобальный массив настроек клавиатуры
		System::m_KeyBoard[XML_TO_STRING(element, "name")] = System::GetKeyBoard(XML_TO_STRING(element, "value"));
	}
}

void Settings::InitTextures()
{
	// Инициализируем объект XML документа
	TiXmlDocument doc;
	doc.LoadFile((PATH_XMLS + "textures.xml").c_str());

	// Главный элемент - Textures
	const auto& root = doc.FirstChildElement();

	// Проходимся по всем элементам - texture
	for (auto element = root->FirstChildElement(); element; element = element->NextSiblingElement())
	{
		// Создаём объект Texture, инициализируем его
		Texture texture;
		texture.loadFromFile(PATH_TEXTURES + XML_TO_STRING(element, "file"));
		texture.setRepeated(true);
		texture.setSmooth(true);

		// Добавляем объект в глобальный массив текстур
		System::m_Textures[XML_TO_STRING(element, "class")] = texture;
	}
}

void Settings::InitFonts()
{
	// Инициализируем объект XML документа
	TiXmlDocument doc;
	doc.LoadFile((PATH_XMLS + "fonts.xml").c_str());

	// Главный элемент - Fonts
	const auto& root = doc.FirstChildElement();

	// Проходимся по всем элементам - Font
	for (auto element = root->FirstChildElement(); element; element = element->NextSiblingElement())
	{
		// Создаём объект Font, инициализируем его
		Font font;
		font.loadFromFile(PATH_FONTS + XML_TO_STRING(element, "file"));

		// Добавляем объект в глобальный массив шрифтов
		System::m_Fonts[XML_TO_STRING(element, "name")] = font;
	}
}

void Settings::InitTemplates()
{
	// Инициализируем объект XML документа
	TiXmlDocument doc;
	doc.LoadFile((PATH_XMLS + "templates.xml").c_str());

	// Главный элемент - Templates
	const auto& root = doc.FirstChildElement();

	// Проходимся по всем элементам - Template
	for (auto element = root->FirstChildElement(); element; element = element->NextSiblingElement())
	{
		// Объект свойств
		map<string, string> properties;

		// Проходимся по всем свойствам
		for (auto subElement = element->FirstChildElement(); subElement; subElement = subElement->NextSiblingElement())
		{
			properties[XML_TO_STRING(subElement, "name")] = XML_TO_STRING(subElement, "value");
		}

		// Добавляем объект в глобальный массив шаблонов
		System::m_Templates[XML_TO_STRING(element, "class")] = properties;
	}
}

void Settings::InitWeapons(map<string, shared_ptr<Weapon>>&& templatesWeapons)
{
	// Инициализируем объект XML документа
	TiXmlDocument doc;
	doc.LoadFile((PATH_XMLS + "weapons.xml").c_str());

	// Главный элемент - Weapons
	const auto& root = doc.FirstChildElement();

	// Проходимся по всем элементам - Weapon
	for (auto element = root->FirstChildElement(); element; element = element->NextSiblingElement())
	{
		// Объект свойств
		shared_ptr<Weapon> weapon = make_shared<Weapon>();

		map<string, string> properties;

		// Проходимся по всем свойствам
		for (auto subElement = element->FirstChildElement(); subElement; subElement = subElement->NextSiblingElement())
		{
			properties[XML_TO_STRING(subElement, "name")] = XML_TO_STRING(subElement, "value");
		}

		weapon->SetName(XML_TO_STRING(element, "name"));
		auto bullet = weapon->GetBullet();
		bullet->SetProperty("speed", stof(properties["speed"]));
		bullet->SetProperty("damage", stoi(properties["damage"]));
		bullet->SetProperty("delay", stof(properties["delay"]));
		bullet->SetProperty("offset", 0.0f);
		bullet->SetTexture(properties["bulletTexture"]);

		auto bulletRect = System::StringArrayToArray<int>(properties["bulletRect"]);
		bullet->SetRect(IntRect(bulletRect[0], bulletRect[1], bulletRect[2], bulletRect[3]));

		auto weaponRect = System::StringArrayToArray<int>(properties["weaponRect"]);
		weapon->SetImageSprite(properties["weaponTexture"], IntRect(weaponRect[0], weaponRect[1], weaponRect[2], weaponRect[3]));

		// Добавляем объект в глобальный массив шаблонов
		templatesWeapons[XML_TO_STRING(element, "name")] = weapon;
	}
}

void Settings::InitAnimations()
{
	// Инициализируем объект XML документа
	TiXmlDocument doc;
	doc.LoadFile((PATH_XMLS + "animations.xml").c_str());

	// Главный элемент - Animations
	const auto& root = doc.FirstChildElement();

	// Проходимся по всем элементам - Set
	for (auto set = root->FirstChildElement(); set; set = set->NextSiblingElement())
	{
		// Инициализация аттрибутов у Set элемента
		const string	&className = XML_TO_STRING(set, "class"),
						&_default = XML_TO_STRING(set, "default");

		// Создание объекта анимаций
		shared_ptr<Animations> animations = make_shared<Animations>();

		// Проходимся по всем элементам - Animation
		for (auto _animation = set->FirstChildElement(); _animation; _animation = _animation->NextSiblingElement())
		{
			// Инициализация аттрибутов у Animation
			const string& name = XML_TO_STRING(_animation, "name");
			const float& timeNextFrame = XML_TO_FLOAT(_animation, "timeNextFrame");

			// Создание объекта кадра анимации
			shared_ptr<Animation> animFrame = make_shared<Animation>(timeNextFrame);

			// Проходимся по всем элементам - Frame
			for (auto frame = _animation->FirstChildElement(); frame; frame = frame->NextSiblingElement())
			{
				// Добавляем значение элемента в объект кадра анимации
				const auto& rectVec = System::StringArrayToArray<int>(XML_TO_STRING(frame, "rect"));
				animFrame->Add(make_shared<IntRect>(rectVec[0], rectVec[1], rectVec[2], rectVec[3]));
			}

			// Добавляем кадр в анимацию
			animations->Add(name, move(animFrame));
		}

		// Устанавливаем текущую анимацию
		animations->SetCurrentAnimation(_default);

		// Добавляем объект в глобальный массив анимаций
		System::m_Animations[className] = animations;
	}
}

void Settings::InitLevels()
{
	// Инициализируем объект XML документа
	TiXmlDocument doc;
	doc.LoadFile((PATH_XMLS + "levels.xml").c_str());

	// Находим главный элемент
	const auto& root = doc.FirstChildElement();

	// Проходимся по всем элементам
	for (auto lvl = root->FirstChildElement(); lvl; lvl = lvl->NextSiblingElement())
	{
		// Экземляр объекта структуры уровень
		shared_ptr<sLevel> addLvl = make_shared<sLevel>();

		// Название карты
		addLvl->mapName = lvl->Attribute("map");
		addLvl->mapName += ".tmx";

		// Переходим к следующему элементу
		auto element = lvl->FirstChildElement();

		// Название фона
		addLvl->textureName = element->Attribute("textureName");

		// Область фона
		auto bg_rect = System::StringArrayToArray<int>(element->Attribute("rect"));
		addLvl->rect = IntRect(bg_rect[0], bg_rect[1], bg_rect[2], bg_rect[3]);

		// Переходим к следующему элементу
		element = element->NextSiblingElement();

		// Цвет фона (RGBA)
		auto ambient_color = System::StringArrayToArray<int>(element->Attribute("color"));
		addLvl->colorAmbient = Color((sf::Uint8)ambient_color[0], (sf::Uint8)ambient_color[1], (sf::Uint8)ambient_color[2], (sf::Uint8)ambient_color[3]);

		// Переходим к следующему элементу
		element = element->NextSiblingElement();

		// Цвет света игрока (RGBA)
		auto ambient_color2 = System::StringArrayToArray<int>(element->Attribute("color"));
		addLvl->colorPlayer = Color((sf::Uint8)ambient_color2[0], (sf::Uint8)ambient_color2[1], (sf::Uint8)ambient_color2[2], (sf::Uint8)ambient_color2[3]);

		// Добавляем объект в глобальный массив уровней
		System::m_Levels.push_back(addLvl);
	}
}
