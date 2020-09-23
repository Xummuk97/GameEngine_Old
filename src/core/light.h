///==========================================================================
///	������:		����
///	�����:		������ �����������
///	���:		2019
///==========================================================================

#ifndef LIGHT_INCLUDE
#define LIGHT_INCLUDE

#include "system.h"

// @brief ��������� �����
class Light
{
public:
	// @brief �����������
	Light();			

	// @brief ����������
	~Light() = default;

	// @brief ���������� �����
	// @param lightType - ��� (point ��� cone)
	// @param key - ����
	// @param light - ������ �����
	void Add(const string& lightType, const string& key, shared_ptr<ltbl::LightPointEmission>&& light);

	// @brief ���������� �����
	// @param lightType - ��� (point ��� cone)
	// @param light - ������ �����
	void Add(const string& lightType, shared_ptr<ltbl::LightPointEmission>&& light);

	// @param key - ����
	// @return ������ ����� �� �����
	shared_ptr<ltbl::LightPointEmission>& Get(const string& key);

	// @brief ���������
	void Draw();

	// @return ���� �� ���������
	bool IsEmpty() const;

	// @brief ������� ����������
	void Reset();		

private:
	// @brief ������� �����
	int m_Score = 0;		
	
	// @brief ������ �������� �����
	map<string, shared_ptr<ltbl::LightPointEmission>> m_Data;

	// @brief ������� ��� ������� �����.
	Shader unshadowShader, lightOverShapeShader;

	// @brief �������� ����� (����)
	Texture pointLightTexture;

	// @brief �������� ����� (�����������)
	Texture ConeLightTexture;
		
	// @brief �������� ��������
	Texture penumbraTexture;

	sf::RenderStates lightRenderStates;

	// @brief ������ �����
	sf::Sprite lightSprite;					
};

#endif /// LIGHT_INCLUDE