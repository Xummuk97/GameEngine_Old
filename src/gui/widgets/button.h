#pragma once
#include "image.h"
#include "label.h"

namespace Widget
{
	struct ButtonImages
	{
		IntRect m_Normal, m_Aiming, m_Click;
	};

	class Button : public WidgetBase
	{
	public:
		Button(Text text, const ButtonImages& images, const string& textureName, const FloatRect& region, const Vector2f& pos, const Horizontal& h = Horizontal::left, const Vertical& v = Vertical::top);
		virtual ~Button();

		Widget::Image* GetImage();
		Widget::Label* GetLabel();

		void Update();
		void Draw();

		void OnClick(std::function<void (Button* button)> funcClick);

	private:
		Widget::Image* m_Image;
		Widget::Label* m_Label;
		ButtonImages m_Images;
		
		std::function<void (Button* button)> m_FuncClick;
	};
}