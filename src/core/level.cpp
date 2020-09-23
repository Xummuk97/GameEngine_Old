///==========================================================================
///	������:		�������
///	�����:		������ �����������
///	���:		2019
///==========================================================================

#include "level.h"
#include "../entity/teleport.h"

Level::Level(const string& file, Light* light, map<string, shared_ptr<Weapon>>* templatesWeapons) 
	: m_Light(light), 
	  m_TemplatesWeapons(templatesWeapons)
{
	// �������� XML �����
	TiXmlDocument Doc;

	/// ���������� �����
	if (!Doc.LoadFile((PATH_MAPS + file).c_str()))
	{
		std::cout << "Loading level \"" << file << "\" failed." << std::endl;
		return;
	}

	// ������������� Root ��������
	const auto& Root = Doc.FirstChildElement("map");

	// ������������� �������� �����
	const int& Width = XML_TO_INT(Root, "width"),
			 & Height = XML_TO_INT(Root, "height"),
			 & TileWidth = XML_TO_INT(Root, "tilewidth"),
			 & TileHeight = XML_TO_INT(Root, "tileheight");
	
	System::m_MapSize.x = (float)Width * (float)TileWidth;
	System::m_MapSize.y = (float)Height * (float)TileHeight;

	// ������������� ��������
	const auto& Tileset		= Root->FirstChildElement("tileset");
	const int&  FirstTileID	= XML_TO_INT(Tileset, "firstgid");

	// ������������� ���� �� �������� � ��������
	const auto& Image = Tileset->FirstChildElement("image");
	std::string tilesetFile	= XML_TO_STRING(Image, "source");

	// �������� ��������
	sf::Image Img;

	// ��� ����������� ������������� � Tiled Map Editor
	tilesetFile.erase(0, 3);
	tilesetFile = "resources/" + tilesetFile;

	// �������� ����������� �����
	if (!Img.loadFromFile(tilesetFile))
	{
		std::cout << "Failed to load tile sheet." << std::endl;
		//Img.createMaskFromColor(Color::White);
		return;
	}

	// ��������� �������� � ������� ������ �������
	Texture texture;
	//texture.loadFromFile(tilesetFile);
	texture.loadFromImage(Img);
	texture.setRepeated(true);
	texture.setSmooth(true);
	
	System::m_Textures["map"] = texture;

	// ������������� ���-�� ������� � �����
	const int& Columns = texture.getSize().x / TileWidth;
	const int& Rows	= texture.getSize().y / TileHeight;

	// ���������� ������� ������ � ��������
	std::vector<sf::Rect<int> > SubRects;

	for (int y = 0; y < Rows; y++)
		for (int x = 0; x < Columns; x++)
		{
			sf::Rect<int> rect;

			rect.top	= y * TileHeight;
			rect.height = TileHeight;
			rect.left	= x * TileWidth;
			rect.width	= TileWidth;

			SubRects.push_back(rect);
		}
	
	// ���������� �� ���� ��������� layer
	for (auto eLayer = Root->FirstChildElement("layer"); 
		eLayer; 
		eLayer = eLayer->NextSiblingElement("layer"))
	{
		// ���������� ����� ����
		const String& LayerName = XML_TO_STRING(eLayer, "name");

		// ���������� ����� � ��������� ����
		const std::string& StrMap = eLayer->FirstChildElement("data")->GetText();

		// ������� ������� �� �����
		int x = 0;
		int y = 0;

		/// ���������� CSV ������� �����

		// ���������� �� ��������� �����
		const size_t& SIZE = StrMap.size();
		for (size_t i = 0; i < SIZE; i++)
		{
			// ��������� ����� �� ������� � ���� ������
			std::string Index = "";

			do
			{
				Index += StrMap[i];
				i++;
			}
			while (StrMap[i] != ',' && i < SIZE);

			// ��������� ��������� ����� � �������
			const int& Id = stoi(Index);

			// ������� ����
			const int& SubRectToUse = Id - FirstTileID;

			// ���� �� ������
			if (SubRectToUse >= 0)
			{
				// ������� ������ � ���������� �������
				Entity entity;
				entity.SetTexture("map");
				entity.SetRect(SubRects[SubRectToUse]);
				entity.SetPosition(static_cast<float>(x * TileWidth), static_cast<float>(y * TileHeight));

				// ��� ���� ������� damage_box ���������� �������� ��������
				if (LayerName == "damage_box")
					entity.SetProperty("hp", stoi(System::m_Templates["damage_box"]["hp"]));

				if (System::m_Animations.find(LayerName) != System::m_Animations.end())
				{
					entity.SetAnimation(std::move(System::m_Animations[LayerName]));
				}

				m_Layers[LayerName].push_back(make_shared<Entity>(entity));
			}

			// ������ ������� �� �����
			x++;
			if (x >= Width)
			{
				x = 0;
				y++;
			}
		}
	}

	/// ���������� ��������
	// ���� ������ �������� ����������
	if (Root->FirstChildElement("objectgroup") != nullptr)
	{
		// ���������� �� ���� ������� ��������
		for (auto ObjectGroup = Root->FirstChildElement("objectgroup");
			ObjectGroup;
			ObjectGroup = ObjectGroup->NextSiblingElement("objectgroup"))
		{
			// ������� ��� ������
			const String& NameGroup = XML_TO_STRING(ObjectGroup, "name");

			// ���������� �� ���� �������� ������
			for (auto Object = ObjectGroup->FirstChildElement();
				Object;
				Object = Object->NextSiblingElement())
			{
				// ������� ���� light_block
				/*if (NameGroup == "light_block")
				{
					// �������� ��������� �����
					float x = XML_TO_INT(Object, "x"),
						y = XML_TO_INT(Object, "y"),
						pX = 0.0f, pY = 0.0f;
					bool trigger = false;
					auto polygon = Object->FirstChildElement();
					string points = polygon->Attribute("points");
					vector<Vector2f> vecPoints;

					string word;
					for (auto symbol : points)
					{
						if (symbol == ',' && !trigger)
						{
							trigger = true;
							pX = stoi(word);
							word = "";
						}
						else if (symbol == ' ' && trigger)
						{
							trigger = false;
							pY = stoi(word);
							word = "";
							vecPoints.push_back(Vector2f(pX, pY));
						}
						else
							word += symbol;
					}

					pY = stoi(word);
					word = "";
					vecPoints.push_back(Vector2f(pX, pY));

					//  ��������� ���������� � ������ �����
					std::shared_ptr<ltbl::LightShape> lightShape = std::make_shared<ltbl::LightShape>();

					const size_t SIZE = vecPoints.size();
					lightShape->_shape.setPointCount(SIZE);

					for (size_t i = 0; i < SIZE; i++)
					{
						lightShape->_shape.setPoint(i, vecPoints[i]);
					}

					lightShape->_renderLightOverShape = true;//��������� ���� ������ ������.�� ��������� false.
					lightShape->_shape.setPosition(x, y);

					// ���������� ������� ����� � ���������� ������
					m_LightSystem.addShape(lightShape);
					
					continue;
				}*/
				
				// ������� �������� �������
				const String& Name = XML_TO_STRING(Object, "name");

				// ������� ������� �������
				const Vector2f& Pos = { XML_TO_FLOAT(Object, "x"), XML_TO_FLOAT(Object, "y") };

				// ������ ������ ��� �����
				Sprite sprite;
				sprite.setPosition(Pos);
				sprite.setTextureRect({ 0, 0, XML_TO_INT(Object, "width"), XML_TO_INT(Object, "height") });

				map<string, string> properties;

				// ������� ��������
				if (Object->FirstChildElement("properties") != nullptr)
				{
					const auto& RootProp = Object->FirstChildElement("properties");

					for (auto Prop = RootProp->FirstChildElement();
						Prop;
						Prop = Prop->NextSiblingElement())
					{
						properties[XML_TO_STRING(Prop, "name")] = XML_TO_STRING(Prop, "value");
					}
				}
				
				// ����������� ��������
				RegisterEntities(NameGroup, Name, sprite, std::move(properties));
			}
		}
	}
}

void Level::RegisterEntities(const string& layer, const string& className, const Sprite& sprite, map<string, string>&& properties)
{
	/*if (layer.find("bonus_") != string::npos)
	{
		Entity entity;
		entity.SetSprite(sprite);

		m_Layers[layer].push_back(make_shared<Entity>(entity));
	}
	else */if (layer == "scripts" || className == "collision_angle_left" || className == "collision_angle_right" || properties["draw"] == "no")
	{
		Entity entity;
		entity.SetType(className);
		entity.SetSprite(sprite);
		entity.SetTexture("test");
		entity.SetProperty("isDraw", false);

		if (properties["draw"] == "no")
			entity.SetProperty("no_validate", true);

		m_Layers[layer].push_back(make_shared<Entity>(entity));
	}

	else if (className == "teleport")
	{
		Teleport entity;
		entity.SetLayers(&m_LayersValide);
		entity.SetType(className);
		entity.SetSprite(sprite);
		entity.SetTexture("test");
		entity.SetProperty("isDraw", false);
		entity.SetProperty("to", properties["to"]);

		m_Layers[layer].push_back(make_shared<Teleport>(entity));
	}

	// ������ player?
	else if (className == "player")
	{
		Player entity;
		entity.SetSprite(sprite);
		entity.SetLayers(&m_LayersValide);
		System::m_Camera.setCenter(entity);

		AddEntity(layer, className, entity, std::move(properties));
	}

	// ������ bot?
	else if (className == "bot")
	{
		Bot entity;
		entity.SetSprite(sprite);
		entity.SetLayers(&m_LayersValide);

		AddEntity(layer, className, entity, std::move(properties));
	}

	// ������ movingPlatform?
	else if (className == "movingPlatform")
	{
		MovingPlatform entity;
		entity.SetSprite(sprite);
		entity.SetLayers(&m_LayersValide);

		AddEntity(layer, className, entity, std::move(properties));
	}

	// ������ light?
	else if (className == "light")
	{
		AnySTD color;
		color.StringToValue(properties["color"]);
		vector<AnySTD> vecColor = color;

		AnySTD scale;
		scale.StringToValue(properties["scale"]);
		vector<AnySTD> vecScale = scale;

		auto light = make_shared<ltbl::LightPointEmission>();//LightPointEmission-> �������� ��������, ������, �����, ���������.
		light->_emissionSprite.setScale(sf::Vector2f(vecScale[0].Get<int>(), vecScale[1].Get<int>()));// ������ ������� ��������.
		light->_emissionSprite.setColor(sf::Color(vecColor[0].Get<int>(), vecColor[1].Get<int>(), vecColor[2].Get<int>()));// ���� �����.
		light->_emissionSprite.setPosition(sprite.getPosition());//������� �����.
		light->_emissionSprite.setRotation(stoi(properties["angle"]));
		light->_sourceRadius = 10;//������ ��������� �����.�� ��������� 1.
		light->_shadowOverExtendMultiplier = 1;// ���������� ������������� ����(� ������� ��� ����������� ����). 
		m_Light->Add(properties["type"], std::move(light));
	}

	// ������ pointer?
	else if (className == "pointer")
	{
		Entity entity;
		entity.SetType(className);
		entity.SetSprite(sprite);
		entity.SetTexture("test");
		entity.SetRect({ 0, 0, 32, 32 });
		entity.SetProperty("direction", properties["direction"]);
		entity.SetProperty("isDraw", false);
		m_Layers[layer].push_back(make_shared<Entity>(entity));
	}

	// ������ ��������?
	else if (className == "stair")
	{
		Entity entity;
		entity.SetType(className);
		entity.SetSprite(sprite);
		entity.SetTexture("test");
		entity.Move({ 0.0f, -1.0f });
		entity.SetProperty("isDraw", false);
		m_Layers[layer].push_back(make_shared<Entity>(entity));
	}

	// ���������
	else
	{
		Entity entity;
		entity.SetSprite(sprite);

		AddEntity(layer, className, entity, std::move(properties));
	}
}

bool Level::IsNextLevel() const
{
	return m_isNextLevel;
}

void Level::UpdateLayersValide()
{
	// �������� ����������� ��������� ����
	m_LayersValide.clear();

	// ������������ �������� ���� (����� ����� �� �������� ������ ����� ���������)
	FloatRect visibleRect = System::GetVisibleRect();
	visibleRect.left -= 500.0f;
	visibleRect.top -= 500.0f;
	visibleRect.width += 1000.0f;
	visibleRect.height += 1000.0f;

	// ���������� �� �����
	for (auto&& [name, vec] : m_Layers)
	{
		// ���������� �� �������� ����
		for (auto&& it = vec.begin(); it != vec.end();)
		{
			// ���� ������ damage_box ����
			if (name == "damage_box" && (*it)->GetProperty<int>("hp") < 0)
				(*it)->SetProperty("isLife", false);

			// ���� ������ ���
			if ((*it)->GetProperty<bool>("isLife"))
			{
				// �������� ������� � ���� ������� ����
				if ((*it)->GetRect().intersects(visibleRect) || (*it)->GetType() == "player" || (*it)->IsFindProperty("no_validate"))
				{
					// �������� ������ �� �������� ����
					m_LayersValide[name].push_back(*it);
				}
				// ����� ���� ������ ����
				else if ((*it)->IsType("bullet"))
				{
					// ������� ������
					m_Layers[name].erase(it);
					it--;
				}

				it++;
			}
			else
			{
				// ������� ������
				m_Layers[name].erase(it);
			}
		}
	}
}

void Level::Update()
{
	if (KEY_PRESSED(L) && !System::m_isPause) m_isNextLevel = true;

	// ���������� �������� ����
	UpdateLayersValide();

	// ����������� �� �����
	for (auto&& [name, value] : m_LayersValide)
		// ����������� �� �������� ����
		for (auto& entity : value)
		{
			// ���������� �������
			entity->Update();

			// �����
			Fire(name, std::move(entity));
		}
}

void Level::Draw()
{
	// ��������� ���� �������� ����
	for (auto&& [name, value] : m_LayersValide)
		for (auto& entity : value)
			entity->Draw();
}

Entity::Layers& Level::GetLayers()
{
	return m_Layers;
}

shared_ptr<Entity>& Level::GetEntityByType(const string& type)
{
	// ���� ������ ���� �� ���� = type, �� ���������� ���� ������
	for (auto&& [name, value] : m_Layers)
		for (auto& entity : value)
			if (entity->GetType() == type) return entity;
}

Entity::Entities& Level::GetEntitiesByType(const string& type)
{
	// ������, �������� ������ �������
	Entity::Entities entities;

	// ���� ������ ���� �� ���� = type, ��������� � ������
	for (auto&& [name, value] : m_Layers)
		for (auto& entity : value)
			if (entity->GetType() == type)
				entities.push_back(entity);

	// ���������� ������
	return entities;
}

void Level::Fire(const string& layer, shared_ptr<Entity>&& entity)
{
	// ������ ��� ������ ��� ����
	if (entity->IsType("player") || entity->IsType("bot"))
	{
		// �������� �����
		Vector2f p2;

		if (entity->IsType("player"))
		{
			// ��� ������ - ������� ����
			p2 = System::m_MousePosition;
		}
		else
		{
			// ��� ����� - ������� ������
			const FloatRect& rectPlayer = entity->GetProperty("playerRect");

			// ���� ��� �������� ������ � ����� � ������� �������� �����
			if (rectPlayer != FloatRect(0.0f, 0.0f, 0.0f, 0.0f) && entity->To<Bot>().GetBotType() == Bot::BotType::Enemy)
			{
				p2 = { rectPlayer.left + rectPlayer.width / 2.0f, rectPlayer.top + rectPlayer.height / 2.0f };
			}
			else
			{
				// ����� ������� ������ �� ������� "�����" �� ����������� ��������
				const FloatRect& rect = entity->GetRect();
				p2 = { rect.left + rect.width / 2.0f, rect.top + rect.height / 2.0f };

				const string& direction = entity->GetProperty("direction");

				if (direction == "left")		p2.x -= 50.0f;
				else if (direction == "right")	p2.x += 50.0f;
			}
		}

		// �������� ������ �������
		auto weapon = entity->To<Player>().GetWeapons();

		// �������� � ��������� ���� ������
		auto bullet = weapon->Update(entity->GetRect(), p2);

		// ���� ���� ����������
		if (bullet)
		{
			// ��������� ���������� ��� ��������
			static auto fireDelegate = [](Entity& entity, Bullet bullet, Entity::Entities* layer)
			{
				auto abilityContainer = entity.To<Player>().GetAbility();

				// ���� ��������������� ������ ������������
				if (abilityContainer)
				{
					// ��������� ����������� �������� � ������� ������������
					auto ability = abilityContainer->Get();

					for (auto& ability : ability)
					{
						if (ability.first == AbilityType::WeaponHoming)
						{
							bullet.SetProperty("typeBullet", string("homing"));
						}
					}

					// ��������� ����������� ����
					for (auto& ability : ability)
					{
						if (ability.first == AbilityType::WeaponSuper)
						{
							layer->push_back(make_shared<Bullet>(bullet));
							bullet.SetProperty("offset", 0.2f);
							layer->push_back(make_shared<Bullet>(bullet));
							bullet.SetProperty("offset", -0.2f);
							layer->push_back(make_shared<Bullet>(bullet));
							return;
						}
					}
				}

				// ����������� ���������� ����
				layer->push_back(make_shared<Bullet>(bullet));
			};

			// ��������� ���� � ���������� ����� ��������
			bullet->SetProperty("fireEntity", entity->GetType());
			bullet->SetLayers(&m_LayersValide);
			fireDelegate(*entity, *bullet, &m_Layers[layer]);
		}
	}
}