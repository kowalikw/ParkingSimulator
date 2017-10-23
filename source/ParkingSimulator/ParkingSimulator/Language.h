#ifndef LANGUAGE_H
#define LANGUAGE_H

#include <vector>
#include <string>
#include <map>
#include <qsettings.h>

class Language
{
public:
	static Language *getInstance()
	{
		if (!Language::s_instance)
			Language::s_instance = new Language;
		return Language::s_instance;
	}

	void LoadLanguage(std::string lang);

	std::map<std::string, std::string> GetDictionary();
private:
	static Language *s_instance;
	Language()
	{
		LoadLanguage("pl");
	}

	std::map<std::string, std::string> dictionary;
};

#endif