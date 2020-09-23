///==========================================================================
///	Модуль:		Устаревшая база виджетов, следует перевести на TGUI
///	Автор:		Андрей Гребенщиков
///	Год:		2019
///==========================================================================

#include "widget.h"

using namespace Widget;

bool WidgetBase::m_IsClickButton;

WidgetBase::WidgetBase(const FloatRect& objRect, const Vector2f& pos, const FloatRect& region, const Horizontal& h, const Vertical& v) :
	m_region(region), m_pos(pos)
{
	if (m_region.width == 0.0f && m_region.height == 0.0f)
	{
		m_region.width = System::m_Permission.x;
		m_region.height = System::m_Permission.y;
	}

	switch (h)
	{
	case Horizontal::left:
		m_pos.x += m_region.left;
		break;

	case Horizontal::center:
		m_pos.x = m_region.left + m_region.width / 2.0f - objRect.width / 2.0f;
		break;

	case Horizontal::right:
		m_pos.x = m_region.left + m_region.width - objRect.width;
		break;
	}

	switch (v)
	{
	case Vertical::top:
		m_pos.y += m_region.top;
		break;

	case Vertical::middle:
		m_pos.y = m_region.top + m_region.height / 2.0f - objRect.height / 2.0f;
		break;

	case Vertical::bottom:
		m_pos.y = m_region.top + m_region.height - objRect.height;
		break;
	}

	m_region.left = m_pos.x;
	m_region.top = m_pos.y;
	m_region.width = objRect.width;
	m_region.height = objRect.height;
}

WidgetBase::~WidgetBase()
{
}

void WidgetBase::Update()
{
	if (!m_isAbsolute)
	{
		FloatRect wndRect = System::GetVisibleRect();
		m_BasePos.x = wndRect.left + m_pos.x;
		m_BasePos.y = wndRect.top + m_pos.y;
	}
}

bool Widget::WidgetBase::Contains(float x, float y)
{
	return FloatRect(m_BasePos.x, m_BasePos.y, m_region.width, m_region.height).contains(x, y);
}

FloatRect WidgetBase::GetRegion() const
{
	return m_region;
}

Vector2f WidgetBase::GetPos() const
{
	return m_pos;
}

Vector2f Widget::WidgetBase::GetBasePos() const
{
	return m_BasePos;
}

void Widget::WidgetBase::SetAbsolute(bool is)
{
	m_isAbsolute = is;
}

bool Widget::WidgetBase::IsAbsolute() const
{
	return m_isAbsolute;
}

void Widget::WidgetBase::SetPos(const Vector2f& pos)
{
	m_BasePos = pos;
}
