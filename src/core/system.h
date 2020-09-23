///==========================================================================
///	Модуль:		Система
///				Глобальная область библиотек, констант, макросов, переменных
///	Автор:		Андрей Гребенщиков
///	Год:		2019
///==========================================================================

#ifndef SYSTEM_INCLUDE
#define SYSTEM_INCLUDE

#include <ltbl/lighting/LightSystem.h>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <functional>
#include "any.h"
#include <math.h>
#include <TGUI/TGUI.hpp>
#include <cstdlib>
#include "effects.h"
#include "animation.h"
#include "sLevel.h"
#include "../../tinyxml/tinyxml.h"
#include "../gui/dialogs.h"
#include "../gui/Menu.h"
#include <sstream>

using namespace std;
using namespace sf;

// @brief Если нажата клавища клавиатуры
#define KEY_PRESSED(key) Keyboard::isKeyPressed(Keyboard::key)

// @brief Если нажата клавища мышки
#define MOUSE_PRESSED(key) Mouse::isButtonPressed(Mouse::key)

// @brief Взятие строки у XML элемента
#define XML_TO_STRING(cls, var) cls->Attribute(var)

// @brief Взятие целочисленного числа у XML элемента
#define XML_TO_INT(cls, var) stoi(cls->Attribute(var))

// @brief Взятие вещественного числа у XML элемента
#define XML_TO_FLOAT(cls, var) stof(cls->Attribute(var))

// @brief Если нажата клавища с клавиатуры с шаблона
#define IsKeyBoard(key) Keyboard::isKeyPressed(System::m_KeyBoard[key])

// @brief Возвращение строкового представления функциональной кнопки
#define GetStrKeyBoard(key) System::GetKeyBoard(System::m_KeyBoard[key])

// @brief Путь до папки с текстурами
const string PATH_TEXTURES = "resources/materials/textures/";

// @brief Путь до папки с шрифтами
const string PATH_FONTS = "resources/materials/fonts/";

// @brief Путь до папки с картами
const string PATH_MAPS = "resources/maps/";

// @brief Путь до папки с XML файлами (Настройками)
const string PATH_XMLS = "resources/xmls/";

// @brief Степень многочлена
const float DEG = 57.29582f;

// @brief Радиан
const float RAD = 0.017453f;

// @brief Клавищи, доступные пользователю для игры
const map<string, Keyboard::Key> KeyBoard =
{
	{"a", Keyboard::Key::A},
	{"b", Keyboard::Key::B},
	{"c", Keyboard::Key::C},
	{"d", Keyboard::Key::D},
	{"e", Keyboard::Key::E},
	{"f", Keyboard::Key::F},
	{"g", Keyboard::Key::G},
	{"h", Keyboard::Key::H},
	{"i", Keyboard::Key::I},
	{"j", Keyboard::Key::J},
	{"k", Keyboard::Key::K},
	{"l", Keyboard::Key::E},
	{"m", Keyboard::Key::M},
	{"n", Keyboard::Key::N},
	{"o", Keyboard::Key::O},
	{"p", Keyboard::Key::P},
	{"q", Keyboard::Key::Q},
	{"r", Keyboard::Key::R},
	{"s", Keyboard::Key::S},
	{"t", Keyboard::Key::T},
	{"u", Keyboard::Key::U},
	{"v", Keyboard::Key::V},
	{"w", Keyboard::Key::W},
	{"x", Keyboard::Key::X},
	{"y", Keyboard::Key::Y},
	{"z", Keyboard::Key::Z},
	{"0", Keyboard::Key::Num0},
	{"1", Keyboard::Key::Num1},
	{"2", Keyboard::Key::Num2},
	{"3", Keyboard::Key::Num3},
	{"4", Keyboard::Key::Num4},
	{"5", Keyboard::Key::Num5},
	{"6", Keyboard::Key::Num6},
	{"7", Keyboard::Key::Num7},
	{"8", Keyboard::Key::Num8},
	{"9", Keyboard::Key::Num9},
	{"space", Keyboard::Key::Space},
	{"left", Keyboard::Key::Left},
	{"right", Keyboard::Key::Right},
	{"up", Keyboard::Key::Up},
	{"down", Keyboard::Key::Down}
};

// @brief Система
class System
{
public:
	// @brief Конструктор
	System() = default;

	// @brief Деструктор
	~System() = default;

	// @return Видимая область камеры игрока
	static FloatRect GetVisibleRect();

	// @brief В m_MousePosition устанавливается позиция мыши
	static void	LocationMouse();	

	// @brief Форматирование строки
	// @param message - сообщение
	// @param args - аргументы
	// @return Полученная строка
	// Обращение к элементу args - {ИНДЕКС}.
	template<class ... Args>
	static constexpr String GetFormat(const String& message, Args&& ... args);

	// @brief Приведение к Unicode строке
	// @param value - Значение
	// @return Число, представленное в Unicode строке.
	template<class T>
	static String ToString(T value);

	// @brief Анти-баг, т.к. стандартный класс wstringstream не может работать с sf::String.
	// @param value - Строка
	// @return Эта же строка.
	// Используется потому-что GetFormat использует ToString в Args... и там есть вероятность получения sf::String.
	static String ToString(const String& str);

	// @brief Приведение строкового представления массива в вектор с T типом
	// @param str - Строка
	// @return Вектор
	template<class T>
	static constexpr vector<T> StringArrayToArray(const string& str);

	// @param p1 - Первая точка
	// @param p2 - Вторая точка
	// @return Угол между точками
	static float GetAngle(const Vector2f& p1, const Vector2f& p2);

	// @param speed - Скорость перемещения
	// @param angle - Угол перемещения
	// @param time - Время, прошедшее с предыдущего кадра
	// @return Направление движения в соответствии с скоростью и углом
	static Vector2f MoveToAngle(float speed, float angle);

	// @brief Исполнение функций при обновлении виджета интерфейса
	static void DelegateWidgetUpdate();

	// @brief Рандомное целочисленное число
	// @param left - Левый интервал
	// @param right - Правый интервал
	// @return Полученное значение
	static int GetRandInt(int left, int right);

	// @brief Рандомное вещественное число
	// @param left - Левый интервал
	// @param right - Правый интервал
	// @return Полученное значение
	static float GetRandFloat(float left, float right);

	// @param key - Ключ
	// @return Enum код ключа
	static Keyboard::Key GetKeyBoard(const string& key);

	// @param keyBoard - Enum клавищи
	// @return Строковое представление клавищи
	static String GetKeyBoard(const Keyboard::Key& keyBoard);

	// @brief Запустить ли паузу
	static bool m_isPause;

	// @brief Прошедшее время
	static float m_Time;

	// @brief Объект окна
	static RenderWindow* m_Window;
	
	// @brief Интерфейс
	static tgui::Gui* m_Gui;

	// @brief Диалоги
	static Dialog::Core* m_Dialogs;

	// @brief Меню
	static MenuManagament* m_Menu;

	// @brief Текстуры
	static map<string, Texture> m_Textures;

	// @brief Шрифты
	static map<string, Font> m_Fonts;

	// @brief Шаблоны (<название класса, <название свойства, строковое значение>>)	
	static map<string, map<string, string>> m_Templates;

	// @brief Делегат, вызывающийся при обновлении виджета по ключу
	static map<string, function<void (tgui::Widget::Ptr widget)>> m_WidgetsUpdate;

	// @brief Позиция мыши
	static Vector2f m_MousePosition;

	// @brief Разрешение
	static Vector2f m_Permission;

	// @brief Ширина и высота карты
	static Vector2f m_MapSize;

	// @brief Глобальная система света и тени	
	static ltbl::LightSystem m_LightSystem;

	// @brief Клавиатура
	static map<string, Keyboard::Key> m_KeyBoard;

	// @brief Эффекты
	static map<string, Effects::Base*> m_Effects;

	// @brief Анимации
	static map<string, shared_ptr<Animations>> m_Animations;

	// @brief Уровни
	static vector<shared_ptr<sLevel>> m_Levels;

	// @brief Объект камеры
	static View m_Camera;

	// @brief Если началась игра
	static bool m_IsRunGame;
};

template<class T>
static String System::ToString(T value)
{
	std::wostringstream os;
	os << value;

	return os.str();
};

template<class ...Args>
inline constexpr String System::GetFormat(const String& message, Args&& ...args)
{
	String result;
	String sArgs[] = { ToString(args)... };

	for (size_t i = 0; i < message.getSize(); i++)
	{
		if (message[i] != '{')
		{
			result += message[i];
		}
		else
		{
			i++;
			string index;

			while (i < message.getSize() && message[i] != '}')
			{
				index += message[i];
				i++;
			}

			result += sArgs[(size_t)stoi(index)];
		}
	}

	return result;
};

template<class T>
inline constexpr vector<T> System::StringArrayToArray(const string& str)
{
	AnySTD _any;
	_any.StringToValue(str);
	vector<AnySTD> _any_vec = _any;

	vector<T> _vec;
	for (auto& _any_vec_el : _any_vec)
		_vec.push_back(_any_vec_el.Get<T>());

	return _vec;
};

#endif /// SYSTEM_INCLUDE