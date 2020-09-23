#pragma once
#include "..\core\system.h"
#include "widgets\widget.h"
#include "widgets\label.h"
#include "widgets\image.h"
#include "widgets\button.h"

class Widgets
{
public:
	Widgets();
	~Widgets();

	void Add(const string& name, shared_ptr<Widget::WidgetBase> widget);
	shared_ptr<Widget::WidgetBase> Get(const string& name);
	void Update();
	void Draw();

private:
	int m_Count;
	map<string, shared_ptr<Widget::WidgetBase>> m_data;
};

class GuiData
{
public:
	GuiData();
	~GuiData();

	void SetCurrentGui(const string& guiName);

	shared_ptr<Widgets> Get(const string& layer);

	void Update();
	void Draw();

private:
	int m_Count;
	string m_CurrentGui;
	map<string, shared_ptr<Widgets>> m_data;
};