///==========================================================================
///	������:		���������� ������ ��������, ������� ��������� �� TGUI
///	�����:		������ �����������
///	���:		2019
///==========================================================================

#ifndef IMAGE_INCLUDE
#define IMAGE_INCLUDE

#include "widget.h"
#include "../../core/effects.h"

namespace Widget
{
	class Image : public WidgetBase
	{
	public:
		Image(Sprite sprite, const FloatRect& region, const Vector2f& pos, const Horizontal& h = Horizontal::left, const Vertical& v = Vertical::top);
		virtual ~Image();

		void Update();
		void Draw();

		Sprite* GetSprite();

	private:
		Sprite m_Sprite;
	};
}

#endif /// IMAGE_INCLUDE