///==========================================================================
///	Модуль:		Всплывающие сообщения
///	Автор:		Андрей Гребенщиков
///	Год:		2019
///==========================================================================

#ifndef MESSAGES_INCLUDE
#define MESSAGES_INCLUDE

#include "entity.h"

// @brief Тип сообщений
enum struct MessageType
{
	None, // @brief Не определено
	Say, // @brief Разговор
	Teleport // @brief Телепорт
};

// @brief Преобразование Enum Типа сообщения в строковое представление
// @param type - Тип
// @return Строковое представление
string cast(const MessageType& type);

// @brief Преобразование строкового представления Enum Типа сообщения в обычный Enum
// @param type - Тип
// @return Enum представление
MessageType cast(const string& type);

// @brief Проверяет есть ли такой тип
// @param type - Тип
// @return Результат
bool is(const string& type);

// @brief Структура сообщения
struct sMessage
{
	// @brief Показывать сообщение?
	bool is = false;

	// @brief Объект
	Entity* entity = nullptr;

	// @brief Конструктор по умолчанию
	// @param is - Показывать сообщение?
	// @param entity - Объект
	sMessage(bool is, Entity* entity) 
		: is(is),
		  entity(entity)
	{}
};

// @brief Всплывающие сообщения
class Messages 
{
public:
	// @brief Конструктор
	Messages() = default;

	// @brief Деструктор
	~Messages();

	// @brief Первоначальная инициализация
	static void Init();

	// @brief Отображение всплывающей подсказки над объектом
	static void Run();

	// @brief Сброс всех всплывающих сообщений
	static void Reset();

	// @brief Удаление сообщение
	// @bparam messageWidgetName - Название виджета
	static void Delete(const MessageType& type);

	// @brief Удаление всех виджетов
	static void DeleteAll();

	// @brief Установка сообщения
	// @param type - Тип
	// @param message - Данные о сообщении
	static void SetMessage(const MessageType& type, const sMessage& message);

	// @brief Возврат сообщения по типу
	// @param type - Тип
	// @return Сообщение
	static shared_ptr<sMessage> GetMessage(const MessageType& type);

private:
	// @brief Всплывающие сообщения
	static map<MessageType, shared_ptr<sMessage>> m_messages;
};

#endif /// MESSAGES_INCLUDE