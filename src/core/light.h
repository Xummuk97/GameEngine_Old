///==========================================================================
///	Модуль:		Свет
///	Автор:		Андрей Гребенщиков
///	Год:		2019
///==========================================================================

#ifndef LIGHT_INCLUDE
#define LIGHT_INCLUDE

#include "system.h"

// @brief Контейнер света
class Light
{
public:
	// @brief Конструктор
	Light();			

	// @brief Деструктор
	~Light() = default;

	// @brief Добавление света
	// @param lightType - Тип (point или cone)
	// @param key - Ключ
	// @param light - Объект света
	void Add(const string& lightType, const string& key, shared_ptr<ltbl::LightPointEmission>&& light);

	// @brief Добавление света
	// @param lightType - Тип (point или cone)
	// @param light - Объект света
	void Add(const string& lightType, shared_ptr<ltbl::LightPointEmission>&& light);

	// @param key - Ключ
	// @return Объект света по ключу
	shared_ptr<ltbl::LightPointEmission>& Get(const string& key);

	// @brief Рисование
	void Draw();

	// @return Пуст ли контейнер
	bool IsEmpty() const;

	// @brief Очистка контейнера
	void Reset();		

private:
	// @brief Счётчик света
	int m_Score = 0;		
	
	// @brief Массив объектов света
	map<string, shared_ptr<ltbl::LightPointEmission>> m_Data;

	// @brief Шейдеры для рендера света.
	Shader unshadowShader, lightOverShapeShader;

	// @brief Текстура света (Круг)
	Texture pointLightTexture;

	// @brief Текстура света (Треугольник)
	Texture ConeLightTexture;
		
	// @brief Текстура полутени
	Texture penumbraTexture;

	sf::RenderStates lightRenderStates;

	// @brief Спрайт света
	sf::Sprite lightSprite;					
};

#endif /// LIGHT_INCLUDE