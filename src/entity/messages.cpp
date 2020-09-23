///==========================================================================
///	Модуль:		Всплывающие сообщения
///	Автор:		Андрей Гребенщиков
///	Год:		2019
///==========================================================================

#include "messages.h"

#define SET_WIDGET_ORIGIN_POSITION(nameWidget, entity) \
{\
	auto position = entity->GetPosition();\
	const auto& VISIBLE = System::GetVisibleRect();\
	position.x -= VISIBLE.left + nameWidget->getSize().x / 2.0f;\
	position.y -= VISIBLE.top + 20.0f;\
	nameWidget->setPosition(position);\
}

#define MESSAGE_SHOW(messageType, strMsg) \
if (m_messages[messageType]->is)\
{\
	tgui::Widget::Ptr widget;\
\
	if ((widget = System::m_Gui->get(cast(messageType))) != nullptr)\
	{\
		SET_WIDGET_ORIGIN_POSITION(widget, m_messages[messageType]->entity);\
	}\
	else\
	{\
		String str = System::GetFormat(strMsg, GetStrKeyBoard("action"));\
		auto label = tgui::Label::create(str);\
\
		auto renderer = label->getRenderer();\
		renderer->setBackgroundColor(tgui::Color::Black);\
		renderer->setTextColor(tgui::Color::White);\
\
		SET_WIDGET_ORIGIN_POSITION(label, m_messages[messageType]->entity);\
		System::m_Gui->add(label, cast(messageType));\
	}\
}\
else\
{\
	Delete(messageType);\
	m_messages[messageType]->entity = nullptr;\
}

map<MessageType, shared_ptr<sMessage>> Messages::m_messages;

string cast(const MessageType& type)
{
	switch (type)
	{
	case MessageType::Say:
		return "message_say";
		break;

	case MessageType::Teleport:
		return "message_teleport";
		break;
	}
	return "message_none";
}

MessageType cast(const string& type)
{
	if (type == "message_say")
		return MessageType::Say;
	else if (type == "message_teleport")
		return MessageType::Teleport;
	return MessageType::None;
}

bool is(const string& type)
{
	if (type == "message_say")
		return true;
	else if (type == "message_teleport")
		return true;
	return false;
}

Messages::~Messages()
{
	DeleteAll();
}

void Messages::Init()
{
	m_messages =
	{
		{MessageType::Say, make_shared<sMessage>(false, nullptr)},
		{MessageType::Teleport, make_shared<sMessage>(false, nullptr)}
	};
}

void Messages::Run()
{
	MESSAGE_SHOW(MessageType::Say, L"Нажмите {0}, чтобы поговорить.");
	MESSAGE_SHOW(MessageType::Teleport, L"Нажмите {0}, чтобы телепортироваться.");
}

void Messages::Reset()
{
	for (auto&& [name, value] : m_messages)
		value->is = false;
}

void Messages::Delete(const MessageType& type)
{
	tgui::Widget::Ptr widget;

	if ((widget = System::m_Gui->get(cast(type))) != nullptr)
		System::m_Gui->remove(widget);
}

void Messages::DeleteAll()
{
	auto widgetVector = &System::m_Gui->getWidgets();

	for (auto it = widgetVector->begin(); it != widgetVector->end();)
	{
		string widgetName = System::m_Gui->getWidgetName(*it);

		if (is(widgetName)) System::m_Gui->remove(*it);
		else it++;
	}
}

void Messages::SetMessage(const MessageType& type, const sMessage& message)
{
	auto ptrMessage = m_messages[type];

	ptrMessage->is = message.is;
	ptrMessage->entity = message.entity;
}

shared_ptr<sMessage> Messages::GetMessage(const MessageType& type)
{
	return m_messages[type];
}
