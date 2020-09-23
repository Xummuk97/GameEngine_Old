///==========================================================================
///	������:		������� �� ���� TGUI
///	�����:		������ �����������
///	���:		2019
///==========================================================================

#ifndef DIALOGS_INCLUDE
#define DIALOGS_INCLUDE

#include <TGUI/TGUI.hpp>
#include <SFML/System/String.hpp>
#include <vector>
#include <memory>
#include <string>
#include <iostream>
#include "../entity/person.h"

using namespace std;
using namespace sf;

// @brief ������� ��������
namespace Dialog
{
	// @brief ���������������� ����
	class Core;

	// @brief �������
	using Delegate = function<void (Dialog::Core* core)>;

	// @brief ��������������� ������ ����
	class Option;

	// @brief ��������� �� ����� ����
	using OptionPtr = shared_ptr<Option>;

	// @brief ����� ����
	class Option
	{
	public:
		// @brief �����������
		Option() = default;

		// @brief �����������
		// @param text - �����
		// @param delegate - �������
		Option(const String& text, Delegate&& delegate);

		// @brief ����������
		~Option() = default;

		// @brief �������� ������ ����
		static OptionPtr Create();

		// @brief �������� ������ ����
		// @param text - �����
		// @param delegate - �������
		static OptionPtr Create(const String& text, Delegate&& delegate);

		// @brief ��������� ������
		// @param text - �����
		void SetText(const String& text);

		// @return �����
		String GetText() const;

		// @brief ��������� �������
		// @param delegate - �������
		void SetDelegate(Delegate&& delegate);

		// @return �������
		Delegate& GetDelegate();

	private:
		// @brief �����
		String m_text;	

		// @brief �������
		Delegate m_delegate;
	};

	// @brief ��������������� ���� �������
	class Base;

	// @brief ��������� �� ���� �������
	using DialogPtr = shared_ptr<Base>;

	// @brief ���� �������
	class Base
	{
	public:
		// @brief �����������
		Base() = default;

		// @brief ����������
		~Base() = default;

		// @brief �������� ���� �������
		static DialogPtr Create();

		// @brief ��������� ������
		// @param text - �����
		void SetText(const String& text);

		// @return �����
		String GetText() const;

		// @brief �������� ����� ����
		// @param option - ����� ����
		void AddOption(OptionPtr&& option);

		// @brief ������� ������ ���� �� �������
		// @param index - ������
		// @return ����� ����
		OptionPtr& GetOption(int index);

		// @return ������ ����
		vector<OptionPtr>& GetOption();

	private:
		// @brief ������ ����
		vector<OptionPtr> m_options;	

		// @brief �����
		String m_text;					
	};

	// @brief ���������
	class Container
	{
	public:
		// @brief �����������
		Container() = default;

		// @brief ����������
		~Container() = default;

		// @brief ���������� ������ �������
		// @param key - ����
		// @param dialog - ������
		void Add(const string& key, DialogPtr&& dialog);

		// @brief ����������� ������� �� �����
		// @param key - ����
		// @return ������
		DialogPtr& Get(const string& key);

		// @return ������� ������
		DialogPtr& Get();

		// @return �������
		map<string, DialogPtr>& GetData();

		// @brief ��������� �������� �������
		// @param current - ������� ������
		void SetCurrent(const string& current);

		// @brief �������� ������� �� �����
		// @param key - ����
		void Delete(const string& key);

		// @brief �������� ���� ��������
		void DeleteAll();

	private:
		// @brief �������
		map<string, DialogPtr> m_dialogs;	

		// @brief ������� ������
		string m_current;				
	};

	// @brief ���� ��������
	class Core : public Container
	{
	public:
		// @brief �����������
		Core();

		// @brief �����������
		// @param gui - TGUI GUI ������
		Core(tgui::Gui* gui);

		// @brief ����������
		virtual ~Core() = default;

		// @brief ������� (����������) ������� ������
		void Create();

		// @brief ��������� ������� ������
		void Close();

		// @brief �������, ����������� ��� ���������� �������
		// @param delegate - �������
		void OnClose(Delegate&& delegate);

		// @brief ��������� TGUI GUI �������
		// @param gui - TGUI GUI ������
		void SetGui(tgui::Gui* gui);

		// @return TGUI GUI ������
		tgui::Gui* GetGui();

		// @return ������� ���� �������� �� TGUI
		tgui::ChildWindow::Ptr& GetChildWindow();

		// @brief ��������� ������� �������
		// @param person - �������
		void SetPerson(const Person& person);

		// @return ������� �������
		Person& GetPerson();

		// @brief �������� ��������� 
		// @param message - ���������
		// @param person - �������
		void AddMessage(const String& message, const Person& person);

	private:
		// @brief ������� ��������� �������
		size_t m_widgetMessageCount;

		// @brief TGUI GUI ������
		tgui::Gui* m_gui;		

		// @brief TGUI ������� ���� �������
		tgui::ChildWindow::Ptr m_childWindow;	

		// @brief ������� �������
		Person m_person;

		// @brief �������, ���������� ��� ���������� �������
		Delegate m_OnClose;
	};
}

#endif /// DIALOGS_INCLUDE