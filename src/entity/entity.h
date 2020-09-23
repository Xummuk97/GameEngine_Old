///==========================================================================
///	Модуль:		Игровой объект
///	Автор:		Андрей Гребенщиков
///	Год:		2019
///==========================================================================

#ifndef ENTITY_INCLUDE
#define ENTITY_INCLUDE

#include "..\core\system.h"
#include "..\core\any.h"
#include "person.h"

// @brief Игровой объект
class Entity
{
public:
	// @brief Массив объектов
	using Entities	= vector<shared_ptr<Entity>>;

	// @brief Слои (Массив массивов объектов)
	using Layers	= map<string, Entities>;

	// @brief Конструктор
	Entity();

	// @brief Деструктор
	~Entity();

	// @brief Загрузка
	// Вызывается при полной загрузке объекта на карте
	virtual void Load();

	// @brief Обновление
	virtual void Update();

	// @brief Рисование
	virtual void Draw();

	// @brief Установка анимации
	// @param animation - Анимация
	void SetAnimation(const shared_ptr<Animations>& animation);

	// @brief Установка спрайта
	// @param sprite - Спрайт
	void SetSprite(const Sprite& sprite);

	// @return Спрайт
	Sprite* GetSprite();

	// @brief Установка текстуры
	// @param key - Ключ
	// Если не указать параметр key, то текстура берётся из шаблона
	void SetTexture(const string& key = "null");

	// @brief Установка области
	// @param rect - Область
	void SetRect(const IntRect& rect);

	// @brief Установка области из шаблона
	void SetRectByProperty();

	// @brief Перемещение
	// @param pos - Смещение
	void Move(const Vector2f& pos);

	// @brief Перемещение
	// @param x - Смещение X
	// @param y - Смещение Y
	void Move(float x, float y);

	// @brief Установка позиции
	// @param pos - Позиция
	void SetPosition(const Vector2f& pos);

	// @brief Установка позиции
	// @param x - X
	// @param y - Y
	void SetPosition(float x, float y);

	// @brief Установка свойства
	// @param key - Ключ
	// @param value - Значение
	template<class T>
	void SetProperty(const string& key, T value);

	// @brief Установка свойства
	// @param key - Ключ
	// @param value - Строковое значение
	void SetProperty(const string& key, const string& value);

	// @brief Установка типа
	// @param type - Тип
	void SetType(const string& type);

	// @return Тип
	string GetType() const;

	// @return Позиция
	Vector2f GetPosition() const;

	// @return Область
	FloatRect GetRect() const;

	// @param key - Ключ
	// @return Свойство
	AnySTD& GetProperty(const string& key);

	// @brief Возвращение свойства с приведением
	// @param key - Ключ
	// @return Приведённое свойство
	template<class T>
	T GetProperty(const string& key);
	
	// @param entity - Объект
	// @return Есть ли пересечение с объектом
	bool IsIntersects(const Entity& entity) const;

	// @param key - Ключ
	// @return Существует ли свойство
	bool IsFindProperty(const string& key) const;

	// @param entity - Объект
	// @return Есть ли пересечение с объектом
	bool operator == (const Entity& entity) const;

	// @param entity - Объект
	// @return Нет ли пересечение с объектом
	bool operator != (const Entity& entity) const;

	// @param type - Тип
	// @return Совпадает ли тип
	bool IsType(const string& type) const;

	// @return Приведение объекта
	template<class T>
	constexpr T& To();

	// @brief Смерть через время
	// @param time - Время
	void DeadThroughTime(float time);

	// @brief Толчок
	// @param time - Время действия
	// @param damage - Сила толчка
	void Push(float time, float damage);

	// @brief Урон
	// @param damage - Урон
	void Damage(int damage);

	// @brief Установка скриптов
	void SetScripts(function<bool (Entity* entity, string scriptName)>&& scripts);

	// @brief Установка персоны
	// @param person - Персона
	void SetPerson(const Person& person);

	// @brief Установка персоны по свойству
	void SetPersonByProperty();

	// @return Персона
	Person GetPerson();

	// @return Позиция 
	operator Vector2f() const { return m_Sprite.getPosition(); }

	// @return Область
	operator FloatRect() const { return m_Sprite.getGlobalBounds(); }

	// @return Персона
	operator Person() const { return m_Person; }

protected:
	// @brief Поворот спрайта (Если объект по позиции X меньше X, то влево, иначе вправо)
	// @param x - Позиция для направления
	void IsFlipAnimation(float x);

	// @brief Таймер получения урона
	float m_timerDmg = 0.0f;

	// @brief Свойства
	map<string, AnySTD> m_Property;

	// @brief Анимации
	shared_ptr<Animations> m_Animation;

	// @brief Тип
	string m_Type = "null";

	// @brief Персона
	Person m_Person = Person(L"Объект", Color::White, Color::Black);

	// @brief Скрипты
	function<bool (Entity* entity, string scriptName)> m_scripts;

	// @brief Спрайт
	Sprite m_Sprite;
};

template<class T>
inline void Entity::SetProperty(const string& key, T value)
{ m_Property[key] = value; }

template<class T>
inline T Entity::GetProperty(const string& key)
{
	return m_Property[key].Get<T>();
};

template<class T>
inline constexpr T& Entity::To()
{ return *(T*)(this); };


#endif /// ENTITY_INCLUDE