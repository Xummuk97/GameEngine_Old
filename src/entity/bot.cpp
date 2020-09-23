///==========================================================================
///	Модуль:		Бот
///	Автор:		Андрей Гребенщиков
///	Год:		2019
///==========================================================================

#include "bot.h"

Bot::Bot()
{
	SetProperty("playerRect", FloatRect(0.0f, 0.0f, 0.0f, 0.0f));
	SetProperty("rectVisiblePlayer", FloatRect(0.0f, 0.0f, 500.0f, 500.0f));
	SetProperty("direction", string("right"));
}

Bot::~Bot()
{
}

void Bot::Load()
{
	Player::Load();
	SetSizeVisiblePlayerProperty();

	if (IsFindProperty("action"))
	{
		string action = GetProperty("action");

		if (action == "move")
			m_action = Bot::Action::Move;
		else if (action == "stop")
			m_action = Bot::Action::Stop;
	}

	if (IsFindProperty("type"))
	{
		string type = GetProperty("type");

		if (type == "friend")
			m_type = Bot::BotType::Friend;
		else if (type == "enemy")
			m_type = Bot::BotType::Enemy;
	}
}

void Bot::Intersects(char coord)
{
	const string& direction = GetProperty("direction");

	for (auto& layer : *m_Layers)
	{
		for (auto& entity : layer.second)
		{
			if (entity->IsType("player"))
			{
				SetProperty("playerRect", entity->GetRect());
			}

			if (*this == *entity && !IsType(entity->GetType()))
			{
				if (!IsIntersectCollisionAngleLeft(entity))
					IsIntersectCollisionAngleRight(entity);

				if (entity->IsType("player"))
				{
					if (m_type == Bot::BotType::Friend)
					{
						Messages::SetMessage(MessageType::Say, sMessage(true, this));
					}
				}

				if (IsIntersectScripts(layer.first, entity))				break;

				else if (IsIntersectCollision(layer.first, entity, coord))	
				{
					if (coord == 'x' && layer.first == "collision")
					{
						if (GetProperty<string>("direction") == "left")
						{
							if (GetProperty<FloatRect>("collisionRectLeft") != FloatRect(0.0f, 0.0f, 0.0f, 0.0f))
							{
								SetPosition(GetPosition().x, entity->GetPosition().y);
								SetProperty("collisionRectLeft", FloatRect(0.0f, 0.0f, 0.0f, 0.0f));
							}
							else SetProperty("direction", string("right"));
						}
						else
						{
							if (GetProperty<FloatRect>("collisionRectRight") != FloatRect(0.0f, 0.0f, 0.0f, 0.0f))
							{
								SetPosition(GetPosition().x, entity->GetPosition().y);
								SetProperty("collisionRectRight", FloatRect(0.0f, 0.0f, 0.0f, 0.0f));
							}
							else SetProperty("direction", string("left"));
						}
					}

					break;
				}
				else if (IsIntersectDamageAll(layer.first))				break;
				else if (IsIntersectDamage(layer.first))					break;
				else if (IsIntersectCollisionUp(layer.first, entity))		break;

				IsIntersectMovingPlatform(entity);
			}

			IsIntersectMovingPlatformBugFix(entity);
		}
	}
}

void Bot::Update()
{
	// Передвижение динамических объектов
	DynamicMove();

	// Проверка на пересечения
	Bot::Intersects();

	switch (m_action)
	{

		// Движение
	case Bot::Action::Move:

		if (IsAbyss())
		{
			if (GetProperty<string>("direction") == "left")
				SetProperty("direction", string("right"));
			else
				SetProperty("direction", string("left"));

		}

		const string& direction = GetProperty("direction");

		if (direction == "left")
		{
			GoLeft();

			// Поворот спрайта врага в сторону его движения
			IsFlipAnimation(GetPosition().x - 10000.0f);
		}

		else if (direction == "right")
		{
			GoRight();

			// Поворот спрайта врага в сторону его движения
			IsFlipAnimation(GetPosition().x + 10000.0f);
		}

		break;
	}

	switch (m_type)
	{

	case Bot::BotType::Enemy:

		SetRectPlayer();
		Fire();

		// Способности
		if (m_Ability)
		{
			m_Ability->Update(this);
		}
		break;

	}
}

void Bot::Fire()
{
	const FloatRect& playerRect = GetProperty("playerRect");

	if (playerRect != FloatRect(0.0f, 0.0f, 0.0f, 0.0f) && playerRect.intersects(GetProperty<FloatRect>("rectVisiblePlayer")))
	{
		bool isFire = false;
		const FloatRect& rect = GetRect();
		const string& direction = GetProperty("direction");

		if (direction == "left")
		{ 
			if (rect.left > playerRect.left + playerRect.width) isFire = true; 
		}

		if (direction == "right")
		{		
			if (rect.left + rect.width < playerRect.left) isFire = true;
		}

		if (isFire && !IsBulletCollision(System::GetAngle({rect.left, rect.top}, {playerRect.left, playerRect.top}) * RAD, playerRect))
		{
			m_Weapons.SetIsFire(true);
			m_action = Bot::Action::Stop;
		}
		else 
		{
			SetProperty("playerRect", FloatRect(0.0f, 0.0f, 0.0f, 0.0f));
			m_action = Bot::Action::Move;
		}
	}
	else 
	{
		SetProperty("playerRect", FloatRect(0.0f, 0.0f, 0.0f, 0.0f));
		m_action = Bot::Action::Move;
	}
}

void Bot::SetSizeVisiblePlayerProperty()
{
	if (IsFindProperty("sizeVisiblePlayer"))
	{
		vector<AnySTD> sizeVisiblePlayer = GetProperty("sizeVisiblePlayer");

		SetProperty("rectVisiblePlayer", FloatRect(0.0f, 0.0f, sizeVisiblePlayer[0].Get<float>(), sizeVisiblePlayer[1].Get<float>()));
	}
}

bool Bot::IsBulletCollision(float angle, const FloatRect& player)
{
	FloatRect	
		bulletRect = GetRect(),
		rectVisiblePlayer = GetProperty<FloatRect>("rectVisiblePlayer");

	bulletRect.width =
	bulletRect.height = 1.0f;

	do
	{
		Vector2f move = System::MoveToAngle(100.0f, angle);
		bulletRect.left += move.x;
		bulletRect.top += move.y;

		if (bulletRect.intersects(player)) return false;

		for (auto& entity : (*m_Layers)["collision"])
		{
			if (bulletRect.intersects(entity->GetRect())) return true;
		}
	}
	while (bulletRect.intersects(rectVisiblePlayer));

	return false;
}

bool Bot::IsAbyss()
{
	const string& direction = GetProperty("direction");
	FloatRect rect = GetRect();

	if (direction == "left")
	{
		rect.left -= rect.width;
		if (!IsGround(rect))
			return true;
	}
	else if (direction == "right")
	{
		rect.left += rect.width;
		if (!IsGround(rect))
			return true;
	}

	return false;
}

void Bot::SetBotType(const BotType& type)
{
	m_type = type;
}

Bot::BotType Bot::GetBotType() const
{
	return m_type;
}

void Bot::SetAction(const Action& action)
{
	m_action = action;
}

Bot::Action Bot::GetAction() const
{
	return m_action;
}

void Bot::SetRectPlayer()
{
	const FloatRect& rect = GetRect();
	FloatRect rectVisiblePlayer = GetProperty("rectVisiblePlayer");

	rectVisiblePlayer.left = (rect.left + rect.width / 2.0f) - (rectVisiblePlayer.width / 2.0f);
	rectVisiblePlayer.top = (rect.top + rect.height / 2.0f) - (rectVisiblePlayer.height / 2.0f);

	SetProperty("rectVisiblePlayer", rectVisiblePlayer);
}
