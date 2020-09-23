///==========================================================================
///	������:		������� (������������ �������)
///	�����:		������ �����������
///	���:		2019
///==========================================================================


#ifndef EFFECTS_INCLUDE
#define EFFECTS_INCLUDE

#include <SFML/Graphics/Shader.hpp>
#include <string>

using namespace sf;

namespace Effects
{
	// @brief ���� �� ����� � ���������
	const std::string PATH_SHADERS = "resources/shaders/";

	// @brief ��� �������
	enum struct Type
	{
		Blur, // @brief ��������
		Color // @brief ������ ����
	};

	// @brief ��� ��������
	class Base
	{
	public:
		// @brief ����������� � ����������
		// @param type - ��� �������
		Base(const Effects::Type& type);

		// @brief ����������
		~Base() = default;

		// @brief ���������� � T (�������)
		// @return ���������� ������
		template <class T>
		T& Cast() { return *(T*)this; };

		// @return ��������� �� ������
		Shader* GetShader();

		// @return ��� �������
		Effects::Type& GetType();

	protected:
		// @brief ��� �������
		Effects::Type m_type;

		// @brief ������
		Shader m_shader;
	};

	// @brief Blur (��������)
	class Blur : public Base
	{
	public:
		// @brief �����������
		Blur();

		// @brief ����������� � ����������
		// @param offsetFactor - ����������� � ���� ��������
		Blur(const Vector2f& offsetFactor);

		// @brief ����������
		virtual ~Blur() = default;

		// @brief ��������� ����������� � ���� ��������
		// @param offsetFactor - ����������� � ���� ��������
		void SetOffsetFactor(const Vector2f& offsetFactor);
	};

	// @brief Color (����)
	class Color : public Base
	{
	public:
		// @brief �����������
		Color();

		// @brief ����������
		~Color();

		// @brief ��������� �����
		// @param color - ����
		void SetColor(const Vector3f& color);
	};
}

#endif /// EFFECTS_INCLUDE