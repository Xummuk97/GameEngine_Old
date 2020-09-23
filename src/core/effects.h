///==========================================================================
///	Модуль:		Эффекты (Подключаемые шейдеры)
///	Автор:		Андрей Гребенщиков
///	Год:		2019
///==========================================================================


#ifndef EFFECTS_INCLUDE
#define EFFECTS_INCLUDE

#include <SFML/Graphics/Shader.hpp>
#include <string>

using namespace sf;

namespace Effects
{
	// @brief Путь до папки с шейдерами
	const std::string PATH_SHADERS = "resources/shaders/";

	// @brief Тип эффекта
	enum struct Type
	{
		Blur, // @brief Размытие
		Color // @brief Меняет цвет
	};

	// @brief Баз эффектов
	class Base
	{
	public:
		// @brief Конструктор с параметром
		// @param type - Тип эффекта
		Base(const Effects::Type& type);

		// @brief Деструктор
		~Base() = default;

		// @brief Приведение к T (шаблону)
		// @return Приведённый объект
		template <class T>
		T& Cast() { return *(T*)this; };

		// @return Указатель на шейдер
		Shader* GetShader();

		// @return Тип эффекта
		Effects::Type& GetType();

	protected:
		// @brief Тип эффекта
		Effects::Type m_type;

		// @brief Шейдер
		Shader m_shader;
	};

	// @brief Blur (Размытие)
	class Blur : public Base
	{
	public:
		// @brief Конструктор
		Blur();

		// @brief Конструктор с параметром
		// @param offsetFactor - Направление и сила размытия
		Blur(const Vector2f& offsetFactor);

		// @brief Деструктор
		virtual ~Blur() = default;

		// @brief Установка направления и силы размытия
		// @param offsetFactor - Направление и сила размытия
		void SetOffsetFactor(const Vector2f& offsetFactor);
	};

	// @brief Color (Цвет)
	class Color : public Base
	{
	public:
		// @brief Конструктор
		Color();

		// @brief Деструктор
		~Color();

		// @brief Установка цвета
		// @param color - Цвет
		void SetColor(const Vector3f& color);
	};
}

#endif /// EFFECTS_INCLUDE