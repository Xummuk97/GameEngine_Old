///==========================================================================
///	������:		��������� ��������
///	�����:		������ �����������
///	���:		2019
///==========================================================================

#include "Settings.h"
#include "../gui/dialogs.h"
#include "system.h"

void Settings::InitGui(Level&& level)
{
	// ����� ��������� ��������� (Tooltip)
	tgui::ToolTip::setTimeToDisplay(sf::milliseconds(0));
	
	auto hpPlayer = tgui::Label::create();
	auto renderer =	hpPlayer->getRenderer();
	renderer->setTextColor(tgui::Color::White);

	System::m_WidgetsUpdate["label_hpPlayer"] = [&](tgui::Widget::Ptr widget) 
	{
		widget->setVisible(!System::m_isPause);

		if (level.GetEntityByType("player") != nullptr)
			widget->cast<tgui::Label>()->setText(System::GetFormat(L"��������: {0}", level.GetEntityByType("player")->GetProperty<int>("hp")));
	};

	System::m_Gui->add(hpPlayer, "label_hpPlayer");
}

void Settings::InitScripts(Level&& level)
{
	// ������������� ������� ������
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

	// ������������� ������� �����
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
		dialog->SetText(L"�������� ������!");

		dialog->AddOption(Dialog::Option::Create(L"������� � ������ ������", [](Dialog::Core* core)
		{
			core->SetCurrent("dialog_new");
			core->Create();
		}));

		dialog->AddOption(Dialog::Option::Create(L"�����", [&](Dialog::Core* core)
		{
			core->Close();
		}));

		System::m_Dialogs->Add("dialog_A", std::move(dialog));
	}

	// 2.
	{
		auto dialog = Dialog::Base::Create();
		dialog->SetText(L"����� �������� ������");

		dialog->AddOption(Dialog::Option::Create(L"�����", [](Dialog::Core* core)
		{
			core->Close();
		}));


		System::m_Dialogs->Add("dialog_new", std::move(dialog));
	}
}

void Settings::InitCursor(Widget::Image* cursor)
{
	// �������� ������� �������� ��� ������� � �������
	const auto& rect = System::StringArrayToArray<int>(System::m_Templates["cursor"]["rect"]);

	// �������������� ������
	auto spriteCursor = cursor->GetSprite();
	spriteCursor->setTexture(System::m_Textures[System::m_Templates["cursor"]["textureName"]]);
	spriteCursor->setTextureRect({ rect[0], rect[1], rect[2], rect[3] });
	spriteCursor->setOrigin({ rect[2] / 2.0f, rect[3] / 2.0f });

	// ������ ������� ���������� (� (0,0) ����������)
	cursor->SetAbsolute(true);
}

void Settings::InitKeyBoard()
{
	// �������������� ������ XML ���������
	TiXmlDocument doc;
	doc.LoadFile((PATH_XMLS + "keyboard.xml").c_str());

	// ������� ������� - Game
	const auto& root = doc.FirstChildElement();

	// ���������� �� ���� ��������� - Setting
	for (auto element = root->FirstChildElement(); element; element = element->NextSiblingElement())
	{
		// ��������� ������ � ���������� ������ �������� ����������
		System::m_KeyBoard[XML_TO_STRING(element, "name")] = System::GetKeyBoard(XML_TO_STRING(element, "value"));
	}
}

void Settings::InitTextures()
{
	// �������������� ������ XML ���������
	TiXmlDocument doc;
	doc.LoadFile((PATH_XMLS + "textures.xml").c_str());

	// ������� ������� - Textures
	const auto& root = doc.FirstChildElement();

	// ���������� �� ���� ��������� - texture
	for (auto element = root->FirstChildElement(); element; element = element->NextSiblingElement())
	{
		// ������ ������ Texture, �������������� ���
		Texture texture;
		texture.loadFromFile(PATH_TEXTURES + XML_TO_STRING(element, "file"));
		texture.setRepeated(true);
		texture.setSmooth(true);

		// ��������� ������ � ���������� ������ �������
		System::m_Textures[XML_TO_STRING(element, "class")] = texture;
	}
}

void Settings::InitFonts()
{
	// �������������� ������ XML ���������
	TiXmlDocument doc;
	doc.LoadFile((PATH_XMLS + "fonts.xml").c_str());

	// ������� ������� - Fonts
	const auto& root = doc.FirstChildElement();

	// ���������� �� ���� ��������� - Font
	for (auto element = root->FirstChildElement(); element; element = element->NextSiblingElement())
	{
		// ������ ������ Font, �������������� ���
		Font font;
		font.loadFromFile(PATH_FONTS + XML_TO_STRING(element, "file"));

		// ��������� ������ � ���������� ������ �������
		System::m_Fonts[XML_TO_STRING(element, "name")] = font;
	}
}

void Settings::InitTemplates()
{
	// �������������� ������ XML ���������
	TiXmlDocument doc;
	doc.LoadFile((PATH_XMLS + "templates.xml").c_str());

	// ������� ������� - Templates
	const auto& root = doc.FirstChildElement();

	// ���������� �� ���� ��������� - Template
	for (auto element = root->FirstChildElement(); element; element = element->NextSiblingElement())
	{
		// ������ �������
		map<string, string> properties;

		// ���������� �� ���� ���������
		for (auto subElement = element->FirstChildElement(); subElement; subElement = subElement->NextSiblingElement())
		{
			properties[XML_TO_STRING(subElement, "name")] = XML_TO_STRING(subElement, "value");
		}

		// ��������� ������ � ���������� ������ ��������
		System::m_Templates[XML_TO_STRING(element, "class")] = properties;
	}
}

void Settings::InitWeapons(map<string, shared_ptr<Weapon>>&& templatesWeapons)
{
	// �������������� ������ XML ���������
	TiXmlDocument doc;
	doc.LoadFile((PATH_XMLS + "weapons.xml").c_str());

	// ������� ������� - Weapons
	const auto& root = doc.FirstChildElement();

	// ���������� �� ���� ��������� - Weapon
	for (auto element = root->FirstChildElement(); element; element = element->NextSiblingElement())
	{
		// ������ �������
		shared_ptr<Weapon> weapon = make_shared<Weapon>();

		map<string, string> properties;

		// ���������� �� ���� ���������
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

		// ��������� ������ � ���������� ������ ��������
		templatesWeapons[XML_TO_STRING(element, "name")] = weapon;
	}
}

void Settings::InitAnimations()
{
	// �������������� ������ XML ���������
	TiXmlDocument doc;
	doc.LoadFile((PATH_XMLS + "animations.xml").c_str());

	// ������� ������� - Animations
	const auto& root = doc.FirstChildElement();

	// ���������� �� ���� ��������� - Set
	for (auto set = root->FirstChildElement(); set; set = set->NextSiblingElement())
	{
		// ������������� ���������� � Set ��������
		const string	&className = XML_TO_STRING(set, "class"),
						&_default = XML_TO_STRING(set, "default");

		// �������� ������� ��������
		shared_ptr<Animations> animations = make_shared<Animations>();

		// ���������� �� ���� ��������� - Animation
		for (auto _animation = set->FirstChildElement(); _animation; _animation = _animation->NextSiblingElement())
		{
			// ������������� ���������� � Animation
			const string& name = XML_TO_STRING(_animation, "name");
			const float& timeNextFrame = XML_TO_FLOAT(_animation, "timeNextFrame");

			// �������� ������� ����� ��������
			shared_ptr<Animation> animFrame = make_shared<Animation>(timeNextFrame);

			// ���������� �� ���� ��������� - Frame
			for (auto frame = _animation->FirstChildElement(); frame; frame = frame->NextSiblingElement())
			{
				// ��������� �������� �������� � ������ ����� ��������
				const auto& rectVec = System::StringArrayToArray<int>(XML_TO_STRING(frame, "rect"));
				animFrame->Add(make_shared<IntRect>(rectVec[0], rectVec[1], rectVec[2], rectVec[3]));
			}

			// ��������� ���� � ��������
			animations->Add(name, move(animFrame));
		}

		// ������������� ������� ��������
		animations->SetCurrentAnimation(_default);

		// ��������� ������ � ���������� ������ ��������
		System::m_Animations[className] = animations;
	}
}

void Settings::InitLevels()
{
	// �������������� ������ XML ���������
	TiXmlDocument doc;
	doc.LoadFile((PATH_XMLS + "levels.xml").c_str());

	// ������� ������� �������
	const auto& root = doc.FirstChildElement();

	// ���������� �� ���� ���������
	for (auto lvl = root->FirstChildElement(); lvl; lvl = lvl->NextSiblingElement())
	{
		// �������� ������� ��������� �������
		shared_ptr<sLevel> addLvl = make_shared<sLevel>();

		// �������� �����
		addLvl->mapName = lvl->Attribute("map");
		addLvl->mapName += ".tmx";

		// ��������� � ���������� ��������
		auto element = lvl->FirstChildElement();

		// �������� ����
		addLvl->textureName = element->Attribute("textureName");

		// ������� ����
		auto bg_rect = System::StringArrayToArray<int>(element->Attribute("rect"));
		addLvl->rect = IntRect(bg_rect[0], bg_rect[1], bg_rect[2], bg_rect[3]);

		// ��������� � ���������� ��������
		element = element->NextSiblingElement();

		// ���� ���� (RGBA)
		auto ambient_color = System::StringArrayToArray<int>(element->Attribute("color"));
		addLvl->colorAmbient = Color((sf::Uint8)ambient_color[0], (sf::Uint8)ambient_color[1], (sf::Uint8)ambient_color[2], (sf::Uint8)ambient_color[3]);

		// ��������� � ���������� ��������
		element = element->NextSiblingElement();

		// ���� ����� ������ (RGBA)
		auto ambient_color2 = System::StringArrayToArray<int>(element->Attribute("color"));
		addLvl->colorPlayer = Color((sf::Uint8)ambient_color2[0], (sf::Uint8)ambient_color2[1], (sf::Uint8)ambient_color2[2], (sf::Uint8)ambient_color2[3]);

		// ��������� ������ � ���������� ������ �������
		System::m_Levels.push_back(addLvl);
	}
}
