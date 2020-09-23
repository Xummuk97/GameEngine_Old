///==========================================================================
///	Модуль:		Система
///				Глобальная область библиотек, констант, макросов, переменных
///	Автор:		Андрей Гребенщиков
///	Год:		2019
///==========================================================================

#include "system.h"

RenderWindow*											System::m_Window;
View													System::m_Camera;
float													System::m_Time;
map<string, Texture>									System::m_Textures;
map<string, Font>										System::m_Fonts;
map<string, map<string, string>>						System::m_Templates;
Vector2f												System::m_MousePosition;
Vector2f												System::m_Permission;
ltbl::LightSystem										System::m_LightSystem;
Vector2f												System::m_MapSize;
tgui::Gui*												System::m_Gui;
map<string, Keyboard::Key>								System::m_KeyBoard;
map<string, function<void (tgui::Widget::Ptr widget)>>	System::m_WidgetsUpdate;
Dialog::Core*											System::m_Dialogs;
MenuManagament*											System::m_Menu;
bool													System::m_isPause;
map<string, Effects::Base*>								System::m_Effects;
map<string, shared_ptr<Animations>>						System::m_Animations;
vector<shared_ptr<sLevel>>								System::m_Levels;
bool													System::m_IsRunGame;

FloatRect System::GetVisibleRect()
{
	Vector2f center = m_Camera.getCenter(),
		     size = m_Camera.getSize();

	return FloatRect(center.x - size.x / 2.0f, center.y - size.y / 2.0f, size.x, size.y);
}

void System::LocationMouse()
{
	m_MousePosition = m_Window->mapPixelToCoords(Mouse::getPosition(*m_Window));
}

String System::ToString(const String& str)
{
	return str;
}

float System::GetAngle(const Vector2f& p1, const Vector2f& p2)
{
	return atan2f(p2.y - p1.y, p2.x - p1.x) * DEG;
}

Vector2f System::MoveToAngle(float speed, float angle)
{
	return Vector2f(cosf(angle), sinf(angle)) * speed * m_Time;
}

void System::DelegateWidgetUpdate()
{
	for (auto&& [name, func] : m_WidgetsUpdate)
	{
		if (auto widget = m_Gui->get(name))
		{
			func(m_Gui->get(name));
		}
		else cout << "[Core::DelegateWidgetUpdate] Widget " << name << " not found!" << endl;
	}
}

int System::GetRandInt(int left, int right)
{
	return left + rand() % (right + 1);
}

float System::GetRandFloat(float left, float right)
{
	return left + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (right - left)));
}

Keyboard::Key System::GetKeyBoard(const string& key)
{
	for (auto&& [name, value] : KeyBoard)
		if (name == key)
			return value;

	return Keyboard::Key();
}

String System::GetKeyBoard(const Keyboard::Key& keyBoard)
{
	for (auto&& [name, value] : KeyBoard)
		if (value == keyBoard)
			return name;

	return L"Undefined key board!";
}
