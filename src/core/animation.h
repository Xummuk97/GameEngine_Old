///==========================================================================
///	Модуль:		Анимация
///	Автор:		Андрей Гребенщиков
///	Год:		2019
///==========================================================================

#ifndef ANIMATION_INCLUDE
#define ANIMATION_INCLUDE

#include <SFML/Graphics/Rect.hpp>
#include <vector>
#include <map>
#include <memory>
#include <string>
#include <iostream>

using namespace std;
using namespace sf;

// @brief Анимация
class Animation
{
public:
	// @brief Кадр
	using Frame = shared_ptr<IntRect>;

	// @brief Указатель
	using Ptr = shared_ptr<Animation>;

	// @brief Конструктор
	Animation() = default;

	// @brief Конструктор с параметром
	// @param maxTime - Время, через которое осуществляется смена кадра анимации
	Animation(float maxTime);

	// @brief Деструктор
	~Animation() = default;

	// @brief Добавление кадра
	// @param rect - Область кадра на текстуре
	void Add(Frame&& rect);

	// @brief Обновление
	void Update();

	// @brief Сброс
	void Reset();	

	// @return Был ли совершён переход на новый кадр
	bool IsNextFrame() const;

	// @return Область текущей анимации
	IntRect& GetRectCurrentAnimation();

	// @return Массив кадров
	vector<Frame>& GetData();								

	// @brief Перевернуть анимацию
	// @param Включить/выключить
	void SetFlip(bool isFlip);

	// @return Перевёрнуто ли изображение
	bool IsFlip() const;

private:
	// @brief Совершён ли сброс
	bool m_IsReset = false;

	// @brief Совершён ли переход на следующий кадр
	bool m_IsNextFrame = false;	

	// Переворачивать ли изображение
	bool m_IsFlip = false;

	// @brief Счётчик времени
	float m_Time = 0.0f;

	// @brief Время, через которое будет осуществляться смена кадра
	float m_MaxTime;	

	// @brief Индекс текущей анимации
	size_t m_IndexAnimation = 0;		

	// @brief Массив кадров
	vector<Frame> m_Data;					
};

// @brief Анимации
class Animations
{
public:
	// @brief Конструктор
	Animations() = default;
	
	// @brief Деструктор
	~Animations() = default;

	// @brief Добавление анимации
	// @param key - Ключ
	// @param animation - Объект анимации
	void Add(const string& key, Animation::Ptr&& animation);	

	// @brief Установка текущей анимации
	// @param key - Ключ
	void SetCurrentAnimation(const string& key);

	// @brief Обновление и возвращение текущей анимации
	Animation& UpdateAndGetCurrentAnimation();		

	// @brief Перевернуть анимацию
	// @param Включить/выключить
	void SetFlip(bool isFlip);

	// @return Перевёрнуто ли изображение
	bool IsFlip() const;

private:
	// Переворачивать ли изображение
	bool m_IsFlip = false;

	// @brief Счётчик времени
	float m_CurrentTime = 0.0f;							

	// @brief Массив анимаций
	map<string, Animation::Ptr> m_Data;	

	// @brief Текущая анимация	
	string m_CurrentAnimationFrame = "none";				
};

#endif /// ANIMATION_INCLUDE