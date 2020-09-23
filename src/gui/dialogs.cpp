///==========================================================================
///	Модуль:		Диалоги на базе TGUI
///	Автор:		Андрей Гребенщиков
///	Год:		2019
///==========================================================================

#include "dialogs.h"

using namespace Dialog;

// CLASS Option
Option::Option(const String& text, Delegate&& delegate) 
	: m_text(text), 
	  m_delegate(std::move(delegate))
{
}

OptionPtr Option::Create()
{
	return make_shared<Option>();
}

OptionPtr Option::Create(const String& text, Delegate&& delegate)
{
	return make_shared<Option>(text, std::move(delegate));
}

void Option::SetText(const String& text)
{
	m_text = text;
}

String Option::GetText() const
{
	return m_text;
}

void Option::SetDelegate(Delegate&& delegate)
{
	m_delegate = std::move(delegate);
}

Delegate& Option::GetDelegate()
{
	return m_delegate;
}

// CLASS Base
DialogPtr Base::Create()
{
	return make_shared<Base>();
}

void Base::SetText(const String& text)
{
	m_text = text;
}

String Base::GetText() const
{
	return m_text;
}

void Base::AddOption(OptionPtr&& option)
{
	m_options.push_back(std::move(option));
}

OptionPtr& Base::GetOption(int index)
{
	return m_options[index];
}

vector<OptionPtr>& Base::GetOption()
{
	return m_options;
}

// CLASS Container
void Container::Add(const string& key, DialogPtr&& dialog)
{
	m_dialogs[key] = std::move(dialog);
	SetCurrent(key);
}

DialogPtr& Container::Get(const string& key)
{
	return m_dialogs[key];
}

DialogPtr& Container::Get()
{
	return m_dialogs[m_current];
}

map<string, DialogPtr>& Container::GetData()
{
	return m_dialogs;
}

void Container::SetCurrent(const string& current)
{
	m_current = current;
}

void Container::Delete(const string& key)
{
	m_dialogs.erase(key);
}

void Container::DeleteAll()
{
	for (auto it = m_dialogs.begin(); it != m_dialogs.end();)
	{
		if (!m_dialogs.empty())
		{
			it = m_dialogs.erase(it);
			it++;
		}
	}
}

// CLASS Core
Core::Core() :
	m_widgetMessageCount(0)
{
	m_childWindow = tgui::ChildWindow::create(L"Окно диалогов", tgui::ChildWindow::TitleButton::None);
	m_childWindow->setSize("60%", "40%");
	m_childWindow->setPositionLocked(true);
	m_childWindow->setPosition("(&.size - size) / 2");

	auto scrollMessage = tgui::ScrollablePanel::create({ "90%", "50%" });
	scrollMessage->setPosition("5%", "5%");
	scrollMessage->setHorizontalScrollbarPolicy(tgui::Scrollbar::Policy::Never);
	m_childWindow->add(scrollMessage, "scrollMessage");

	auto scrollOptions = tgui::ScrollablePanel::create({ "90%", "30%" });
	scrollOptions->setPosition("5%", "scrollMessage.bottom + 1%");
	scrollOptions->setHorizontalScrollbarPolicy(tgui::Scrollbar::Policy::Never);
	m_childWindow->add(scrollOptions, "scrollOptions");
}

Core::Core(tgui::Gui* gui) 
	: m_gui(gui)
{
	Core();
}

void Core::Create()
{
	// Получаем текущий диалог
	const auto& dialog = Get();

	// Удалить объект окна диалогов из глобального GUI
	m_gui->remove(m_childWindow);

	// Устанавливаем текст сообщения
	AddMessage(dialog->GetText(), m_person);

	// Устанавливаем варианты
	auto scrollOptions = m_childWindow->get<tgui::ScrollablePanel>("scrollOptions");

	scrollOptions->removeAllWidgets();

	static size_t widgetOptionCount = 0;
	widgetOptionCount = 0;

	for (auto& option : dialog->GetOption())
	{
		auto widgetOption = tgui::Label::create(to_string(widgetOptionCount + 1) + ". " + option->GetText());
		
		widgetOption->connect("Clicked", [&](Core* core)
		{
			AddMessage(option->GetText(), PERSON_PLAYER);
			option->GetDelegate()(core);

		}, this);
		
		if (widgetOptionCount > 0)
			widgetOption->setPosition(0, "option" + to_string(widgetOptionCount - 1) + ".bottom + 2%");

		scrollOptions->add(widgetOption, "option" + to_string(widgetOptionCount));

		widgetOptionCount++;
	}

	// Добавляем объект окна диалогов в глобальный GUI
	m_gui->add(m_childWindow, "childWindowDialog");
}

void Core::Close()
{
	m_childWindow->close();

	if (m_OnClose) m_OnClose(this);

	m_childWindow->get<tgui::ScrollablePanel>("scrollMessage")->removeAllWidgets();

	m_widgetMessageCount = 0;
}

void Core::OnClose(Delegate&& delegate)
{
	m_OnClose = std::move(delegate);
}

void Core::SetGui(tgui::Gui* gui)
{
	m_gui = gui;
}

tgui::Gui* Core::GetGui()
{
	return m_gui;
}

tgui::ChildWindow::Ptr& Core::GetChildWindow()
{
	return m_childWindow;
}

void Core::SetPerson(const Person& person)
{
	m_person = person;
}

Person& Core::GetPerson()
{
	return m_person;
}

void Core::AddMessage(const String& message, const Person& person)
{
	auto scrollMessage = m_childWindow->get<tgui::ScrollablePanel>("scrollMessage");
	auto panel = tgui::Panel::create({0, 0});
	auto renderer1 = panel->getRenderer();

	renderer1->setBorders(tgui::Borders(1));
	renderer1->setBackgroundColor(Color(240, 240, 240));
	renderer1->setBorderColor(Color(200, 200, 200));

	auto labelAuthor = tgui::Label::create(person.name);
	auto renderer2 = labelAuthor->getRenderer();

	labelAuthor->setSize("100%", labelAuthor->getSize().y);
	renderer2->setTextColor(person.nameColor);
	renderer2->setBorders(tgui::Borders(0, 0, 0, 1));
	renderer2->setBorderColor(Color(200, 200, 200));
	renderer2->setBackgroundColor(Color(160, 160, 160));

	if (m_widgetMessageCount > 0)
		panel->setPosition(0, "panel" + to_string(m_widgetMessageCount - 1) + ".bottom + 2%");

	const auto& labelAuthorStr = "labelAuthor" + to_string(m_widgetMessageCount);
	panel->setSize(panel->getSize() + labelAuthor->getSize());
	panel->add(labelAuthor, labelAuthorStr);

	auto labelMessage = tgui::Label::create(message);
	labelMessage->setPosition("1%", labelAuthorStr + ".bottom");
	labelMessage->getRenderer()->setTextColor(person.textColor);

	panel->setSize(panel->getSize() + labelMessage->getSize());
	panel->add(labelMessage, "labelMessage" + to_string(m_widgetMessageCount));
	panel->setSize("100%", panel->getSize().y);

	scrollMessage->add(panel, "panel" + to_string(m_widgetMessageCount));

	m_widgetMessageCount++;
}
