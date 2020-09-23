#pragma once
#include "widget.h"

namespace Widget
{
	class Label : public WidgetBase
	{
	public:
		Label(Text text, const FloatRect& region, const Vector2f& pos, const Horizontal& h = Horizontal::left, const Vertical& v = Vertical::top);
		virtual ~Label();

		void Update();
		void Draw();

		Text* GetText();

		void OnUpdate(std::function<void (Label* label)> funcUpdate);

	private:
		Text m_Text;
		std::function<void (Label* label)> m_FuncUpdate;
	};
}