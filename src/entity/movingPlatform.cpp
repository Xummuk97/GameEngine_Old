#include "movingPlatform.h"

MovingPlatform::MovingPlatform()
{
}

MovingPlatform::~MovingPlatform()
{
}

void MovingPlatform::Update()
{
	DynamicMove();
	Intersects();
	
}

void MovingPlatform::DynamicMove()
{
	const string& direction = GetProperty("direction");

	if (direction == "left")
		Move(-200.0f * GetProperty<float>("speed") * System::m_Time, 0);
	else if (direction == "right")
		Move(200.0f * GetProperty<float>("speed") * System::m_Time, 0);
	else if (direction == "up")
		Move(0, -200.0f * GetProperty<float>("speed") * System::m_Time);
	else if (direction == "down")
		Move(0, 200.0f * GetProperty<float>("speed") * System::m_Time);

}

void MovingPlatform::Intersects(char coord)
{
	for (const auto& [name, vec] : *m_Layers)
		for (const auto& e : vec)
			if (e->IsType("pointer") && IsIntersects(*e))
			{
				SetProperty("direction", e->GetProperty<string>("direction"));
				return;
			}
}

void MovingPlatform::SetLayers(Layers* layers)
{
	m_Layers = std::move(layers);
}
