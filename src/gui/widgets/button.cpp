#include "button.h"

using namespace Widget;

Button::Button(Text text, const ButtonImages& images, const string& textureName, const FloatRect& region, const Vector2f& pos, const Horizontal& h, const Vertical& v) :
	WidgetBase({(float)images.m_Normal.left, (float)images.m_Normal.top, (float)images.m_Normal.width, (float)images.m_Normal.height}, 
		pos, region, h, v),
	m_Images(images)
{
	Sprite sprite;
	sprite.setTexture(m_Textures[textureName]);
	sprite.setTextureRect(m_Images.m_Normal);
	m_Image = new Widget::Image(sprite, region, pos, h, v);
	m_Label = new Widget::Label(text, m_Image->GetRegion(), { 0.0f, 0.0f }, Horizontal::center, Vertical::top);
}

Button::~Button()
{
}

Widget::Image* Button::GetImage()
{
	return m_Image;
}

Widget::Label* Button::GetLabel()
{
	return m_Label;
}

void Button::Update()
{
	auto imgSpr = m_Image->GetSprite();

	if (m_FuncClick && !m_IsClickButton && m_Image->Contains(m_MousePosition.x, m_MousePosition.y))
	{
		static bool presedFix = false;

		if (!presedFix)
		{
			if (MOUSE_PRESSED(Left))
			{
				m_IsClickButton = true;
				m_FuncClick(this);
				imgSpr->setTextureRect(m_Images.m_Click);
				presedFix = true;
			}
			else
				imgSpr->setTextureRect(m_Images.m_Aiming);
		}
		else if (presedFix && !MOUSE_PRESSED(Left))
		{
			presedFix = false;
		}
	}
	else
		imgSpr->setTextureRect(m_Images.m_Normal);

	m_Image->Update();
	m_Label->Update();
}

void Button::Draw()
{
	m_Image->Draw();
	m_Label->Draw();
}

void Widget::Button::OnClick(std::function<void(Button* button)> funcClick)
{
	m_FuncClick = funcClick;
}
