#include "data.h"

using namespace Widget;

Widgets::Widgets() :
	m_Count(0)
{
}

Widgets::~Widgets()
{
}

void Widgets::Add(const string& name, shared_ptr<WidgetBase> widget)
{
	m_data[to_string(m_Count) + "_" + name] = widget;
	m_Count++;
}

shared_ptr<WidgetBase> Widgets::Get(const string& name)
{
	for (auto&& [dName, dVal] : m_data)
	{
		if (dName.find(name) != string::npos)
		{
			return dVal;
		}
	}

	cout << "Widget " << name << " not found" << endl;
	return shared_ptr<WidgetBase>();
}

void Widgets::Update()
{
	for (auto it = m_data.rbegin(); it != m_data.rend(); it++)
		it->second->Update();
}

void Widgets::Draw()
{
	for (auto&& [dName, dVal] : m_data)
	{
		dVal->Draw();
	}
}

GuiData::GuiData() :
	m_Count(0),
	m_CurrentGui("none")
{
}

GuiData::~GuiData()
{
}

void GuiData::SetCurrentGui(const string& guiName)
{
	m_CurrentGui = guiName;
}

shared_ptr<Widgets> GuiData::Get(const string& layer)
{
	for (auto&& [dName, dVal] : m_data)
	{
		if (dName.find(layer) != string::npos)
		{
			return dVal;
		}
	}

	string key = to_string(m_Count) + "_" + layer;
	m_data[key] = make_shared<Widgets>();
	return m_data[key];
}

void GuiData::Update()
{
	if (m_CurrentGui == "none")
	{
		cout << "Current Gui == NONE!";
		return;
	}

	WidgetBase::m_IsClickButton = false;
	Get(m_CurrentGui)->Update();
}

void GuiData::Draw()
{
	if (m_CurrentGui == "none")
	{
		cout << "Current Gui == NONE!";
		return;
	}

	Get(m_CurrentGui)->Draw();
}
