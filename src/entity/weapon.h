///==========================================================================
///	������:		������
///	�����:		������ �����������
///	���:		2019
///==========================================================================

#ifndef WEAPON_INCLUDE
#define WEAPON_INCLUDE

#include "..\entity\entity.h"

// @brief ����
class Bullet 
	: public Entity
{
public:
	// @brief �����������
	Bullet();

	// @brief ����������
	virtual ~Bullet();

	// @brief ����������
	void Update();

	// @brief ��������� ����
	// @param p1 - ������ �����
	// @param p2 - ������ �����
	void SetAngle(const Vector2f& p1, const Vector2f& p2);

	// @brief ��������� ����
	// @param layers - ����
	void SetLayers(Layers* layers);

private:
	// @brief ����
	Layers* m_Layers = nullptr;
};

// @brief ������
class Weapon 
{
public:
	// @brief �����������
	Weapon();

	// @brief ����������
	virtual ~Weapon();

	// @brief ����������
	// @param p1 - ������ �����
	// @param p2 - ������ �����
	void Update(const Vector2f& p1, const Vector2f& p2);

	// @brief ���������
	void Draw();

	// @return ����
	Bullet* GetBullet();

	// @brief ������ ������
	Sprite* GetSprite();

	// @brief ��������� ����� ������
	// @param name - ���
	void SetName(const string& name);

	// @return ��� ������
	string GetName() const;

	// @brief ��������� �������� �������
	// @param textureName - �������� ��������
	// @param rect - �������
	void SetImageSprite(const string& textureName, const IntRect& rect);

private:
	// @brief ��� ������
	string m_name;

	// @brief ������ ������
	Sprite m_sprite;

	// @brief ����
	Bullet m_bullet;
};

// @brief ������
class Weapons 
{
public:
	// @brief �����������
	Weapons();

	// @brief ����������
	virtual ~Weapons();

	// @brief ���������� ������
	// @param key - ����
	// @param weapon - ������
	void Add(const string& key, Weapon weapon);

	// @brief ����������� ������ �� �����
	// @param key - ����
	// @return ������
	shared_ptr<Weapon> Get(const string& key);

	// @return ������
	map<string, shared_ptr<Weapon>> Get();

	// @brief ��������� �������� ������
	// @param key - ����
	void SetCurrentWeapon(const string& key);

	// @return ������� ������
	string GetCurrentWeapon() const;

	// @brief ��������� ����
	// @param isFire - �������� ��� ��������� �����
	void SetIsFire(bool isFire);

	// @param ������� �� �����
	bool GetIsFire() const;

	// @brief ����������
	// @param r1 - ������� ����������� �������
	// @param p2 - ����� ���������� �����
	shared_ptr<Bullet> Update(FloatRect r1, const Vector2f& p2);

	// @brief ���������
	void Draw();

private:
	// @brief ������� �� �����
	bool m_isFire = false;

	// @brief �������� ����� ������
	float m_delay = 0.0f;

	// @brief ������
	map<string, shared_ptr<Weapon>> m_data;

	// @brief ������� ������
	string m_currentWeapon;
};

#endif /// WEAPON_INCLUDE