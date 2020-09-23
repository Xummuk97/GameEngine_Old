///==========================================================================
///	������:		��������
///	�����:		������ �����������
///	���:		2019
///==========================================================================

#ifndef TELEPORT_INCLUDE
#define TELEPORT_INCLUDE

#include "messages.h"
#include "physix.h"

// @brief ��������
class Teleport
	: public Physix
{
public:
	// @brief �����������
	Teleport() = default;

	// @brief ����������
	~Teleport() = default;

	// @brief ����������
	void Update();

	// @brief �����������
	// @param coord - �� ����� ��� ������������ (X ��� Y)
	void Intersects(char coord = 'y');
};

#endif /// TELEPORT_INCLUDE