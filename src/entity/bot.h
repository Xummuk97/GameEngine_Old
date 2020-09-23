///==========================================================================
///	������:		���
///	�����:		������ �����������
///	���:		2019
///==========================================================================

#ifndef BOT_INCLUDE
#define BOT_INCLUDE

#include "player.h"
#include "messages.h"

// @brief ���
class Bot : public Player
{
public:
	// @brief ��� ����
	enum struct BotType
	{
		Friend, // @brief ����
		Enemy // @brief ����
	};

	// @brief �������� ����
	enum struct Action
	{
		Stop, // @brief ������
		Move // @brief �������������
	};

	// @brief �����������
	Bot();

	// @brief ����������
	virtual ~Bot();

	void Load();

	// @brief ����������
	void Update();

	// @brief �����������
	// @param coord - �� ����� ��� ������������ (X ��� Y)
	void Intersects(char coord = 'y');

	// @brief �����
	void Fire();

	// @brief ��������� ������� ������� ������� ������
	// ������ ������� ����� ����� ������� �����������������, ��� ���� �����
	// � ��� �������� � ���� ��� ������� �� ������, �� ���������� ����������� ��������
	void SetSizeVisiblePlayerProperty();

	// @brief �������� �� ����������� ���� � ��������� ������������
	// @param angle - ����
	// @param player - ������� ������ (�� ������� ������� ������ ��� ����)
	bool IsBulletCollision(float angle, const FloatRect& player);

	// @return ��������� �� ������� ������� ��������
	bool IsAbyss();

	// @brief ��������� ���� ����
	// @param type - ���
	void SetBotType(const BotType& type);

	// @return ��� ����
	BotType GetBotType() const;

	// @brief ��������� �������� ����
	// @param action - ��������
	void SetAction(const Action& action);

	// @return �������� ����
	Bot::Action GetAction() const;

private:
	// @brief �������� ������� ��������� ������ �� ������
	void SetRectPlayer();

	// @brief ��� �������
	BotType m_type = BotType::Enemy;

	// @brief �������� �������
	Bot::Action m_action = Bot::Action::Move;
};

#endif /// BOT_INCLUDE