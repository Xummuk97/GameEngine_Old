///==========================================================================
///	������:		��������������� ���������
///	�����:		������ �����������
///	���:		2019
///==========================================================================

#ifndef MOVING_PLATFORM_INCLUDE
#define MOVING_PLATFORM_INCLUDE

#include "entity.h"

// @brief ��������������� ���������
class MovingPlatform : public Entity
{
public:
	// @brief �����������
	MovingPlatform();

	// @brief ����������
	virtual ~MovingPlatform();

	// @brief ����������
	void Update();

	// @brief ������������
	void DynamicMove();

	// @brief �����������
	// @param coord - �� ����� ��� ������������ (X ��� Y)
	void Intersects(char coord = 'y');

	// @brief ��������� ����
	// @param layers - ����
	void SetLayers(Layers* layers);

private:
	// @brief ����
	Layers* m_Layers;
};

#endif /// MOVING_PLATFORM_INCLUDE