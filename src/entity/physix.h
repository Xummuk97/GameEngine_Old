///==========================================================================
///	������:		������
///	�����:		������ �����������
///	���:		2019
///==========================================================================

#ifndef PHYSIX_INCLUDE
#define PHYSIX_INCLUDE

#include "entity.h"

// @brief ������
class Physix : public Entity
{
public:
	// @brief �����������
	Physix();

	// @brief ����������
	virtual ~Physix();

	// @brief �������� �����
	// @param speed - ��������
	// @param isCheckIntersects - ��������� �������� �� �����������
	virtual void GoLeft(float speed = 0.0f, bool isCheckIntersects = true);

	// @brief �������� ������
	// @param speed - ��������
	// @param isCheckIntersects - ��������� �������� �� �����������
	virtual void GoRight(float speed = 0.0f, bool isCheckIntersects = true);

	// @brief �������� �����
	// @param speed - ��������
	// @param isCheckIntersects - ��������� �������� �� �����������
	virtual void GoUp(float speed = 0.0f);

	// @brief �������� ����
	// @param speed - ��������
	// @param isCheckIntersects - ��������� �������� �� �����������
	virtual void GoDown(float speed = 0.0f);

	// @brief ��������� ����
	// @param layers - ����
	void SetLayers(Layers* layers);

	// @return �� ����� �� ������
	void IsGround();

	// @param rect - �������
	// @return �� ����� �� �������
	bool IsGround(const FloatRect& rect);

protected:
	// @brief ������������ ������������
	virtual void DynamicMove();

	// @brief ������
	virtual void GoJump();

	// @brief ����� �������� direction �� up
	virtual void DirectionToUp();

	// @brief ����� �������� direction �� down
	virtual void DirectionToDown();

	// @brief �����������
	// @param coord - �� ����� ��� ������������ (X ��� Y)
	virtual void Intersects(char coord = 'y');

	// @brief ������������
	// @param _rect - �������
	virtual void Collision(char coord, const FloatRect& _rect);

	// @brief ����������� �� ��������
	virtual void Stair();

	// @brief ��������� ����������� �� ��������
	virtual void StairClear();

	// @brief ��������� ������
	virtual void JumpClear();

	// @brief ����������� � �������� �������
	// @param layer - ����
	// @param entity - ������
	// @return ���� �� ��������� �����������
	bool IsIntersectScripts(const string& layer, const shared_ptr<Entity>& entity);

	// @brief ����������� � �������� ������������
	// @param layer - ����
	// @param entity - ������
	// @param coord - �� ����� ��� ������������ (X ��� Y)
	// @return ���� �� ��������� �����������
	bool IsIntersectCollision(const string& layer, const shared_ptr<Entity>& entity, char coord);

	// @brief ����������� � �������� ��������� ���������� ����
	// @param layer - ����
	// @return ���� �� ��������� �����������
	bool IsIntersectDamageAll(const string& layer);

	// @brief ����������� � �������� ���������� ���������� ����
	// @param layer - ����
	// @return ���� �� ��������� �����������
	bool IsIntersectDamage(const string& layer);

	// @brief ����������� � �������� ������������ ������ �� ����
	// @param layer - ����
	// @param entity - ������
	// @return ���� �� ��������� �����������
	bool IsIntersectCollisionUp(const string& layer, const shared_ptr<Entity>& entity);

	// @brief ����������� � �������� ��������
	// @param entity - ������
	// @param direction - ��������
	// @return ���� �� ��������� �����������
	bool IsIntersectStair(const shared_ptr<Entity>& entity, const string& direction);

	// @brief ����������� � �������� ��������������� ���������
	// @param entity - ������
	// @return ���� �� ��������� �����������
	bool IsIntersectMovingPlatform(const shared_ptr<Entity>& entity);

	// @brief ����������� � �������� ��������������� ���������, ���� ��� ������������ ����
	// @param entity - ������
	// @return ���� �� ��������� �����������
	bool IsIntersectMovingPlatformBugFix(const shared_ptr<Entity>& entity);

	// @brief ����������� � �������� ������� ������������, ����� ��������� �����
	// @param entity - ������
	// @return ���� �� ��������� �����������
	bool IsIntersectCollisionAngleLeft(const shared_ptr<Entity>& entity);

	// @brief ����������� � �������� ������� ������������, ����� ��������� ������
	// @param entity - ������
	// @return ���� �� ��������� �����������
	bool IsIntersectCollisionAngleRight(const shared_ptr<Entity>& entity);

	// @brief ���� �� ����������� ���� � �������
	// @param entity - ������
	// @return ���� �� ��������� �����������
	bool IsPlayerIntersectBot(const shared_ptr<Entity>& entity);

	// @brief ���� �� ����������� ��������� � �������
	// @param entity - ������
	// @return ���� �� ��������� �����������
	bool IsPlayerTeleport(const shared_ptr<Entity>& entity);

	// @brief ���� �� ����������� ������ � ������������� �����, ��� ������� �������
	// @param entity - ������
	// @return ���� �� ��������� �����������
	bool IsPlayerDialog(const shared_ptr<Entity>& entity);

	// @brief ����������� � ��������.
	bool IsPlayerBonus(const string& layer, const shared_ptr<Entity>& entity);

	// @brief ���� �� �������
	// @return ���� �� ��������� ��������
	bool IsFallDamage();

	// @brief ����������� � �������� �������
	// @param script - ������ �������
	void Scripts(Entity* script);

	// @brief ����
	Layers* m_Layers;
};

#endif /// PHYSIX_INCLUDE