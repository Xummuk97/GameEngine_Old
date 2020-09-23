///==========================================================================
///	������:		��������� ��������
///	�����:		������ �����������
///	���:		2019
///==========================================================================

#ifndef SETTINGS_INCLUDE
#define SETTINGS_INCLUDE

#include "system.h"
#include "animation.h"
#include "level.h"
#include "..\gui\widgets\image.h"
#include "..\entity\weapon.h"

// @brief ��������� ��������
class Settings 
{
public:
	// @brief �����������
	Settings() = default;
	
	// @brief ����������
	~Settings() = default;

	// @brief ������������� �������
	static void InitTextures();

	// @brief ������������� �������
	static void InitFonts();

	// @brief ������������� ��������
	static void InitTemplates();

	// @brief ������������� ������
	// @param templatesWeapons - ������� ������
	static void InitWeapons(map<string, shared_ptr<Weapon>>&& templatesWeapons);

	// @brief ������������ ��������
	static void InitAnimations();

	// @brief ������������� �������
	static void InitLevels();

	// @brief ������������� GUI
	// @param level - ������ ������
	static void InitGui(Level&& level);

	// @brief ������������� ��������
	// @param level - ������ ������
	static void InitScripts(Level&& level);

	// @brief ������������� �������
	// @param cursor - ������ �������
	static void InitCursor(Widget::Image* cursor);

	// @brief ������������� ����������
	static void InitKeyBoard();

	// @brief ������������� ��������
	// @param level - ������ ������
	static void InitDialogs(Level&& level);
};

#endif /// SETTINGS_INCLUDE