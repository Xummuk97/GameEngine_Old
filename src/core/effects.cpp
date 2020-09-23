///==========================================================================
///	Модуль:		Эффекты (Подключаемые шейдеры)
///	Автор:		Андрей Гребенщиков
///	Год:		2019
///==========================================================================

#include "effects.h"
#include <cmath>

using namespace Effects;

Base::Base(const Type& type) 
	: m_type(type)
{
}


Shader* Base::GetShader()
{
	return &m_shader;
}

Type& Base::GetType()
{
	return m_type;
}


Blur::Blur() : Base(Type::Blur)
{
	m_shader.loadFromFile(PATH_SHADERS + "blur.frag", Shader::Fragment);
	m_shader.setUniform("source", Shader::CurrentTexture);
}

Blur::Blur(const Vector2f& offsetFactor) : Base(Type::Blur)
{
	m_shader.loadFromFile(PATH_SHADERS + "blur.frag", Shader::Fragment);
	m_shader.setUniform("source", Shader::CurrentTexture);
	SetOffsetFactor(offsetFactor);
}

void Blur::SetOffsetFactor(const Vector2f& offsetFactor)
{
	m_shader.setUniform("offsetFactor", offsetFactor);
}

Effects::Color::Color() : Base(Type::Color)
{
	m_shader.loadFromFile(PATH_SHADERS + "color.frag", Shader::Fragment);
	m_shader.setUniform("source", Shader::CurrentTexture);
}

Effects::Color::~Color()
{
}

void Effects::Color::SetColor(const Vector3f& color)
{
	m_shader.setUniform("color", color);
}
