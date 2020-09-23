///==========================================================================
///	Модуль:		Диалоги на базе TGUI
///	Автор:		Андрей Гребенщиков
///	Год:		2019
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

// @brief Система диалогов
namespace Dialog
{
	// @brief Переопределление ядра
	class Core;

	// @brief Функция
	using Delegate = function<void (Dialog::Core* core)>;

	// @brief Переопределение пункта меню
	class Option;

	// @brief Указатель на пункт меню
	using OptionPtr = shared_ptr<Option>;

	// @brief Пункт меню
	class Option
	{
	public:
		// @brief Конструктор
		Option() = default;

		// @brief Конструктор
		// @param text - Текст
		// @param delegate - Функция
		Option(const String& text, Delegate&& delegate);

		// @brief Деструктор
		~Option() = default;

		// @brief Создание пункта меню
		static OptionPtr Create();

		// @brief Создание пункта меню
		// @param text - Текст
		// @param delegate - Функция
		static OptionPtr Create(const String& text, Delegate&& delegate);

		// @brief Установка текста
		// @param text - Текст
		void SetText(const String& text);

		// @return Текст
		String GetText() const;

		// @brief Установка функции
		// @param delegate - Функция
		void SetDelegate(Delegate&& delegate);

		// @return Функция
		Delegate& GetDelegate();

	private:
		// @brief Текст
		String m_text;	

		// @brief Функция
		Delegate m_delegate;
	};

	// @brief Переопределение базы диалога
	class Base;

	// @brief Указатель на базы диалога
	using DialogPtr = shared_ptr<Base>;

	// @brief База диалога
	class Base
	{
	public:
		// @brief Конструктор
		Base() = default;

		// @brief Деструктор
		~Base() = default;

		// @brief Создание базы диалога
		static DialogPtr Create();

		// @brief Установка текста
		// @param text - Текст
		void SetText(const String& text);

		// @return Текст
		String GetText() const;

		// @brief Добавить пункт меню
		// @param option - Пункт меню
		void AddOption(OptionPtr&& option);

		// @brief Возврат пункта меню по индексу
		// @param index - Индекс
		// @return Пункт меню
		OptionPtr& GetOption(int index);

		// @return Пункты меню
		vector<OptionPtr>& GetOption();

	private:
		// @brief Пункты меню
		vector<OptionPtr> m_options;	

		// @brief Текст
		String m_text;					
	};

	// @brief Контейнер
	class Container
	{
	public:
		// @brief Конструктор
		Container() = default;

		// @brief Деструктор
		~Container() = default;

		// @brief Добавление нового диалога
		// @param key - Ключ
		// @param dialog - Диалог
		void Add(const string& key, DialogPtr&& dialog);

		// @brief Возвращение диалога по ключу
		// @param key - Ключ
		// @return Диалог
		DialogPtr& Get(const string& key);

		// @return Текущий диалог
		DialogPtr& Get();

		// @return Диалоги
		map<string, DialogPtr>& GetData();

		// @brief Установка текущего диалога
		// @param current - Текущий диалог
		void SetCurrent(const string& current);

		// @brief Удаление диалога по ключу
		// @param key - Ключ
		void Delete(const string& key);

		// @brief Удаление всех диалогов
		void DeleteAll();

	private:
		// @brief Диалоги
		map<string, DialogPtr> m_dialogs;	

		// @brief Текущий диалог
		string m_current;				
	};

	// @brief Ядро диалогов
	class Core : public Container
	{
	public:
		// @brief Конструктор
		Core();

		// @brief Конструктор
		// @param gui - TGUI GUI объект
		Core(tgui::Gui* gui);

		// @brief Деструктор
		virtual ~Core() = default;

		// @brief Создать (отобразить) текущий диалог
		void Create();

		// @brief Отключить текущий диалог
		void Close();

		// @brief Функция, выполняемая при отключении диалога
		// @param delegate - Функция
		void OnClose(Delegate&& delegate);

		// @brief Установка TGUI GUI объекта
		// @param gui - TGUI GUI объект
		void SetGui(tgui::Gui* gui);

		// @return TGUI GUI объект
		tgui::Gui* GetGui();

		// @return Базовое окно диалогов от TGUI
		tgui::ChildWindow::Ptr& GetChildWindow();

		// @brief Установка текущей персоны
		// @param person - Персона
		void SetPerson(const Person& person);

		// @return Текущая персона
		Person& GetPerson();

		// @brief Добавить сообщение 
		// @param message - Сообщение
		// @param person - Персона
		void AddMessage(const String& message, const Person& person);

	private:
		// @brief Счётчик сообщений диалога
		size_t m_widgetMessageCount;

		// @brief TGUI GUI объект
		tgui::Gui* m_gui;		

		// @brief TGUI Базовое окно диалога
		tgui::ChildWindow::Ptr m_childWindow;	

		// @brief Текущая персона
		Person m_person;

		// @brief Функция, вызываемая при отключении диалога
		Delegate m_OnClose;
	};
}

#endif /// DIALOGS_INCLUDE