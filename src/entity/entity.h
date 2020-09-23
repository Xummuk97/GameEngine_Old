///==========================================================================
///	������:		������� ������
///	�����:		������ �����������
///	���:		2019
///==========================================================================

#ifndef ENTITY_INCLUDE
#define ENTITY_INCLUDE

#include "..\core\system.h"
#include "..\core\any.h"
#include "person.h"

// @brief ������� ������
class Entity
{
public:
	// @brief ������ ��������
	using Entities	= vector<shared_ptr<Entity>>;

	// @brief ���� (������ �������� ��������)
	using Layers	= map<string, Entities>;

	// @brief �����������
	Entity();

	// @brief ����������
	~Entity();

	// @brief ��������
	// ���������� ��� ������ �������� ������� �� �����
	virtual void Load();

	// @brief ����������
	virtual void Update();

	// @brief ���������
	virtual void Draw();

	// @brief ��������� ��������
	// @param animation - ��������
	void SetAnimation(const shared_ptr<Animations>& animation);

	// @brief ��������� �������
	// @param sprite - ������
	void SetSprite(const Sprite& sprite);

	// @return ������
	Sprite* GetSprite();

	// @brief ��������� ��������
	// @param key - ����
	// ���� �� ������� �������� key, �� �������� ������ �� �������
	void SetTexture(const string& key = "null");

	// @brief ��������� �������
	// @param rect - �������
	void SetRect(const IntRect& rect);

	// @brief ��������� ������� �� �������
	void SetRectByProperty();

	// @brief �����������
	// @param pos - ��������
	void Move(const Vector2f& pos);

	// @brief �����������
	// @param x - �������� X
	// @param y - �������� Y
	void Move(float x, float y);

	// @brief ��������� �������
	// @param pos - �������
	void SetPosition(const Vector2f& pos);

	// @brief ��������� �������
	// @param x - X
	// @param y - Y
	void SetPosition(float x, float y);

	// @brief ��������� ��������
	// @param key - ����
	// @param value - ��������
	template<class T>
	void SetProperty(const string& key, T value);

	// @brief ��������� ��������
	// @param key - ����
	// @param value - ��������� ��������
	void SetProperty(const string& key, const string& value);

	// @brief ��������� ����
	// @param type - ���
	void SetType(const string& type);

	// @return ���
	string GetType() const;

	// @return �������
	Vector2f GetPosition() const;

	// @return �������
	FloatRect GetRect() const;

	// @param key - ����
	// @return ��������
	AnySTD& GetProperty(const string& key);

	// @brief ����������� �������� � �����������
	// @param key - ����
	// @return ���������� ��������
	template<class T>
	T GetProperty(const string& key);
	
	// @param entity - ������
	// @return ���� �� ����������� � ��������
	bool IsIntersects(const Entity& entity) const;

	// @param key - ����
	// @return ���������� �� ��������
	bool IsFindProperty(const string& key) const;

	// @param entity - ������
	// @return ���� �� ����������� � ��������
	bool operator == (const Entity& entity) const;

	// @param entity - ������
	// @return ��� �� ����������� � ��������
	bool operator != (const Entity& entity) const;

	// @param type - ���
	// @return ��������� �� ���
	bool IsType(const string& type) const;

	// @return ���������� �������
	template<class T>
	constexpr T& To();

	// @brief ������ ����� �����
	// @param time - �����
	void DeadThroughTime(float time);

	// @brief ������
	// @param time - ����� ��������
	// @param damage - ���� ������
	void Push(float time, float damage);

	// @brief ����
	// @param damage - ����
	void Damage(int damage);

	// @brief ��������� ��������
	void SetScripts(function<bool (Entity* entity, string scriptName)>&& scripts);

	// @brief ��������� �������
	// @param person - �������
	void SetPerson(const Person& person);

	// @brief ��������� ������� �� ��������
	void SetPersonByProperty();

	// @return �������
	Person GetPerson();

	// @return ������� 
	operator Vector2f() const { return m_Sprite.getPosition(); }

	// @return �������
	operator FloatRect() const { return m_Sprite.getGlobalBounds(); }

	// @return �������
	operator Person() const { return m_Person; }

protected:
	// @brief ������� ������� (���� ������ �� ������� X ������ X, �� �����, ����� ������)
	// @param x - ������� ��� �����������
	void IsFlipAnimation(float x);

	// @brief ������ ��������� �����
	float m_timerDmg = 0.0f;

	// @brief ��������
	map<string, AnySTD> m_Property;

	// @brief ��������
	shared_ptr<Animations> m_Animation;

	// @brief ���
	string m_Type = "null";

	// @brief �������
	Person m_Person = Person(L"������", Color::White, Color::Black);

	// @brief �������
	function<bool (Entity* entity, string scriptName)> m_scripts;

	// @brief ������
	Sprite m_Sprite;
};

template<class T>
inline void Entity::SetProperty(const string& key, T value)
{ m_Property[key] = value; }

template<class T>
inline T Entity::GetProperty(const string& key)
{
	return m_Property[key].Get<T>();
};

template<class T>
inline constexpr T& Entity::To()
{ return *(T*)(this); };


#endif /// ENTITY_INCLUDE