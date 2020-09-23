///==========================================================================
///	Модуль:		Уровень
///	Автор:		Андрей Гребенщиков
///	Год:		2019
///==========================================================================

#include "level.h"
#include "../entity/teleport.h"

Level::Level(const string& file, Light* light, map<string, shared_ptr<Weapon>>* templatesWeapons) 
	: m_Light(light), 
	  m_TemplatesWeapons(templatesWeapons)
{
	// Загрузка XML файла
	TiXmlDocument Doc;

	/// Считывание карты
	if (!Doc.LoadFile((PATH_MAPS + file).c_str()))
	{
		std::cout << "Loading level \"" << file << "\" failed." << std::endl;
		return;
	}

	// Инициализация Root элемента
	const auto& Root = Doc.FirstChildElement("map");

	// Инициализация настроек карты
	const int& Width = XML_TO_INT(Root, "width"),
			 & Height = XML_TO_INT(Root, "height"),
			 & TileWidth = XML_TO_INT(Root, "tilewidth"),
			 & TileHeight = XML_TO_INT(Root, "tileheight");
	
	System::m_MapSize.x = (float)Width * (float)TileWidth;
	System::m_MapSize.y = (float)Height * (float)TileHeight;

	// Инициализация тайлсета
	const auto& Tileset		= Root->FirstChildElement("tileset");
	const int&  FirstTileID	= XML_TO_INT(Tileset, "firstgid");

	// Инициализация пути до картинки у тайлсета
	const auto& Image = Tileset->FirstChildElement("image");
	std::string tilesetFile	= XML_TO_STRING(Image, "source");

	// Загрузка картинки
	sf::Image Img;

	// Для обеспечения совместимости с Tiled Map Editor
	tilesetFile.erase(0, 3);
	tilesetFile = "resources/" + tilesetFile;

	// Загрузка изображения карты
	if (!Img.loadFromFile(tilesetFile))
	{
		std::cout << "Failed to load tile sheet." << std::endl;
		//Img.createMaskFromColor(Color::White);
		return;
	}

	// Установка картинки в главный массив текстур
	Texture texture;
	//texture.loadFromFile(tilesetFile);
	texture.loadFromImage(Img);
	texture.setRepeated(true);
	texture.setSmooth(true);
	
	System::m_Textures["map"] = texture;

	// Инициализация кол-ва колонок и строк
	const int& Columns = texture.getSize().x / TileWidth;
	const int& Rows	= texture.getSize().y / TileHeight;

	// Нахождение области тайлов в тайлсете
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
	
	// Проходимся по всем элементам layer
	for (auto eLayer = Root->FirstChildElement("layer"); 
		eLayer; 
		eLayer = eLayer->NextSiblingElement("layer"))
	{
		// Нахождение имени слоя
		const String& LayerName = XML_TO_STRING(eLayer, "name");

		// Нахождение карты в строковом виде
		const std::string& StrMap = eLayer->FirstChildElement("data")->GetText();

		// Текущая позиция на карте
		int x = 0;
		int y = 0;

		/// Расшифрока CSV формата карты

		// Проходимся по строковой карте
		const size_t& SIZE = StrMap.size();
		for (size_t i = 0; i < SIZE; i++)
		{
			// Считываем цифры до запятой в виде строки
			std::string Index = "";

			do
			{
				Index += StrMap[i];
				i++;
			}
			while (StrMap[i] != ',' && i < SIZE);

			// Переводим строковые цифры в обычные
			const int& Id = stoi(Index);

			// Находим тайл
			const int& SubRectToUse = Id - FirstTileID;

			// Если не пустой
			if (SubRectToUse >= 0)
			{
				// Заносим спрайт в глобальные объекты
				Entity entity;
				entity.SetTexture("map");
				entity.SetRect(SubRects[SubRectToUse]);
				entity.SetPosition(static_cast<float>(x * TileWidth), static_cast<float>(y * TileHeight));

				// Для типа объекта damage_box установить свойство здоровья
				if (LayerName == "damage_box")
					entity.SetProperty("hp", stoi(System::m_Templates["damage_box"]["hp"]));

				if (System::m_Animations.find(LayerName) != System::m_Animations.end())
				{
					entity.SetAnimation(std::move(System::m_Animations[LayerName]));
				}

				m_Layers[LayerName].push_back(make_shared<Entity>(entity));
			}

			// Логика прохода по карте
			x++;
			if (x >= Width)
			{
				x = 0;
				y++;
			}
		}
	}

	/// Считывание объектов
	// Если группы объектов существуют
	if (Root->FirstChildElement("objectgroup") != nullptr)
	{
		// Проходимся по всем группам объектов
		for (auto ObjectGroup = Root->FirstChildElement("objectgroup");
			ObjectGroup;
			ObjectGroup = ObjectGroup->NextSiblingElement("objectgroup"))
		{
			// Находим имя группы
			const String& NameGroup = XML_TO_STRING(ObjectGroup, "name");

			// Проходимся по всем объектам группы
			for (auto Object = ObjectGroup->FirstChildElement();
				Object;
				Object = Object->NextSiblingElement())
			{
				// Находим слой light_block
				/*if (NameGroup == "light_block")
				{
					// Начинаем считывать точки
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

					//  Занесение информации в объект света
					std::shared_ptr<ltbl::LightShape> lightShape = std::make_shared<ltbl::LightShape>();

					const size_t SIZE = vecPoints.size();
					lightShape->_shape.setPointCount(SIZE);

					for (size_t i = 0; i < SIZE; i++)
					{
						lightShape->_shape.setPoint(i, vecPoints[i]);
					}

					lightShape->_renderLightOverShape = true;//Рендерить свет сквозь фигуру.По умолчанию false.
					lightShape->_shape.setPosition(x, y);

					// Добавление объекта света в глобальный объект
					m_LightSystem.addShape(lightShape);
					
					continue;
				}*/
				
				// Находим название объекта
				const String& Name = XML_TO_STRING(Object, "name");

				// Находим позицию объекта
				const Vector2f& Pos = { XML_TO_FLOAT(Object, "x"), XML_TO_FLOAT(Object, "y") };

				// Создаём объект для карты
				Sprite sprite;
				sprite.setPosition(Pos);
				sprite.setTextureRect({ 0, 0, XML_TO_INT(Object, "width"), XML_TO_INT(Object, "height") });

				map<string, string> properties;

				// Заносим свойства
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
				
				// Регистрация объектов
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

	// объект player?
	else if (className == "player")
	{
		Player entity;
		entity.SetSprite(sprite);
		entity.SetLayers(&m_LayersValide);
		System::m_Camera.setCenter(entity);

		AddEntity(layer, className, entity, std::move(properties));
	}

	// объект bot?
	else if (className == "bot")
	{
		Bot entity;
		entity.SetSprite(sprite);
		entity.SetLayers(&m_LayersValide);

		AddEntity(layer, className, entity, std::move(properties));
	}

	// объект movingPlatform?
	else if (className == "movingPlatform")
	{
		MovingPlatform entity;
		entity.SetSprite(sprite);
		entity.SetLayers(&m_LayersValide);

		AddEntity(layer, className, entity, std::move(properties));
	}

	// объект light?
	else if (className == "light")
	{
		AnySTD color;
		color.StringToValue(properties["color"]);
		vector<AnySTD> vecColor = color;

		AnySTD scale;
		scale.StringToValue(properties["scale"]);
		vector<AnySTD> vecScale = scale;

		auto light = make_shared<ltbl::LightPointEmission>();//LightPointEmission-> точечное свечение, фонарь, костёр, прожектор.
		light->_emissionSprite.setScale(sf::Vector2f(vecScale[0].Get<int>(), vecScale[1].Get<int>()));// Размер области свечения.
		light->_emissionSprite.setColor(sf::Color(vecColor[0].Get<int>(), vecColor[1].Get<int>(), vecColor[2].Get<int>()));// Цвет света.
		light->_emissionSprite.setPosition(sprite.getPosition());//Позиция света.
		light->_emissionSprite.setRotation(stoi(properties["angle"]));
		light->_sourceRadius = 10;//Радиус источника света.По умолчанию 1.
		light->_shadowOverExtendMultiplier = 1;// Умножитель отбрасываемой тени(в столько раз увеличиться тень). 
		m_Light->Add(properties["type"], std::move(light));
	}

	// объект pointer?
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

	// объект лестница?
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

	// Остальные
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
	// Удаление предыдущего валидного слоя
	m_LayersValide.clear();

	// Растягивание валидной зоны (Чтобы враги за областью экрана могли двигаться)
	FloatRect visibleRect = System::GetVisibleRect();
	visibleRect.left -= 500.0f;
	visibleRect.top -= 500.0f;
	visibleRect.width += 1000.0f;
	visibleRect.height += 1000.0f;

	// Проходимся по слоям
	for (auto&& [name, vec] : m_Layers)
	{
		// Проходимся по объектам слоя
		for (auto&& it = vec.begin(); it != vec.end();)
		{
			// Если объект damage_box умер
			if (name == "damage_box" && (*it)->GetProperty<int>("hp") < 0)
				(*it)->SetProperty("isLife", false);

			// Если объект жив
			if ((*it)->GetProperty<bool>("isLife"))
			{
				// Заносить объекты в зоне текущий игры
				if ((*it)->GetRect().intersects(visibleRect) || (*it)->GetType() == "player" || (*it)->IsFindProperty("no_validate"))
				{
					// Добавить объект на валидный слой
					m_LayersValide[name].push_back(*it);
				}
				// Иначе если объект пуля
				else if ((*it)->IsType("bullet"))
				{
					// Удалить объект
					m_Layers[name].erase(it);
					it--;
				}

				it++;
			}
			else
			{
				// Удалить объект
				m_Layers[name].erase(it);
			}
		}
	}
}

void Level::Update()
{
	if (KEY_PRESSED(L) && !System::m_isPause) m_isNextLevel = true;

	// Обновление валидных слоёв
	UpdateLayersValide();

	// Прохождение по слоям
	for (auto&& [name, value] : m_LayersValide)
		// Прохождение по объектам слоя
		for (auto& entity : value)
		{
			// Обновление объекта
			entity->Update();

			// Огонь
			Fire(name, std::move(entity));
		}
}

void Level::Draw()
{
	// Рисование всех объектов слоёв
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
	// Если объект слоя по типу = type, то возвращать этот объект
	for (auto&& [name, value] : m_Layers)
		for (auto& entity : value)
			if (entity->GetType() == type) return entity;
}

Entity::Entities& Level::GetEntitiesByType(const string& type)
{
	// Объект, хранящий нужные объекты
	Entity::Entities entities;

	// Если объект слоя по типу = type, добавлять в объект
	for (auto&& [name, value] : m_Layers)
		for (auto& entity : value)
			if (entity->GetType() == type)
				entities.push_back(entity);

	// Возвратить объект
	return entities;
}

void Level::Fire(const string& layer, shared_ptr<Entity>&& entity)
{
	// Только для игрока или бота
	if (entity->IsType("player") || entity->IsType("bot"))
	{
		// Конечная точка
		Vector2f p2;

		if (entity->IsType("player"))
		{
			// Для игрока - позиция мыши
			p2 = System::m_MousePosition;
		}
		else
		{
			// Для врага - позиция игрока
			const FloatRect& rectPlayer = entity->GetProperty("playerRect");

			// Если бот является врагом и игрок в области действия врага
			if (rectPlayer != FloatRect(0.0f, 0.0f, 0.0f, 0.0f) && entity->To<Bot>().GetBotType() == Bot::BotType::Enemy)
			{
				p2 = { rectPlayer.left + rectPlayer.width / 2.0f, rectPlayer.top + rectPlayer.height / 2.0f };
			}
			else
			{
				// Иначе ставить оружие на позицию "вперёд" от направления движения
				const FloatRect& rect = entity->GetRect();
				p2 = { rect.left + rect.width / 2.0f, rect.top + rect.height / 2.0f };

				const string& direction = entity->GetProperty("direction");

				if (direction == "left")		p2.x -= 50.0f;
				else if (direction == "right")	p2.x += 50.0f;
			}
		}

		// Получаем оружие объекта
		auto weapon = entity->To<Player>().GetWeapons();

		// Получаем и обновляем пулю оружия
		auto bullet = weapon->Update(entity->GetRect(), p2);

		// Если пуля возвращена
		if (bullet)
		{
			// Возможные модификаты при выстреле
			static auto fireDelegate = [](Entity& entity, Bullet bullet, Entity::Entities* layer)
			{
				auto abilityContainer = entity.To<Player>().GetAbility();

				// Если инициализирован объект способностей
				if (abilityContainer)
				{
					// Возможная модификация выстрела с помощью способностей
					auto ability = abilityContainer->Get();

					for (auto& ability : ability)
					{
						if (ability.first == AbilityType::WeaponHoming)
						{
							bullet.SetProperty("typeBullet", string("homing"));
						}
					}

					// Возможная модификация пули
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

				// Стандартное добавление пули
				layer->push_back(make_shared<Bullet>(bullet));
			};

			// Добавлять пулю в глобальный масив объектов
			bullet->SetProperty("fireEntity", entity->GetType());
			bullet->SetLayers(&m_LayersValide);
			fireDelegate(*entity, *bullet, &m_Layers[layer]);
		}
	}
}