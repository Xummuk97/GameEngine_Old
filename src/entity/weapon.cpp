///==========================================================================
///	Модуль:		Оружие
///	Автор:		Андрей Гребенщиков
///	Год:		2019
///==========================================================================

#include "weapon.h"
#include "bot.h"

Bullet::Bullet()
{
	SetType("bullet");
	SetProperty("angle", 0.0f);
	SetProperty("speed", 0.0f);
	SetProperty("damage", 0);
	SetProperty("delay", 0.0f);
	SetProperty("offset", 0.0f);
	SetProperty("fireEntity", string("none"));
	SetProperty("typeBullet", string("normal"));
}

Bullet::~Bullet()
{
}

void Bullet::Update()
{
	bool isHoming = false;
	const string& fireEntity = GetProperty("fireEntity");
	const string& typeBullet = GetProperty("typeBullet");

	if (m_Layers)
	{
		for (auto& layer : *m_Layers)
		{
			for (auto& entity : layer.second)
			{
				if (typeBullet == "homing" && !isHoming)
				{
					string typeEntity = entity->GetType();

					if ((typeEntity == "player" && fireEntity == "bot")
						|| ((typeEntity == "bot" && entity->To<Bot>().GetBotType() == Bot::BotType::Enemy) && fireEntity == "player"))
					{
						FloatRect	frThis = *this,
							frEntity = *entity;

						frThis.left -= 100.0f;
						frThis.width += 200.0f;
						frThis.top -= 100.0f;
						frThis.height += 200.0f;

						if (frThis.intersects(frEntity))
						{
							SetAngle(*this, *entity);
							isHoming = true;
						}
					}
				}

				if (*this == *entity)
				{
					if (layer.first == "collision")
					{
						SetProperty("isLife", false);
						return;
					}

					if (layer.first == "damage_box")
					{
						SetProperty("isLife", false);
						entity->Damage(GetProperty<int>("damage"));
						return;
					}

					const string& entityType = entity->GetType();
					if (entityType != "none" && entityType != GetProperty<string>("fireEntity"))
					{
						if (entityType == "player" || (entityType == "bot" && entity->To<Bot>().GetBotType() == Bot::BotType::Enemy))
						{
							entity->Damage(GetProperty<int>("damage"));
							SetProperty("isLife", false);
						}
					}

					if (entityType == "collision_angle_left")
					{
						auto entityRect = entity->GetRect();
						auto rect = GetRect();

						float y = (rect.left - entityRect.left) * entityRect.height / entityRect.width + entityRect.top - rect.height;

						if (rect.top > y)
						{
							SetProperty("isLife", false);
							SetProperty("isDraw", false);
							return;
						}
					}

					if (entityType == "collision_angle_right")
					{
						auto entityRect = entity->GetRect();
						auto rect = GetRect();

						float y = -(rect.left + rect.width - entityRect.left) * entityRect.height / entityRect.width + entityRect.top + entityRect.height - rect.height;

						if (rect.top > y)
						{
							SetProperty("isLife", false);
							SetProperty("isDraw", false);
							return;
						}
					}
				}
			}
		}
	}

	Move(System::MoveToAngle(GetProperty<float>("speed"), GetProperty<float>("angle") + GetProperty<float>("offset")));
}

void Bullet::SetAngle(const Vector2f& p1, const Vector2f& p2)
{
	SetProperty("angle", System::GetAngle(p1, p2) * RAD);
}

void Bullet::SetLayers(Layers* layers)
{
	m_Layers = std::move(layers);
}
//---------------------------------------------------------------------------------

Weapon::Weapon()
{
}

Weapon::~Weapon()
{
}

void Weapon::Update(const Vector2f& p1, const Vector2f& p2)
{
	m_bullet.SetAngle(p1, p2);
	m_bullet.Update();
	m_sprite.setPosition(p1);
}

void Weapon::Draw()
{
	//m_Window->draw(m_sprite);
}

Bullet* Weapon::GetBullet()
{
	return &m_bullet;
}

Sprite* Weapon::GetSprite()
{
	return &m_sprite;
}

void Weapon::SetName(const string& name)
{
	m_name = name;
}

string Weapon::GetName() const
{
	return m_name;
}

void Weapon::SetImageSprite(const string& textureName, const IntRect& rect)
{
	m_sprite.setTexture(System::m_Textures[textureName]);
	m_sprite.setTextureRect(rect);
	m_sprite.setOrigin(rect.width / 2.0f, rect.height / 2.0f);
}
//---------------------------------------------------------------------------------

Weapons::Weapons()
{
}

Weapons::~Weapons()
{
}

void Weapons::Add(const string& key, Weapon weapon)
{
	m_data[key] = make_shared<Weapon>(weapon);
	SetCurrentWeapon(key);
}

shared_ptr<Weapon> Weapons::Get(const string& key)
{
	return m_data[key];
}

map<string, shared_ptr<Weapon>> Weapons::Get()
{
	return m_data;
}

void Weapons::SetCurrentWeapon(const string& key)
{
	m_currentWeapon = key;
}

string Weapons::GetCurrentWeapon() const
{
	return m_currentWeapon;
}

void Weapons::SetIsFire(bool isFire)
{
	if (isFire && m_delay != 0.0f)
	{
		m_isFire = false;
		return;
	}

	m_isFire = isFire;
}

bool Weapons::GetIsFire() const
{
	return m_isFire;
}

shared_ptr<Bullet> Weapons::Update(FloatRect p1, const Vector2f& p2)
{
	m_delay -= System::m_Time;
	if (m_delay < 0.0f) m_delay = 0.0f;

	p1.left += p1.width / 2.0f;
	p1.top += p1.height / 2.0f;

	shared_ptr<Weapon> wpn = m_data[m_currentWeapon];

	wpn->Update({ p1.left, p1.top }, p2);
	auto bullet = wpn->GetBullet();

	wpn->GetSprite()->setRotation(bullet->GetProperty<float>("angle") / RAD);

	if (m_isFire && m_delay == 0.0f)
	{
		m_isFire = false;
		bullet->SetPosition(p1.left, p1.top);
		m_delay = bullet->GetProperty<float>("delay");
		return make_shared<Bullet>(*bullet);
	}
	return nullptr;
}

void Weapons::Draw()
{
	m_data[m_currentWeapon]->Draw();
}
