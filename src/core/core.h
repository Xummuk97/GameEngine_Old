///==========================================================================
///	������:		����
///	�����:		������ �����������
///	���:		2019
///==========================================================================

#ifndef CORE_INCLUDE
#define CORE_INCLUDE

#include "system.h"
#include "level.h"
#include "light.h"
#include "Settings.h"
#include "..\gui\widgets\image.h"
#include "..\entity\weapon.h"

// @brief ����
class Core
{
public:
	// @brief �����������
	Core();

	// @brief ����������
	~Core() = default;

	// @brief ������ ����������
	void Run();

	// @brief ������� �� ��������� �������
	void NextLevel();

private:
	// @brief �������� �������� ����
	void CreateBaseWindow();

	// @brief ��������� ���������
	void OtherInit();

	// @brief ����� �������� ��������
	void ThreadProcess();

	// @brief ������� �������	
	size_t m_LevelsScore = 0;																			

	// @brief ���
	Widget::Image* m_BackGround;	

	// @brief ������
	Widget::Image* m_Cursor;																	

	// @brief ��������� ��� ������
	Light* m_Light;																				

	// @brief ������� �������	
	Level* m_Level;																						

	// @brief �����
	Thread* m_thread;

	// @brief ������� ������
	map<string, shared_ptr<Weapon>> m_templatesWeapons;											

	// @brief ����		
	Clock m_Clock;																					

	// @brief �������		
	Event m_Event;
};

#endif /// CORE_INCLUDE