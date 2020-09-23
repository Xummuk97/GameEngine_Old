///==========================================================================
///	������:		������������� ��� ������ � �������� �� ��������������
///				����� � ��������.
///	�����:		������ �����������
///	���:		2019
///==========================================================================

#ifndef ANY_INCLUDE
#define ANY_INCLUDE

#include <iostream>
#include <string>
#include <vector>
#include <any>

using namespace std;

// @brief ������������� ��� ������ �� ������ std::any
class AnySTD
{
public:
	// @brief �����������
	AnySTD() = default;	

	// @brief ����������� � ����������
	// @param value - ��������
	template<class T>
	AnySTD(T value);

	// @brief ����������
	~AnySTD() = default;

	// @brief ���������� �������� � T (�������) ����
	// @return ���������� ���
	template<class T>		
	constexpr operator T();

	// @brief ���������� �������� � T (�������) ����
	// @return ���������� ���
	template<class T>
	constexpr T Get();			

	// @brief ������� ������ � ��������
	// @param value - ��������� ��������
	// [] - array,
	// ����� (�� �����) - string,
	// true, false - bool,
	// ���� ���� ����� - float,
	// ����� int
	void StringToValue(const string& value);

private:
	// @brief ��������
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