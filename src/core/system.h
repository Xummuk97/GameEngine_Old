///==========================================================================
///	������:		�������
///				���������� ������� ���������, ��������, ��������, ����������
///	�����:		������ �����������
///	���:		2019
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

// @brief ���� ������ ������� ����������
#define KEY_PRESSED(key) Keyboard::isKeyPressed(Keyboard::key)

// @brief ���� ������ ������� �����
#define MOUSE_PRESSED(key) Mouse::isButtonPressed(Mouse::key)

// @brief ������ ������ � XML ��������
#define XML_TO_STRING(cls, var) cls->Attribute(var)

// @brief ������ �������������� ����� � XML ��������
#define XML_TO_INT(cls, var) stoi(cls->Attribute(var))

// @brief ������ ������������� ����� � XML ��������
#define XML_TO_FLOAT(cls, var) stof(cls->Attribute(var))

// @brief ���� ������ ������� � ���������� � �������
#define IsKeyBoard(key) Keyboard::isKeyPressed(System::m_KeyBoard[key])

// @brief ����������� ���������� ������������� �������������� ������
#define GetStrKeyBoard(key) System::GetKeyBoard(System::m_KeyBoard[key])

// @brief ���� �� ����� � ����������
const string PATH_TEXTURES = "resources/materials/textures/";

// @brief ���� �� ����� � ��������
const string PATH_FONTS = "resources/materials/fonts/";

// @brief ���� �� ����� � �������
const string PATH_MAPS = "resources/maps/";

// @brief ���� �� ����� � XML ������� (�����������)
const string PATH_XMLS = "resources/xmls/";

// @brief ������� ����������
const float DEG = 57.29582f;

// @brief ������
const float RAD = 0.017453f;

// @brief �������, ��������� ������������ ��� ����
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

// @brief �������
class System
{
public:
	// @brief �����������
	System() = default;

	// @brief ����������
	~System() = default;

	// @return ������� ������� ������ ������
	static FloatRect GetVisibleRect();

	// @brief � m_MousePosition ��������������� ������� ����
	static void	LocationMouse();	

	// @brief �������������� ������
	// @param message - ���������
	// @param args - ���������
	// @return ���������� ������
	// ��������� � �������� args - {������}.
	template<class ... Args>
	static constexpr String GetFormat(const String& message, Args&& ... args);

	// @brief ���������� � Unicode ������
	// @param value - ��������
	// @return �����, �������������� � Unicode ������.
	template<class T>
	static String ToString(T value);

	// @brief ����-���, �.�. ����������� ����� wstringstream �� ����� �������� � sf::String.
	// @param value - ������
	// @return ��� �� ������.
	// ������������ ������-��� GetFormat ���������� ToString � Args... � ��� ���� ����������� ��������� sf::String.
	static String ToString(const String& str);

	// @brief ���������� ���������� ������������� ������� � ������ � T �����
	// @param str - ������
	// @return ������
	template<class T>
	static constexpr vector<T> StringArrayToArray(const string& str);

	// @param p1 - ������ �����
	// @param p2 - ������ �����
	// @return ���� ����� �������
	static float GetAngle(const Vector2f& p1, const Vector2f& p2);

	// @param speed - �������� �����������
	// @param angle - ���� �����������
	// @param time - �����, ��������� � ����������� �����
	// @return ����������� �������� � ������������ � ��������� � �����
	static Vector2f MoveToAngle(float speed, float angle);

	// @brief ���������� ������� ��� ���������� ������� ����������
	static void DelegateWidgetUpdate();

	// @brief ��������� ������������� �����
	// @param left - ����� ��������
	// @param right - ������ ��������
	// @return ���������� ��������
	static int GetRandInt(int left, int right);

	// @brief ��������� ������������ �����
	// @param left - ����� ��������
	// @param right - ������ ��������
	// @return ���������� ��������
	static float GetRandFloat(float left, float right);

	// @param key - ����
	// @return Enum ��� �����
	static Keyboard::Key GetKeyBoard(const string& key);

	// @param keyBoard - Enum �������
	// @return ��������� ������������� �������
	static String GetKeyBoard(const Keyboard::Key& keyBoard);

	// @brief ��������� �� �����
	static bool m_isPause;

	// @brief ��������� �����
	static float m_Time;

	// @brief ������ ����
	static RenderWindow* m_Window;
	
	// @brief ���������
	static tgui::Gui* m_Gui;

	// @brief �������
	static Dialog::Core* m_Dialogs;

	// @brief ����
	static MenuManagament* m_Menu;

	// @brief ��������
	static map<string, Texture> m_Textures;

	// @brief ������
	static map<string, Font> m_Fonts;

	// @brief ������� (<�������� ������, <�������� ��������, ��������� ��������>>)	
	static map<string, map<string, string>> m_Templates;

	// @brief �������, ������������ ��� ���������� ������� �� �����
	static map<string, function<void (tgui::Widget::Ptr widget)>> m_WidgetsUpdate;

	// @brief ������� ����
	static Vector2f m_MousePosition;

	// @brief ����������
	static Vector2f m_Permission;

	// @brief ������ � ������ �����
	static Vector2f m_MapSize;

	// @brief ���������� ������� ����� � ����	
	static ltbl::LightSystem m_LightSystem;

	// @brief ����������
	static map<string, Keyboard::Key> m_KeyBoard;

	// @brief �������
	static map<string, Effects::Base*> m_Effects;

	// @brief ��������
	static map<string, shared_ptr<Animations>> m_Animations;

	// @brief ������
	static vector<shared_ptr<sLevel>> m_Levels;

	// @brief ������ ������
	static View m_Camera;

	// @brief ���� �������� ����
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