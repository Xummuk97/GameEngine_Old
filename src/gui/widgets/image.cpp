///==========================================================================
///	Модуль:		Устаревший виджет картинки, следует перевести на TGUI
///	Автор:		Андрей Гребенщиков
///	Год:		2019
///==========================================================================

#include "image.h"

Widget::Image::Image(Sprite sprite, const FloatRect& region, const Vector2f& pos, const Horizontal& h, const Vertical& v) :
	m_Sprite(sprite),
	WidgetBase(sprite.getGlobalBounds(), pos, region, h, v)
{
}

Widget::Image::~Image()
{
}

void Widget::Image::Update()
{
	WidgetBase::Update();
	m_Sprite.setPosition(m_BasePos);
}

void Widget::Image::Draw()
{
	System::m_Window->draw(m_Sprite);
}

Sprite* Widget::Image::GetSprite()
{
	return &m_Sprite;
}
