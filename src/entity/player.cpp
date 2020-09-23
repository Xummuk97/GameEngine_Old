///==========================================================================
///	Модуль:		Игрок
///	Автор:		Андрей Гребенщиков
///	Год:		2019
///==========================================================================

#include "player.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::Load()
{
	Entity::Load();

	static auto abilityDelegate = [](Entity* entity, const AbilityType& type, bool isNextEnd)
	{
		switch (type)
		{
		case AbilityType::Speed:
			static float speed = entity->GetProperty("speed");

			if (!isNextEnd)
				entity->SetProperty("speed", speed + stof(System::m_Templates["bonus_speed"]["speed"]));
			else
				entity->SetProperty("speed", speed);
			break;
		}
	};

	SetAbility(abilityDelegate);

	// TODO: Сделать возможность бесконечной установки абилки и её программного уничтожения
	//GetAbility()->Add(AbilityType::WeaponSuper, 10.0f);
}

Weapons* Player::GetWeapons()
{
	return &m_Weapons;
}

void Player::SetWeaponsProperty(map<string, shared_ptr<Weapon>>& templatesWeapons)
{
	if (IsFindProperty("weapons"))
	{
		vector<AnySTD> weapons = GetProperty("weapons");

		for (auto& weapon : weapons)
		{
			string weaponName = weapon.Get<string>();
			m_Weapons.Add(weaponName, *templatesWeapons[weaponName]);
		}
	}
}

void Player::Draw()
{
	Entity::Draw();
	m_Weapons.Draw();
}

void Player::Update()
{
	IsFlipAnimation(System::m_MousePosition.x);

	// Передвижение динамических объектов
	DynamicMove();

	// Нажатие кнопки огня
	if (MOUSE_PRESSED(Left) && System::GetVisibleRect().contains(System::m_MousePosition))
		m_Weapons.SetIsFire(true);

	// Дополнительные возможности с клавиатуры
	if (IsKeyBoard("up"))
	{
		m_Animation->SetCurrentAnimation("normal");
		DirectionToUp();
	}
	else if (IsKeyBoard("down"))
	{
		m_Animation->SetCurrentAnimation("two");
		DirectionToDown();
	}
	
	// Проверка на пересечения
	Physix::Intersects();

	// Перемещение ессли не на лестнице
	bool isStair = GetProperty("isStair");
	const bool& isKeyBoardJump = IsKeyBoard("jump");

	if (isKeyBoardJump && isStair)
	{
		isStair = false;
		StairClear();
	}

	if (!isStair)
	{
		if (IsKeyBoard("left"))
		{
			GoLeft();
		}

		// Движение вправо
		else if (IsKeyBoard("right"))
		{
			GoRight();
		}

		// Прыжок
		static bool pressedFix = false;

		if (!pressedFix && isKeyBoardJump)
		{
			GoJump();
			pressedFix = true;
		}
		else if (pressedFix && !isKeyBoardJump)
		{
			pressedFix = false;
		}
	}


	// Обновление камеры
	Vector2f pos = *this;

	if ((pos.x - System::m_Permission.x / 2.0f) < 0.0f)
	{
		pos.x = System::m_Permission.x / 2.0f;
	}

	if ((pos.x + System::m_Permission.x / 2.0f) > System::m_MapSize.x)
	{
		pos.x = System::m_MapSize.x - System::m_Permission.x / 2.0f;
	}

	if ((pos.y - System::m_Permission.y / 2.0f) < 0.0f)
	{
		pos.y = System::m_Permission.y / 2.0f;
	}

	if ((pos.y + System::m_Permission.y / 2.0f) > System::m_MapSize.y)
	{
		pos.y = System::m_MapSize.y - System::m_Permission.y / 2.0f;
	}

	System::m_Camera.setCenter((int)pos.x, (int)pos.y);

	// Способности
	if (m_Ability)
	{
		m_Ability->Update(this);
	}
}

void Player::SetAbility(function<void(Entity* entity, const AbilityType& type, bool isNextEnd)>&& delegate)
{
	m_Ability = new AbilityContainer();
	m_Ability->SetDelegate(std::move(delegate));
}

AbilityContainer* Player::GetAbility()
{
	return m_Ability;
}
