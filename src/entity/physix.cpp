///==========================================================================
///	Модуль:		Физика
///	Автор:		Андрей Гребенщиков
///	Год:		2019
///==========================================================================

#include "physix.h"
#include "movingPlatform.h"
#include "bot.h"
#include "player.h"

const float MOVING_PLATFORM_ESP = 0.4f;

Physix::Physix()
{
	SetProperty("isGround", false);
	SetProperty("isJump", false);
	SetProperty("isStair", false);
	SetProperty("isMovingPlatform", false);
	SetProperty("jumpCount", 0);
	SetProperty("inertiaUp", 0.0f);
	SetProperty("inertiaDown", 0.0f);
	SetProperty("direction", string("right"));
	SetProperty("stairRect", FloatRect());
	SetProperty("collisionRectLeft", FloatRect(0.0f, 0.0f, 0.0f, 0.0f));
	SetProperty("collisionRectRight", FloatRect(0.0f, 0.0f, 0.0f, 0.0f));
}

Physix::~Physix()
{
}

void Physix::DynamicMove()
{
	// Базовое обновление
	Entity::Update();

	const bool& isStair = GetProperty("isStair");

	// Передвижение по лестнице
	if (isStair)
	{
		Stair();
	}

	SetProperty("isGround", false);
	SetProperty("isMovingPlatform", false);

	// Если объект на лестнице, на него не действует основная физика
	if (isStair)
	{
		return;
	}

	string direction = GetProperty("direction");

	if (direction == "up" || direction == "down")
	{
		direction = "stop";
		SetProperty("direction", direction);
	}

	// Если совершён прыжок
	if (GetProperty<bool>("isJump"))
	{
		// Движение вверх
		float inertiaUp = GetProperty("inertiaUp");

		// Пока инерция не отрицательна
		if (inertiaUp >= 0.0f)
		{
			inertiaUp = GetProperty("inertiaUp");

			Move(0, -inertiaUp * System::m_Time * 700.0f);

			inertiaUp -= System::m_Time * 3.0f;
			SetProperty("inertiaUp", inertiaUp);
		}
		else
		{
			JumpClear();
		}
	}
	else
	{
		// Движение вниз
		float inertiaDown = GetProperty("inertiaDown");

		Move(0, GetProperty<float>("gravity") * inertiaDown * System::m_Time * 150.0f);

		inertiaDown += System::m_Time * 10.0f;
		SetProperty("inertiaDown", inertiaDown);
		
	}

	if (GetProperty<int>("hp") <= 0)
		SetProperty("isLife", false);
}

void Physix::GoJump()
{
	IsGround();

	const bool& isGround = GetProperty("isGround");
	int jumpCount = GetProperty("jumpCount");

	if (!isGround && jumpCount == 0)
		return;

	if ((!GetProperty<bool>("isJump") && isGround) || jumpCount < GetProperty<int>("jumpCountMax"))
	{
		SetProperty("isJump", true);
		SetProperty("inertiaUp", GetProperty<float>("maxInertiaUp"));
		SetProperty("inertiaDown", 0.0f);
		jumpCount++;
		SetProperty("jumpCount", jumpCount);
	}
 }

void Physix::DirectionToUp()
{
	SetProperty("direction", string("up"));
}

void Physix::DirectionToDown()
{
	SetProperty("direction", string("down"));
}

void Physix::GoLeft(float speed, bool isCheckIntersects)
{
	Move(-200.0f * (speed == 0.0f ? GetProperty<float>("speed") : speed) * System::m_Time, 0);
	SetProperty("direction", string("left"));
	if (isCheckIntersects)
		Intersects('x');

	if (GetProperty<FloatRect>("collisionRectRight") != FloatRect(0.0f, 0.0f, 0.0f, 0.0f))
	{
		if (GetRect().intersects(GetProperty<FloatRect>("collisionRectRight")))
			Move(0.0f, 400.0f * System::m_Time);

		SetProperty("collisionRectRight", FloatRect(0.0f, 0.0f, 0.0f, 0.0f));
	}
}

void Physix::GoRight(float speed, bool isCheckIntersects)
{
	Move(200.0f * (speed == 0.0f ? GetProperty<float>("speed") : speed) * System::m_Time, 0);
	SetProperty("direction", string("right"));
	if (isCheckIntersects)
		Intersects('x');

	if (GetProperty<FloatRect>("collisionRectLeft") != FloatRect(0.0f, 0.0f, 0.0f, 0.0f))
	{
		if (GetRect().intersects(GetProperty<FloatRect>("collisionRectLeft")))
			Move(0.0f, 400.0f * System::m_Time);

		SetProperty("collisionRectLeft", FloatRect(0.0f, 0.0f, 0.0f, 0.0f));
	}
}

void Physix::GoUp(float speed)
{
	Move(System::m_Time, -200.0f * (speed == 0.0f ? GetProperty<float>("speed") : speed));
}

void Physix::GoDown(float speed)
{
	Move(System::m_Time, 200.0f * (speed == 0.0f ? GetProperty<float>("speed") : speed));
}

void Physix::Intersects(char coord)
{
	const string& direction = GetProperty("direction");

	for (auto& layer : *m_Layers)
	{
		for (auto& entity : layer.second)
		{
			if (*this == *entity && !IsType(entity->GetType()))
			{
				if (IsPlayerBonus(layer.first, entity))						break;
				else if (IsIntersectScripts(layer.first, entity))			break;
				else if (IsIntersectCollision(layer.first, entity, coord))	break;
				else if (IsIntersectDamageAll(layer.first))					break;
				else if (IsIntersectDamage(layer.first))					break;
				else if (IsIntersectCollisionUp(layer.first, entity))		break;

				IsIntersectStair(entity, direction);
				IsIntersectMovingPlatform(entity);

				if (!IsIntersectCollisionAngleLeft(entity))
					IsIntersectCollisionAngleRight(entity);

				IsPlayerIntersectBot(entity);
				IsPlayerTeleport(entity);
				IsPlayerDialog(entity);
			}

			IsIntersectMovingPlatformBugFix(entity);
		}
	}
}

void Physix::Collision(char coord, const FloatRect& rect)
{
	const string& direction = GetProperty("direction");

	switch (coord)
	{
	case 'x':
		// Если была нажата клавища влево
		if (direction == "left")
		{
			SetPosition(rect.left + rect.width, GetPosition().y);
		}
		// Если была нажата клавища вправо
		else if (direction == "right")
		{
			SetPosition(rect.left - GetRect().width, GetPosition().y);
		}

		break;

	case 'y':
		// Если объект ударился головой об потолок
		if (GetProperty<bool>("isJump"))
		{
			SetPosition(GetPosition().x, rect.top + rect.height);
			JumpClear();
		}

		// Если было совершено приземление
		else if (!GetProperty<bool>("isGround"))
		{
			IsFallDamage();

			SetPosition(GetPosition().x, rect.top - GetRect().height);
			SetProperty("isGround", true);
			SetProperty("inertiaDown", 0.0f);
		}
		break;
	}
}

void Physix::Stair()
{
	FloatRect rect = *this;
	const FloatRect& rectStair = GetProperty("stairRect");

	if (!rect.intersects(rectStair))
	{
		StairClear();
		return;
	}

	string direction = GetProperty("direction");

	// Передвижение вверх
	if (direction == "up")
	{
		Move(0, -200.0f * GetProperty<float>("speed") * System::m_Time);
		SetProperty("direction", string("stop"));

		rect.top = GetPosition().y;

		if (rect.top + rect.height < rectStair.top)
		{
			SetPosition(rect.left, rectStair.top - rect.height);
			StairClear();
		}
	}

	// Передвижение вниз
	else if (direction == "down")
	{
		Move(0, 200.0f * GetProperty<float>("speed") * System::m_Time);

		// Проверка на выход объекта за верхней предел лестницы
		rect.top = GetPosition().y;

		if (rect.top + rect.height > rectStair.top + rectStair.height)
		{
			StairClear();
			SetPosition(rect.left, rectStair.top + rectStair.height);
		}

		SetProperty("direction", string("stop"));
	}
}

void Physix::StairClear()
{
	SetProperty("isStair", false);
	SetProperty("stairRect", FloatRect());
}

void Physix::JumpClear()
{
	SetProperty("isJump", false);
	SetProperty("inertiaUp", 0.0f);
}

void Physix::SetLayers(Layers* layers)
{
	m_Layers = std::move(layers);
}

void Physix::IsGround()
{
	if (IsGround(*this))
	{
		SetProperty("isGround", true);
		SetProperty("jumpCount", 0);
	}
}

bool Physix::IsGround(const FloatRect& _rect)
{
	FloatRect rect = std::move(_rect);
	rect.top += 10.0f;

	for (auto& layer : *m_Layers)
	{
		for (auto& entity : layer.second)
		{
			if (rect.intersects(*entity) && !IsType(entity->GetType()))
			{
				if (layer.first == "collision" || layer.first == "falling" || layer.first == "collision_up" || layer.first == "damage_box" || entity->IsType("movingPlatform") || entity->IsType("collision_angle_left") || entity->IsType("collision_angle_right") || entity->IsType("stair"))
				{
					return true;
				}
			}
		}
	}

	return false;
}

bool Physix::IsIntersectScripts(const string& layer, const shared_ptr<Entity>& entity)
{
	if (layer == "scripts")
	{
		Scripts(&(*entity));
		return true;
	}
	return false;
}

bool Physix::IsIntersectCollision(const string& layer, const shared_ptr<Entity>& entity, char coord)
{
	if (!GetProperty<bool>("isStair"))
	{
		// Пересечение с объектом коллизии
		if (layer == "collision" || layer == "falling" || layer == "damage_box")
		{
			Collision(coord, *entity);

			if (layer == "falling" && !entity->GetProperty<bool>("isDeathTime") && (GetPosition().y < entity->GetPosition().y))
				entity->DeadThroughTime(0.6f);

			return true;
		}
	}
	return false;
}

bool Physix::IsIntersectDamageAll(const string& layer)
{
	if (layer == "damage_all")
	{
		SetProperty("isLife", false);
		return true;
	}
	return false;
}

bool Physix::IsIntersectDamage(const string& layer)
{
	if (layer == "damage" && !GetProperty("isPush"))
	{
		Damage(stoi(System::m_Templates["damage"]["damage"]));
		Push(0.1f, 100.0f);
		return true;
	}
	return false;
}

bool Physix::IsIntersectCollisionUp(const string& layer, const shared_ptr<Entity>& entity)
{
	if (layer == "collision_up" && !GetProperty<bool>("isGround") && !GetProperty<bool>("isJump"))
	{
		const FloatRect	&rect = *this,
					&rectEntity = *entity;

		if ((rect.top + rect.height) < (rectEntity.top + rectEntity.height / 2.0f))
		{
			if (GetProperty<string>("direction") == "down")
			{
				SetPosition(GetPosition().x, rectEntity.top);
			}
			else
			{
				IsFallDamage();
				SetPosition(GetPosition().x, rectEntity.top - rect.height);
				SetProperty("isGround", true);
				SetProperty("inertiaDown", 0.0f);
			}
		}
		return true;
	}
	return false;
}

bool Physix::IsIntersectStair(const shared_ptr<Entity>& entity, const string& direction)
{
	if (entity->IsType("stair"))
	{
		const FloatRect& rect = *this;
		const FloatRect& rectEntity = *entity;

		if (rect.intersects(rectEntity))
		{
			// Начало передвижения по лестнице вверх
			float centerX = rect.left + rect.width / 2.0f;

			if (centerX >= rectEntity.left + rectEntity.width * 0.3f && centerX <= rectEntity.left + rectEntity.width * 0.7f)
			{
				if (direction == "up")
				{
					SetPosition(rectEntity.left, GetPosition().y);
					SetProperty("isStair", true);
					SetProperty("stairRect", rectEntity);
					SetProperty("inertiaDown", 0.0f);
					JumpClear();
					return true;
				}

				// Начало передвижения по лестнице вниз
				else if (direction == "down")
				{
					SetPosition(rectEntity.left, GetPosition().y);
					SetProperty("isStair", true);
					SetProperty("stairRect", rectEntity);
					SetProperty("inertiaDown", 0.0f);
					JumpClear();
					return true;
				}
			}

			if (rect.top + rect.height * 0.9f < rectEntity.top + rectEntity.height * 0.1f)
			{
				IsFallDamage();
				SetPosition(rect.left, rectEntity.top - rect.height);
				SetProperty("isGround", true);
				SetProperty("inertiaDown", 0.0f);
			}
		}
	}
	return false;
}

bool Physix::IsIntersectMovingPlatform(const shared_ptr<Entity>& entity)
{
	if (!GetProperty<bool>("isMovingPlatform") && !GetProperty<bool>("isGround") && !GetProperty<bool>("isJump") && entity->IsType("movingPlatform"))
	{
		const string& directionEntity = entity->GetProperty("direction");

		if (directionEntity == "left" || directionEntity == "right")
		{
			const FloatRect& rect = *this;
			const FloatRect& rectEntity = *entity;

			if (rect.top + rect.height < rectEntity.top + rectEntity.height * MOVING_PLATFORM_ESP)
			{
				IsFallDamage();

				SetProperty("isMovingPlatform", true);
				SetPosition(rect.left, rectEntity.top - rect.height);

				if (directionEntity == "left")
				{
					GoLeft(entity->GetProperty<float>("speed"), true);
				}
				else if (directionEntity == "right")
				{
					GoRight(entity->GetProperty<float>("speed"), true);
				}

				SetProperty("isGround", true);
				SetProperty("inertiaDown", 1.0f);

				JumpClear();

				return true;
			}
		}
	}
	return false;
}

bool Physix::IsIntersectCollisionAngleLeft(const shared_ptr<Entity>& entity)
{
	if (entity->IsType("collision_angle_left"))
	{
		auto entityRect = entity->GetRect();
		auto rect = GetRect();

		float y = (rect.left - entityRect.left) * entityRect.height / entityRect.width + entityRect.top - rect.height;

		if (rect.top > y)
		{
			IsFallDamage();
			SetPosition(rect.left, y);
			SetProperty("isGround", true);
			SetProperty("inertiaDown", 0.0f);
			SetProperty("collisionRectLeft", entityRect);
			return true;
		}
	}
	return false;
}

bool Physix::IsIntersectCollisionAngleRight(const shared_ptr<Entity>& entity)
{
	if (entity->IsType("collision_angle_right"))
	{
		auto entityRect = entity->GetRect();
		auto rect = GetRect();

		float y = -(rect.left + rect.width - entityRect.left) * entityRect.height / entityRect.width + entityRect.top + entityRect.height - rect.height;

		if (rect.top > y)
		{
			IsFallDamage();
			SetPosition(rect.left, y);
			SetProperty("isGround", true);
			SetProperty("inertiaDown", 0.0f);
			SetProperty("collisionRectRight", entityRect);
			return true;
		}
	}
	return false;
}

bool Physix::IsPlayerIntersectBot(const shared_ptr<Entity>& entity)
{
	if (GetType() == "player" && entity->GetType() == "bot" && entity->To<Bot>().GetBotType() == Bot::BotType::Enemy)
	{
		if (!GetProperty<bool>("isPush"))
		{
			this->Push(0.1f, 100.0f);
			Damage(100);
			return true;
		}
	}
	return false;
}

bool Physix::IsPlayerTeleport(const shared_ptr<Entity>& entity)
{
	if (GetType() == "player" && entity->GetType() == "teleport" && IsKeyBoard("action"))
	{
		string to = entity->GetProperty("to");

		for (auto& layer : *m_Layers)
		{
			for (auto& _entity : layer.second)
			{
				if (_entity->GetType() == to)
				{
					SetPosition(_entity->GetPosition());
					return true;
				}
			}
		}
	}
	return false;
}

bool Physix::IsPlayerDialog(const shared_ptr<Entity>& entity)
{
	if (GetType() == "player" && entity->GetType() == "bot" && entity->To<Bot>().GetBotType() == Bot::BotType::Friend)
	{
		if (!System::m_isPause && IsKeyBoard("action"))
		{
			System::m_isPause = true;

			System::m_Dialogs->SetCurrent("dialog_A");
			System::m_Dialogs->SetPerson(*entity);
			System::m_Dialogs->Create();

			return true;
		}
	}
	return false;
}

bool Physix::IsPlayerBonus(const string& layer, const shared_ptr<Entity>& entity)
{
	if (GetType() != "player" || !entity->GetProperty<bool>("isLife"))
		return false;

	bool isInter = false;

	if (layer == "bonus_health")
	{
		SetProperty("hp", GetProperty<int>("hp") + stoi(System::m_Templates["bonus_health"]["hp"]));
		isInter = true;
	}
	else if (layer == "bonus_speed")
	{
		To<Player>().GetAbility()->Add(AbilityType::Speed, stof(System::m_Templates["bonus_speed"]["sec"]));
		isInter = true;
	}
	else if (layer == "bonus_damage")
	{
		// TODO: Сделать
		isInter = true;
	}
	else if (layer == "bonus_god")
	{
		// TODO: Сделать
		isInter = true;
	}
	else if (layer == "bonus_super_weapon")
	{
		To<Player>().GetAbility()->Add(AbilityType::WeaponSuper, stof(System::m_Templates["bonus_super_weapon"]["sec"]));
		isInter = true;
	}
	else if (layer == "bonus_weapon_homing")
	{
		To<Player>().GetAbility()->Add(AbilityType::WeaponHoming, stof(System::m_Templates["bonus_weapon_homing"]["sec"]));
		isInter = true;
	}

	if (isInter)
		entity->SetProperty("isLife", false);

	return isInter;
}

bool Physix::IsFallDamage()
{
	float inertiaDown = GetProperty("inertiaDown");

	if (inertiaDown > 10.0f)
	{
		Damage(inertiaDown * 20.0f);
		return true;
	}
	return false;
}

bool Physix::IsIntersectMovingPlatformBugFix(const shared_ptr<Entity>& entity)
{
	if (!GetProperty<bool>("isMovingPlatform") && entity->IsType("movingPlatform") && !GetProperty<bool>("isGround") && !GetProperty<bool>("isJump"))
	{
		const string& directionEntity = entity->GetProperty("direction");

		if (directionEntity == "up" || directionEntity == "down")
		{
			SetProperty("isMovingPlatform", true);
			const FloatRect& rect = *this;
			FloatRect rectEntity = *entity;

			rectEntity.top -= rectEntity.height * MOVING_PLATFORM_ESP;

			if (rect.intersects(rectEntity))
			{
				rectEntity.top += rectEntity.height * MOVING_PLATFORM_ESP;

				if (rect.top + rect.height < rectEntity.top + rectEntity.height * MOVING_PLATFORM_ESP)
				{
					IsFallDamage();

					SetPosition(rect.left, rectEntity.top - rect.height + (directionEntity == "down" ? 2.5f : -1.0f));
					SetProperty("isGround", true);
					SetProperty("inertiaDown", 1.0f);

					JumpClear();

					return true;
				}
			}
		}
	}
	return false;
}

void Physix::Scripts(Entity* script)
{
	if (script->GetProperty<bool>("isLife"))
	{
		if (m_scripts(&To<Entity>(), script->GetType())) script->SetProperty("isLife", false);
	}
}
