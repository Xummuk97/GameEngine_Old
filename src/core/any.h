///==========================================================================
///	Модуль:		Универсальный тип данных с расчётом на преобразование
///				строк в значение.
///	Автор:		Андрей Гребенщиков
///	Год:		2019
///==========================================================================

#ifndef ANY_INCLUDE
#define ANY_INCLUDE

#include <iostream>
#include <string>
#include <vector>
#include <any>

using namespace std;

// @brief Универсальный тип данных на основе std::any
class AnySTD
{
public:
	// @brief Конструктор
	AnySTD() = default;	

	// @brief Конструктор с параметром
	// @param value - Значение
	template<class T>
	AnySTD(T value);

	// @brief Деструктор
	~AnySTD() = default;

	// @brief Приведение значения к T (шаблону) типу
	// @return Приведённый тип
	template<class T>		
	constexpr operator T();

	// @brief Приведение значения к T (шаблону) типу
	// @return Приведённый тип
	template<class T>
	constexpr T Get();			

	// @brief Перевод строку в значение
	// @param value - Строковое значение
	// [] - array,
	// буква (не точка) - string,
	// true, false - bool,
	// если есть точка - float,
	// иначе int
	void StringToValue(const string& value);

private:
	// @brief Значение
	any m_value; 	
};

template<class T>
inline AnySTD::AnySTD(T value) :
	m_value(value)
{
};

template<class T>
constexpr inline AnySTD::operator T()
{ 
	return any_cast<T>(m_value);
};

template<class T>
constexpr inline T AnySTD::Get()
{ 
	return any_cast<T>(m_value); 
};

#endif /// ANY_INCLUDE