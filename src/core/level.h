///==========================================================================
///	Модуль:		Уровень
///	Автор:		Андрей Гребенщиков
///	Год:		2019
///==========================================================================


#ifndef LEVEL_INCLUDE
#define LEVEL_INCLUDE

#include "system.h"
#include "light.h"
#include "..\entity\entity.h"
#include "..\entity\player.h"
#include "..\entity\bot.h"
#include "..\entity\movingPlatform.h"

// @brief Уровень
class Level
{
public:
	// @brief Конструктор с параметром
	// @param file - Название .tmx файла
	// @param light - Указатель на объект света
	// @param templatesWeapons - Указатель на объект шаблонов оружия
	Level(const string& file, Light* light, map<string, shared_ptr<Weapon>>* templatesWeapons);

	// @brief Деструктор
	~Level() = default;

	// @brief Обновление
	void Update();		

	// @brief Рисование
	void Draw();			

	// @return Слои
	Entity::Layers& GetLayers();

	// @param type - Тип объекта
	// @return Объект указанного типа
	shared_ptr<Entity>& GetEntityByType(const string& type);

	// @param type - Тип объекта
	// @return Все объекты указанного типа
	Entity::Entities& GetEntitiesByType(const string& type);

	// @brief Добавление объекта
	// @param layer - Слой
	// @param className - Тип объекта
	// @param entity - Объект
	// @param properties - Свойства с карты
	// Метод добавляет объекту свойства из шаблона и с карты
	template<class T>
	void AddEntity(const string& layer, const string& className, T& entity, map<string, string>&& properties);

	// @return Осуществлить ли переход на следующий уровень
	bool IsNextLevel() const;		

private:
	// @brief Обновление валидного слоя
	// Метод в объект валидного слоя добавляет объекты из базового слоя
	// с условием нахождения в определённой области и не быть удалённым
	void UpdateLayersValide();

	// @brief Регистрация объектов
	// @param layer - Слой
	// @param className - Тип объекта
	// @param sprite - Спрайт объекта
	// @param properties - Свойства с карты
	// Метод производит определённые действия по созданию объекта с учётом его типа
	void RegisterEntities(const string& layer, const string& className, const Sprite& sprite, map<string, string>&& properties);

	// @brief Огонь
	// @param layer - Слой
	// @param entity - Объект
	// Если объект "выстрельнул", создавать пулю с определёнными параметрами
	void Fire(const string& layer, shared_ptr<Entity>&& entity);

	// @brief Осуществить ли переход на следующий уровень
	bool m_isNextLevel = false;

	// @brief Свет
	Light* m_Light;

	// @brief Шаблоны оружия
	map<string, shared_ptr<Weapon>>* m_TemplatesWeapons;

	// @brief Слои
	Entity::Layers m_Layers;

	// @brief Валидные слои
	Entity::Layers m_LayersValide;
};

template<class T>
inline void Level::AddEntity(const string& layer, const string& className, T& entity, map<string, string>&& properties)
{
	// Добавляем объекту свойства
	for (auto& property : properties)
	{
		entity.SetProperty(property.first, property.second);
	}

	// Считываем свойства с шаблона и добавляем их в объект
	for (auto& property : System::m_Templates[className])
	{
		entity.SetProperty(property.first, property.second);
	}

	// Установка типа
	entity.SetType(className);	

	// Возможная установка анимации
	if (System::m_Animations.find(className) != System::m_Animations.end())
	{
		entity.SetAnimation(std::move(System::m_Animations[className]));
	}

	// Возможная установка оружия объекту
	entity.To<Player>().SetWeaponsProperty(*m_TemplatesWeapons);

	// Загрузка объекта
	entity.Load();

	// Добавление объекта в глобальный массив объектов
	m_Layers[layer].push_back(make_shared<T>(entity));
};

#endif /// LEVEL_INCLUDE