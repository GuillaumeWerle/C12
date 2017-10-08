#pragma once
class DXMacro
{
public:
	std::string m_name;
	std::string m_definition;

	DXMacro() {}
	DXMacro(const char * name, const char * def = "1")
	{
		m_name = name;
		m_definition = def;
	}
};

