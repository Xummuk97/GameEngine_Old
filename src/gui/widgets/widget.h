///==========================================================================
///	Модуль:		Устаревшая база виджетов, следует перевести на TGUI
///	Автор:		Андрей Гребенщиков
///	Год:		2019
///==========================================================================

#ifndef WIDGET_INCLUDE
#define WIDGET_INCLUDE

#include "..\..\core\system.h"

namespace Widget
{
	enum class Horizontal
	{
		left, center, right
	};

	enum class Vertical
	{
		top, middle, bottom
	};

	class WidgetBase 
	{
	public:
		WidgetBase(const FloatRect& objRect, const Vector2f& pos, const FloatRect& region = FloatRect(0.0f, 0.0f, 0.0f, 0.0f), const Horizontal& h = Horizontal::left, const Vertical& v = Vertical::top);
		virtual ~WidgetBase();

		virtual void Update();
		virtual void Draw() = 0;

		bool Contains(float x, float y);

		FloatRect GetRegion() const;
		Vector2f GetPos() const;
		Vector2f GetBasePos() const;

		void SetAbsolute(bool is);
		bool IsAbsolute() const;

		void SetPos(const Vector2f& pos);

		template<class T>
		constexpr T* To();

		static bool m_IsClickButton;

	private:
		bool m_isAbsolute = false;
		Vector2f m_pos;

	protected:
		Vector2f m_BasePos;
	
	private:
		FloatRect m_region;
	};

	template<class T>
	constexpr inline T* WidgetBase::To()
	{
		return (T*)(this);
	};
}

#endif /// WIDGET_INCLUDE