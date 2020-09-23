///==========================================================================
///	������:		�������
///	�����:		������ �����������
///	���:		2019
///==========================================================================


#ifndef LEVEL_INCLUDE
#define LEVEL_INCLUDE

#include "system.h"
#include "light.h"
#include "..\entity\entity.h"
#include "..\entity\player.h"
#include "..\entity\bot.h"
#include "..\entity\movingPlatform.h"

// @brief �������
class Level
{
public:
	// @brief ����������� � ����������
	// @param file - �������� .tmx �����
	// @param light - ��������� �� ������ �����
	// @param templatesWeapons - ��������� �� ������ �������� ������
	Level(const string& file, Light* light, map<string, shared_ptr<Weapon>>* templatesWeapons);

	// @brief ����������
	~Level() = default;

	// @brief ����������
	void Update();		

	// @brief ���������
	void Draw();			

	// @return ����
	Entity::Layers& GetLayers();

	// @param type - ��� �������
	// @return ������ ���������� ����
	shared_ptr<Entity>& GetEntityByType(const string& type);

	// @param type - ��� �������
	// @return ��� ������� ���������� ����
	Entity::Entities& GetEntitiesByType(const string& type);

	// @brief ���������� �������
	// @param layer - ����
	// @param className - ��� �������
	// @param entity - ������
	// @param properties - �������� � �����
	// ����� ��������� ������� �������� �� ������� � � �����
	template<class T>
	void AddEntity(const string& layer, const string& className, T& entity, map<string, string>&& properties);

	// @return ������������ �� ������� �� ��������� �������
	bool IsNextLevel() const;		

private:
	// @brief ���������� ��������� ����
	// ����� � ������ ��������� ���� ��������� ������� �� �������� ����
	// � �������� ���������� � ����������� ������� � �� ���� ��������
	void UpdateLayersValide();

	// @brief ����������� ��������
	// @param layer - ����
	// @param className - ��� �������
	// @param sprite - ������ �������
	// @param properties - �������� � �����
	// ����� ���������� ����������� �������� �� �������� ������� � ������ ��� ����
	void RegisterEntities(const string& layer, const string& className, const Sprite& sprite, map<string, string>&& properties);

	// @brief �����
	// @param layer - ����
	// @param entity - ������
	// ���� ������ "�����������", ��������� ���� � ������������ �����������
	void Fire(const string& layer, shared_ptr<Entity>&& entity);

	// @brief ����������� �� ������� �� ��������� �������
	bool m_isNextLevel = false;

	// @brief ����
	Light* m_Light;

	// @brief ������� ������
	map<string, shared_ptr<Weapon>>* m_TemplatesWeapons;

	// @brief ����
	Entity::Layers m_Layers;

	// @brief �������� ����
	Entity::Layers m_LayersValide;
};

template<class T>
inline void Level::AddEntity(const string& layer, const string& className, T& entity, map<string, string>&& properties)
{
	// ��������� ������� ��������
	for (auto& property : properties)
	{
		entity.SetProperty(property.first, property.second);
	}

	// ��������� �������� � ������� � ��������� �� � ������
	for (auto& property : System::m_Templates[className])
	{
		entity.SetProperty(property.first, property.second);
	}

	// ��������� ����
	entity.SetType(className);	

	// ��������� ��������� ��������
	if (System::m_Animations.find(className) != System::m_Animations.end())
	{
		entity.SetAnimation(std::move(System::m_Animations[className]));
	}

	// ��������� ��������� ������ �������
	entity.To<Player>().SetWeaponsProperty(*m_TemplatesWeapons);

	// �������� �������
	entity.Load();

	// ���������� ������� � ���������� ������ ��������
	m_Layers[layer].push_back(make_shared<T>(entity));
};

#endif /// LEVEL_INCLUDE