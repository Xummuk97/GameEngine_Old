///==========================================================================
///	Модуль:		Игровой объект
///	Автор:		Андрей Гребенщиков
///	Год:		2019
///==========================================================================

#include "entity.h"
#include "../core/effects.h"
#include <sstream>

Entity::Entity()
{
	SetProperty("isLife", true);

	SetProperty("isDeathTime", false);
	SetProperty("deathTime", 0.0f);

	SetProperty("isPush", false);
	SetProperty("pushDamage", 0.0f);
	SetProperty("pushTime", 0.0f);
	SetProperty("pushDirection", string("none"));

	SetProperty("isDraw", true);

	SetProperty("isDamage", false);
}

Entity::~Entity()
{
}

void Entity::Load()
{
	SetRectByProperty();	
	SetTexture();	
	SetPersonByProperty();
}

void Entity::SetTexture(const string& key)
{
	if (key == "null")
	{
		m_Sprite.setTexture(System::m_Textures[GetProperty<string>("textureName")]);
	}
	else
	{
		m_Sprite.setTexture(System::m_Textures[key]);
	}
}

void Entity::SetProperty(const string& key, const string& value)
{
	m_Property[key].StringToValue(value);
}

void Entity::SetType(const string& type)
{
	m_Type = type;
}

string Entity::GetType() const
{
	return m_Type;
}

Vector2f Entity::GetPosition() const
{
	return m_Sprite.getPosition();
}

FloatRect Entity::GetRect() const
{
	return m_Sprite.getGlobalBounds();
}

bool Entity::IsIntersects(const Entity& entity) const
{
	return m_Sprite.getGlobalBounds().intersects(entity);
}

AnySTD& Entity::GetProperty(const string& key)
{
	return m_Property[key];
}

void Entity::SetRect(const IntRect& rect)
{
	m_Sprite.setTextureRect(rect);
}

void Entity::SetRectByProperty()
{
	if (IsFindProperty("rect"))
	{
		vector<AnySTD> rect = GetProperty("rect");
		SetRect({ rect[0].Get<int>(), rect[1].Get<int>(), rect[2].Get<int>(), rect[3].Get<int>() });
	}
}

void Entity::SetPosition(const Vector2f& pos)
{
	m_Sprite.setPosition(pos);
}

void Entity::SetPosition(float x, float y)
{
	m_Sprite.setPosition({x, y});
}

void Entity::Move(const Vector2f& pos)
{
	m_Sprite.move(pos);
}

void Entity::Move(float x, float y)
{
	m_Sprite.move({x, y});
}

void Entity::Update()
{
	if (m_Animation)
	{
		// Обновление анимации
		Animation result = m_Animation->UpdateAndGetCurrentAnimation();

		if (result.IsNextFrame())
		{
			SetRect(result.GetRectCurrentAnimation());
		}
	}

	// Смерть через время
	if (GetProperty<bool>("isDeathTime"))
	{
		float deathTime = GetProperty("deathTime");

		if (deathTime > 0.0f)
		{
			deathTime -= System::m_Time;
			SetProperty("deathTime", deathTime);
		}
		else
		{
			SetProperty("isLife", false);
		}
	}

	// Толчок
	if (GetProperty<bool>("isPush"))
	{
		float time = GetProperty("pushTime");

		if (time > 0.0f)
		{
			const float& damage = GetProperty("pushDamage").Get<float>() * 4.0f;
			const string& direction = GetProperty("pushDirection");

			Move(0.0f, -damage * System::m_Time * 2.0f);

			if (direction == "left")
			{
				//Move(damage * m_Time, 0.0f);
			}
			else if (direction == "right")
			{
				//Move(-damage * m_Time, 0.0f);
			}
			else
			{
				/*int randDirection = GetRandInt(0, 100);

				if (randDirection < 50)
				{
					Move(damage * m_Time, 0.0f);
					SetProperty("pushDirection", string("left"));
				}
				else
				{
					Move(-damage * m_Time, 0.0f);
					SetProperty("pushDirection", string("right"));
				}*/
			}

			SetProperty("jumpCount", 0);
			SetProperty("inertiaUp", 0.0f);
			SetProperty("inertiaDown", 0.0f);

			time -= System::m_Time;
			SetProperty("pushTime", time);
		}
		else SetProperty("isPush", false);
	}

	if (GetProperty("isDamage"))
	{
		if (m_timerDmg > 0.0f)
			m_timerDmg -= System::m_Time;
		else
			SetProperty("isDamage", false);
	}
}

void Entity::Draw()
{
	if (GetProperty<bool>("isDraw") && m_Sprite.getGlobalBounds().intersects(System::GetVisibleRect()))
	{
		if (GetProperty("isDamage"))
		{
			System::m_Effects["color"]->Cast<Effects::Color>().SetColor({0.5f, 0.0f, 0.0f});
			System::m_Window->draw(m_Sprite, System::m_Effects["color"]->GetShader());
		}
		else System::m_Window->draw(m_Sprite);
	}
}

void Entity::SetAnimation(const shared_ptr<Animations>& animation)
{
	m_Animation = animation;
}

void Entity::SetSprite(const Sprite& sprite)
{
	m_Sprite = sprite;
}

Sprite* Entity::GetSprite()
{
	return &m_Sprite;
}

bool Entity::IsFindProperty(const string& key) const
{
	return m_Property.find(key) != m_Property.end();
}

bool Entity::operator == (const Entity& entity) const
{
	return m_Sprite.getGlobalBounds().intersects(entity);
}

bool Entity::operator != (const Entity & entity) const
{
	return !m_Sprite.getGlobalBounds().intersects(entity);
}

bool Entity::IsType(const string& type) const
{
	return GetType() == type;
}

void Entity::DeadThroughTime(float time)
{
	SetProperty("isDeathTime", true);
	SetProperty("deathTime", time);
}

void Entity::Push(float time, float damage)
{
	if (GetProperty<bool>("isPush")) return;

	SetProperty("isPush", true);
	SetProperty("pushDamage", damage);
	SetProperty("pushTime", time);
	SetProperty("pushDirection", GetProperty<string>("direction"));
}

void Entity::Damage(int damage)
{
	m_timerDmg = 0.1f;
	SetProperty("isDamage", true);
	SetProperty("hp", int(GetProperty<int>("hp") - damage));
}

void Entity::SetScripts(function<bool(Entity* entity, string scriptName)>&& scripts)
{
	m_scripts = std::move(scripts);
}

void Entity::SetPerson(const Person& person)
{
	m_Person = person;
}

unsigned int StringHexToInt(const string& hex)
{
	unsigned int val;
	std::stringstream ss;
	ss << std::hex << hex;
	ss >> val;

	return val;
}

void Entity::SetPersonByProperty()
{
	if (IsFindProperty("person"))
	{
		vector<AnySTD> personArr = GetProperty("person");

		string name		  = personArr[0].Get<string>();
		string snameColor = personArr[1].Get<string>(),
			   stextColor = personArr[2].Get<string>();
		
		m_Person.name = name;
		
		m_Person.nameColor = Color(
			(sf::Uint8)StringHexToInt(snameColor.substr(0, 2)),
			(sf::Uint8)StringHexToInt(snameColor.substr(2, 2)),
			(sf::Uint8)StringHexToInt(snameColor.substr(4, 2)));

		m_Person.textColor = Color(
			(sf::Uint8)StringHexToInt(stextColor.substr(0, 2)),
			(sf::Uint8)StringHexToInt(stextColor.substr(2, 2)),
			(sf::Uint8)StringHexToInt(stextColor.substr(4, 2)));
	}
}

Person Entity::GetPerson()
{
	return m_Person;
}

void Entity::IsFlipAnimation(float x)
{
	FloatRect rect = *this;
	float xPosOrigin = rect.left + rect.width / 2.0f;

	if (x > xPosOrigin)
	{
		m_Animation->SetFlip(false);
	}
	else
	{
		m_Animation->SetFlip(true);
	}
}
