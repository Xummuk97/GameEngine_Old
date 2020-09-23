///==========================================================================
///	������:		�������
///	�����:		������ �����������
///	���:		2019
///==========================================================================

#ifndef PERSON_INCLUDE
#define PERSON_INCLUDE

#include <SFML/System/String.hpp>
#include <SFML/Graphics/Color.hpp>

using namespace std;
using namespace sf;

// @brief �������
struct Person
{
	// @brief ���� �����
	Color nameColor;
	
	// @brief ���� ������
	Color textColor;

	// @brief ���
	String name;

	// @brief �����������
	Person()
	{}

	// @brief ����������� � ����������
	// @param name - ��� 
	// @param nameColor - ���� �����
	// @param textColor - ���� ������
	Person(const String& name, const Color& nameColor, const Color& textColor) :
		name(name), nameColor(nameColor), textColor(textColor)
	{}
};

const Person PERSON_PLAYER = Person(L"�����", Color::White, Color::Black);

#endif /// PERSON_INCLUDE