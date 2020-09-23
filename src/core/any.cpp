/*==========================================================
	Модуль:		Универсальный тип
	Автор:		Андрей Гребенщиков
	Год:		2019
==========================================================*/

#include "any.h"

void AnySTD::StringToValue(const string& value)
{
	// array?
	if (value.front() == '[' && value.back() == ']')
	{
		vector<AnySTD> vec;	
		string word;	

		const size_t& SIZE = value.size();
		for (size_t i = 1; i < SIZE - 1; i++)
		{
			if (value[i] != ' ' && value[i] != ',')
			{
				word += value[i];
			}

			if (value[i] == ' ' || value[i] == ',' || i == SIZE - 2)
			{
				if (word != "\0")
				{
					AnySTD val;
					val.StringToValue(word);
					vec.push_back(val);

					word.clear();
				}
			}
		}

		m_value = vec; 
		return;
	}

	// bool?
	if (value == "true" || value == "false")
	{
		m_value = (value == "true");
		return;
	}

	// string?
	for (auto& c : value)
	{
		if (c != '.')
		{
			if (c < '0' || c > '9')
			{
				m_value = static_cast<string>(value);
				return;
			}
		}
	}

	// float?
	if (value.find('.') != string::npos)
		m_value = stof(value);

	// int?
	else if (value != "")
		m_value = stoi(value);
}
