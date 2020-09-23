///==========================================================================
///	������:		��������
///	�����:		������ �����������
///	���:		2019
///==========================================================================

#ifndef ANIMATION_INCLUDE
#define ANIMATION_INCLUDE

#include <SFML/Graphics/Rect.hpp>
#include <vector>
#include <map>
#include <memory>
#include <string>
#include <iostream>

using namespace std;
using namespace sf;

// @brief ��������
class Animation
{
public:
	// @brief ����
	using Frame = shared_ptr<IntRect>;

	// @brief ���������
	using Ptr = shared_ptr<Animation>;

	// @brief �����������
	Animation() = default;

	// @brief ����������� � ����������
	// @param maxTime - �����, ����� ������� �������������� ����� ����� ��������
	Animation(float maxTime);

	// @brief ����������
	~Animation() = default;

	// @brief ���������� �����
	// @param rect - ������� ����� �� ��������
	void Add(Frame&& rect);

	// @brief ����������
	void Update();

	// @brief �����
	void Reset();	

	// @return ��� �� �������� ������� �� ����� ����
	bool IsNextFrame() const;

	// @return ������� ������� ��������
	IntRect& GetRectCurrentAnimation();

	// @return ������ ������
	vector<Frame>& GetData();								

	// @brief ����������� ��������
	// @param ��������/���������
	void SetFlip(bool isFlip);

	// @return ���������� �� �����������
	bool IsFlip() const;

private:
	// @brief �������� �� �����
	bool m_IsReset = false;

	// @brief �������� �� ������� �� ��������� ����
	bool m_IsNextFrame = false;	

	// �������������� �� �����������
	bool m_IsFlip = false;

	// @brief ������� �������
	float m_Time = 0.0f;

	// @brief �����, ����� ������� ����� �������������� ����� �����
	float m_MaxTime;	

	// @brief ������ ������� ��������
	size_t m_IndexAnimation = 0;		

	// @brief ������ ������
	vector<Frame> m_Data;					
};

// @brief ��������
class Animations
{
public:
	// @brief �����������
	Animations() = default;
	
	// @brief ����������
	~Animations() = default;

	// @brief ���������� ��������
	// @param key - ����
	// @param animation - ������ ��������
	void Add(const string& key, Animation::Ptr&& animation);	

	// @brief ��������� ������� ��������
	// @param key - ����
	void SetCurrentAnimation(const string& key);

	// @brief ���������� � ����������� ������� ��������
	Animation& UpdateAndGetCurrentAnimation();		

	// @brief ����������� ��������
	// @param ��������/���������
	void SetFlip(bool isFlip);

	// @return ���������� �� �����������
	bool IsFlip() const;

private:
	// �������������� �� �����������
	bool m_IsFlip = false;

	// @brief ������� �������
	float m_CurrentTime = 0.0f;							

	// @brief ������ ��������
	map<string, Animation::Ptr> m_Data;	

	// @brief ������� ��������	
	string m_CurrentAnimationFrame = "none";				
};

#endif /// ANIMATION_INCLUDE