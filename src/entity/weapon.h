///==========================================================================
///	Модуль:		Оружие
///	Автор:		Андрей Гребенщиков
///	Год:		2019
///==========================================================================

#ifndef WEAPON_INCLUDE
#define WEAPON_INCLUDE

#include "..\entity\entity.h"

// @brief Пуля
class Bullet 
	: public Entity
{
public:
	// @brief Конструктор
	Bullet();

	// @brief Деструктор
	virtual ~Bullet();

	// @brief Обновление
	void Update();

	// @brief Установка угла
	// @param p1 - Первая точка
	// @param p2 - Вторая точка
	void SetAngle(const Vector2f& p1, const Vector2f& p2);

	// @brief Установка слоёв
	// @param layers - Слои
	void SetLayers(Layers* layers);

private:
	// @brief Слои
	Layers* m_Layers = nullptr;
};

// @brief Оружие
class Weapon 
{
public:
	// @brief Конструктор
	Weapon();

	// @brief Деструктор
	virtual ~Weapon();

	// @brief Обновление
	// @param p1 - Первая точка
	// @param p2 - Вторая точка
	void Update(const Vector2f& p1, const Vector2f& p2);

	// @brief Рисование
	void Draw();

	// @return Пуля
	Bullet* GetBullet();

	// @brief Спрайт оружия
	Sprite* GetSprite();

	// @brief Установка имени оружия
	// @param name - Имя
	void SetName(const string& name);

	// @return Имя оружия
	string GetName() const;

	// @brief Установка картинки спрайта
	// @param textureName - Название текстуры
	// @param rect - Область
	void SetImageSprite(const string& textureName, const IntRect& rect);

private:
	// @brief Имя оружия
	string m_name;

	// @brief Спрайт оружия
	Sprite m_sprite;

	// @brief Пуля
	Bullet m_bullet;
};

// @brief Оружия
class Weapons 
{
public:
	// @brief Конструктор
	Weapons();

	// @brief Деструктор
	virtual ~Weapons();

	// @brief Добавление оружия
	// @param key - Ключ
	// @param weapon - Оружие
	void Add(const string& key, Weapon weapon);

	// @brief Возвращение оружия по ключу
	// @param key - Ключ
	// @return Оружие
	shared_ptr<Weapon> Get(const string& key);

	// @return Оружия
	map<string, shared_ptr<Weapon>> Get();

	// @brief Установка текущего оружия
	// @param key - Ключ
	void SetCurrentWeapon(const string& key);

	// @return Текущее оружия
	string GetCurrentWeapon() const;

	// @brief Установка огня
	// @param isFire - Включить или выключить огонь
	void SetIsFire(bool isFire);

	// @param Включен ли огонь
	bool GetIsFire() const;

	// @brief Обновление
	// @param r1 - Область стреляюшего объекта
	// @param p2 - Точка возможного врага
	shared_ptr<Bullet> Update(FloatRect r1, const Vector2f& p2);

	// @brief Рисование
	void Draw();

private:
	// @brief Включен ли огонь
	bool m_isFire = false;

	// @brief Интервал между пулями
	float m_delay = 0.0f;

	// @brief Оружия
	map<string, shared_ptr<Weapon>> m_data;

	// @brief Текущее оружие
	string m_currentWeapon;
};

#endif /// WEAPON_INCLUDE