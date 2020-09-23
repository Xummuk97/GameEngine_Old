///==========================================================================
///	Модуль:		Телепорт
///	Автор:		Андрей Гребенщиков
///	Год:		2019
///==========================================================================

#include "teleport.h"

void Teleport::Update()
{
	Teleport::Intersects();
}

void Teleport::Intersects(char coord )
{
	for (auto& layer : *m_Layers)
	{
		for (auto& entity : layer.second)
		{
			if (GetRect().intersects(entity->GetRect()) && entity->GetType() == "player")
			{
				Messages::SetMessage(MessageType::Teleport, sMessage(true, this));
			}
		}
	}
}
