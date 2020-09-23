///==========================================================================
///	Модуль:		Анимация
///	Автор:		Андрей Гребенщиков
///	Год:		2019
///==========================================================================

#include "animation.h"
#include "system.h"
#include <sstream>

Animation::Animation(float maxTime) 
	: m_MaxTime(maxTime)
{
}

void Animation::Add(Frame&& rect)
{
	m_Data.push_back(std::move(rect));
}

vector<Animation::Frame>& Animation::GetData()
{
	return m_Data;
}

void Animation::SetFlip(bool isFlip)
{
	m_IsFlip = isFlip;
}

bool Animation::IsFlip() const
{
	return m_IsFlip;
}

void Animation::Update()
{
	const size_t& SIZE = m_Data.size();

	// Массив пуст?
	if (!SIZE)
	{
		cout << "[Animation::NextFrame] Array empty." << endl;
		return;
	}

	m_Time += System::m_Time;

	// Смена кадра или произошёл сброс?
	if ((m_Time > m_MaxTime) || m_IsReset)
	{
		m_IsNextFrame = true;
		m_Time = 0.0f;

		if (!m_IsReset)
		{
			m_IndexAnimation++;

			// Кадры закончились?
			if (m_IndexAnimation > (SIZE - 1)) m_IndexAnimation = 0;
		} 
		else m_IsReset = false;
	}
}

bool Animation::IsNextFrame() const
{
	return m_IsNextFrame;
}

void Animation::Reset()
{
	m_Time = 0.0f;
	m_IsNextFrame = false;
	m_IndexAnimation = 0;
	m_IsReset = true;
}

IntRect& Animation::GetRectCurrentAnimation()
{
	if (!m_IsFlip)
		return *m_Data[m_IndexAnimation];
	else
	{
		IntRect rect = *m_Data[m_IndexAnimation];

		rect.left += rect.width;
		rect.width = -rect.width;

		return rect;
	}
}


void Animations::Add(const string& key, Animation::Ptr&& animation)
{
	m_Data[key] = std::move(animation);
}

Animation& Animations::UpdateAndGetCurrentAnimation()
{
	// Анимации есть?
	if (m_CurrentAnimationFrame != "none")
	{
		auto currentAnimation = m_Data[m_CurrentAnimationFrame];

		// Установка поворота спрайта
		currentAnimation->SetFlip(m_IsFlip);

		// Обновление и возвращение текущей анимации
		currentAnimation->Update();

		return *currentAnimation;
	}
	else cout << "[Animations::UpdateAndGetCurrentAnimation] Current animation empty." << endl;
}

void Animations::SetFlip(bool isFlip)
{
	m_IsFlip = isFlip;
}

bool Animations::IsFlip() const
{
	return m_IsFlip;
}

void Animations::SetCurrentAnimation(const string& key)
{
	// Если работала анимация, сбрасывать её
	if (m_CurrentAnimationFrame != "none") m_Data[m_CurrentAnimationFrame]->Reset();
	
	m_CurrentAnimationFrame = key;
}
