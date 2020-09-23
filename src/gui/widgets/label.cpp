#include "label.h"

using namespace Widget;

Label::Label(Text text, const FloatRect& region, const Vector2f& pos, const Horizontal& h, const Vertical& v) :
	m_Text(text),
	WidgetBase(text.getGlobalBounds(), pos, region, h, v)
{
}

Label::~Label()
{

}

void Label::Update()
{
	WidgetBase::Update();
	m_Text.setPosition(m_BasePos);

	if (m_FuncUpdate)
		m_FuncUpdate(this);
}

void Label::Draw()
{
	m_Window->draw(m_Text);
}

Text* Label::GetText()
{
	return &m_Text;
}

void Widget::Label::OnUpdate(std::function<void(Label* label)> funcUpdate)
{
	m_FuncUpdate = funcUpdate;
}
