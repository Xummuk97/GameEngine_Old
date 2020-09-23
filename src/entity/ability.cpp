///==========================================================================
///	Модуль:		Анимация
///	Автор:		Андрей Гребенщиков
///	Год:		2019
///==========================================================================

#include "ability.h"

void AbilityContainer::Add(const AbilityType& type, float maxTime)
{
	// Игнорировать добавление, если уже способность есть
	if (m_data.find(type) != m_data.end())
		return;

	const shared_ptr<Ability>& ability = make_shared<Ability>(maxTime);
	m_data[type] = ability;
}

shared_ptr<Ability>& AbilityContainer::Get(const AbilityType& type)
{
	return m_data[type];
}

map<AbilityType, shared_ptr<Ability>> AbilityContainer::Get()
{
	return m_data;
}

void AbilityContainer::SetDelegate(Delegate&& delegate)
{
	m_delegate = std::move(delegate);
}

void AbilityContainer::Update(Entity* entity)
{
	if (m_delegate)
	{
		for (auto it = m_data.begin(); it != m_data.end();)
		{
			const auto& second = it->second;

			second->time += System::m_Time;
			
			// Если время способности не привышает её время жизни
			if (second->time < second->maxTime)
			{
				m_delegate(entity, it->first, 
					// Если следующий кадр будет удалением способности
					(second->time + System::m_Time) > second->maxTime);
				it++;
			}
			else it = m_data.erase(it);
		}
	}
}

Ability::Ability(float maxTime) 
	: maxTime(maxTime)
{
}
