///==========================================================================
///	������:		����������� ���������
///	�����:		������ �����������
///	���:		2019
///==========================================================================

#ifndef MESSAGES_INCLUDE
#define MESSAGES_INCLUDE

#include "entity.h"

// @brief ��� ���������
enum struct MessageType
{
	None, // @brief �� ����������
	Say, // @brief ��������
	Teleport // @brief ��������
};

// @brief �������������� Enum ���� ��������� � ��������� �������������
// @param type - ���
// @return ��������� �������������
string cast(const MessageType& type);

// @brief �������������� ���������� ������������� Enum ���� ��������� � ������� Enum
// @param type - ���
// @return Enum �������������
MessageType cast(const string& type);

// @brief ��������� ���� �� ����� ���
// @param type - ���
// @return ���������
bool is(const string& type);

// @brief ��������� ���������
struct sMessage
{
	// @brief ���������� ���������?
	bool is = false;

	// @brief ������
	Entity* entity = nullptr;

	// @brief ����������� �� ���������
	// @param is - ���������� ���������?
	// @param entity - ������
	sMessage(bool is, Entity* entity) 
		: is(is),
		  entity(entity)
	{}
};

// @brief ����������� ���������
class Messages 
{
public:
	// @brief �����������
	Messages() = default;

	// @brief ����������
	~Messages();

	// @brief �������������� �������������
	static void Init();

	// @brief ����������� ����������� ��������� ��� ��������
	static void Run();

	// @brief ����� ���� ����������� ���������
	static void Reset();

	// @brief �������� ���������
	// @bparam messageWidgetName - �������� �������
	static void Delete(const MessageType& type);

	// @brief �������� ���� ��������
	static void DeleteAll();

	// @brief ��������� ���������
	// @param type - ���
	// @param message - ������ � ���������
	static void SetMessage(const MessageType& type, const sMessage& message);

	// @brief ������� ��������� �� ����
	// @param type - ���
	// @return ���������
	static shared_ptr<sMessage> GetMessage(const MessageType& type);

private:
	// @brief ����������� ���������
	static map<MessageType, shared_ptr<sMessage>> m_messages;
};

#endif /// MESSAGES_INCLUDE