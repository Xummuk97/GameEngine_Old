///==========================================================================
///	Модуль:		Свет
///	Автор:		Андрей Гребенщиков
///	Год:		2019
///==========================================================================

#include "light.h"

Light::Light()
{
	// Инициализация объектов
	penumbraTexture.loadFromFile("resources/materials/light/penumbraTexture.png");
	penumbraTexture.setSmooth(true);

	pointLightTexture.loadFromFile("resources/materials/light/pointLightTexture.png");
	pointLightTexture.setSmooth(true);

	ConeLightTexture.loadFromFile("resources/materials/light/spotLightTexture.png");
	ConeLightTexture.setSmooth(true);

	unshadowShader.loadFromFile("resources/materials/light/unshadowShader.vert", "resources/materials/light/unshadowShader.frag");
	lightOverShapeShader.loadFromFile("resources/materials/light/lightOverShapeShader.vert", "resources/materials/light/lightOverShapeShader.frag");

	// Инициализация системы освещения
	System::m_LightSystem.create(ltbl::rectFromBounds(sf::Vector2f(-1000000.0f, -1000000.0f), sf::Vector2f(1000000.0f, 1000000.0f)), System::m_Window->getSize(), penumbraTexture, unshadowShader, lightOverShapeShader);
}

void Light::Add(const string& lightType, const string& key, shared_ptr<ltbl::LightPointEmission>&& light)
{
	// Свет - круг
	if (lightType == "point")
	{
		light->_emissionSprite.setTexture(pointLightTexture);
		light->_emissionSprite.setOrigin(sf::Vector2f(pointLightTexture.getSize().x * 0.5f, pointLightTexture.getSize().y * 0.5f));
	}

	// Свет - треугольник
	else if (lightType == "cone")
	{
		light->_emissionSprite.setTexture(ConeLightTexture);
		light->_emissionSprite.setOrigin(sf::Vector2f(ConeLightTexture.getSize().x * 0.5f, ConeLightTexture.getSize().y * 0.5f));
	}

	// Добавление света
	m_Data[key] = light;
	System::m_LightSystem.addLight(std::move(light));
}

void Light::Add(const string& lightType, shared_ptr<ltbl::LightPointEmission>&& light)
{
	const string& key = to_string(m_Score) + "_" + "light_class";

	Add(lightType, key, std::move(light));

	m_Score++;
}

shared_ptr<ltbl::LightPointEmission>& Light::Get(const string& key)
{
	return m_Data[key];
}

void Light::Draw()
{
	System::m_LightSystem.render(System::m_Camera, unshadowShader, lightOverShapeShader);
	lightSprite.setTexture(System::m_LightSystem.getLightingTexture());
	lightRenderStates.blendMode = sf::BlendMultiply;
	System::m_Window->setView(System::m_Window->getDefaultView());
	System::m_Window->draw(lightSprite, lightRenderStates);
	System::m_Window->setView(System::m_Camera);
}

bool Light::IsEmpty() const
{
	return m_Data.empty();
}

void Light::Reset()
{
	for (auto it = m_Data.begin(); it != m_Data.end(); it++)
	{
		if (!m_Data.empty() && it->first != "light_player")
		{
			it->second->quadtreeRemove();
			it = m_Data.erase(it);
		}

	}

	m_Score = 0;
}