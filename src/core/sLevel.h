///==========================================================================
///	������:		��������� ������
///	�����:		������ �����������
///	���:		2019
///==========================================================================

#ifndef STRUCT_LEVEL_INCLUDE
#define STRUCT_LEVEL_INCLUDE

#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

// @brief ��������� ������
struct sLevel
{
	// @brief ���� ���������� ����
	Color colorAmbient;

	// @brief ���� ����� ������
	Color colorPlayer;

	// @brief ������� �������� ����
	IntRect rect;

	// @brief �������� ����� (� ����������� .tmx)
	string mapName;
		
	// @brief �������� �������� ����
	string textureName;
};

#endif /// STRUCT_LEVEL_INCLUDE