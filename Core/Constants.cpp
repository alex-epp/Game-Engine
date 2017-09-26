#include "Constants.h"

#include <fstream>
#include <regex>
#include <string>

using namespace std;

namespace core
{
	string Constants::HEAD = "constants.json";
	string Constants::PATH = "../data/constants/";


	string Constants::addJsonFileTexts(string first, string second)
	{
		auto firstOpenPos = first.find_first_of('{');
		auto firstClosePos = first.find_last_of('}');
		auto secondOpenPos = second.find_first_of('{');
		auto secondClosePos = second.find_last_of('}');

		if (firstOpenPos == string::npos)
			first = "{" + first;

		if (firstClosePos != string::npos)
			first.replace(firstClosePos, 1, ",");

		if (secondOpenPos != string::npos)
			second.replace(secondOpenPos, 1, " ");

		if (secondClosePos == string::npos)
			second += "}";

		return first + second;
	}

	void Constants::init()
	{
		ifstream file(Constants::PATH + Constants::HEAD);
		if (file.fail())
		{
			LOG_ERR("Cannot load base configuration file: ", Constants::PATH, Constants::HEAD);
			return;
		}
		string head{ istreambuf_iterator<char>(file), istreambuf_iterator<char>() };

		if (m_jsonDoc.Parse(head.c_str()).HasParseError())
		{
			LOG_ERR("Error reading ", Constants::PATH, Constants::HEAD, " at ", head.substr(m_jsonDoc.GetErrorOffset(), 10));
			return;
		}
		
		string json;
		if (m_jsonDoc.HasMember("constant_files") && m_jsonDoc["constant_files"].IsArray())
		{
			for (auto& filename : m_jsonDoc["constant_files"].GetArray())
			{
				ifstream subfile(Constants::PATH + filename.GetString());
				if (subfile.fail())
				{
					LOG_ERR("Cannot load configuration file: ", Constants::PATH, filename.GetString());
					continue;
				}
				string curFile{ istreambuf_iterator<char>(subfile), istreambuf_iterator<char>() };
				json = addJsonFileTexts(json, curFile);
			}
			json = addJsonFileTexts(json, head);
			if (m_jsonDoc.Parse(json.c_str()).HasParseError())
			{
				LOG_ERR("Error reading at ", json.substr(m_jsonDoc.GetErrorOffset(), 10));
				return;
			}
		}
	}

	bool Constants::getBool(string name)
	{
		return m_jsonDoc[name.c_str()].GetBool();
	}
	string Constants::getString(string name)
	{
		return m_jsonDoc[name.c_str()].GetString();
	}
	template <> int Constants::getNum<int>(string name)
	{
		return m_jsonDoc[name.c_str()].GetInt();
	}
	template <> float Constants::getNum<float>(string name)
	{
		return m_jsonDoc[name.c_str()].GetFloat();
	}
	vector<bool> Constants::getBoolArray(string name)
	{
		if (!m_jsonDoc.HasMember(name.c_str()))
		{
			LOG_ERR("Expected \"", name, "\" to exist");
			return vector<bool>();
		}
		if (!m_jsonDoc[name.c_str()].IsArray())
		{
			LOG_ERR("Expected \"", name, "\" to be an array");
			return vector<bool>();
		}
		vector<bool> re;
		for (auto& b : m_jsonDoc[name.c_str()].GetArray())
		{
			re.emplace_back(b.GetBool());
		}
		return re;
	}
	vector<double> Constants::getDoubleArray(string name)
	{
		if (!m_jsonDoc.HasMember(name.c_str()))
		{
			LOG_ERR("Expected \"", name, "\" to exist");
			return vector<double>();
		}
		if (!m_jsonDoc[name.c_str()].IsArray())
		{
			LOG_ERR("Expected \"", name, "\" to be an array");
			return vector<double>();
		}
		vector<double> re;
		for (auto& d : m_jsonDoc[name.c_str()].GetArray())
		{
			re.emplace_back(d.GetDouble());
		}
		return re;
	}
	vector<string> Constants::getStringArray(string name)
	{
		if (!m_jsonDoc.HasMember(name.c_str()))
		{
			LOG_ERR("Expected \"", name, "\" to exist");
			return vector<string>();
		}
		if (!m_jsonDoc[name.c_str()].IsArray())
		{
			LOG_ERR("Expected \"", name, "\" to be an array");
			return vector<string>();
		}
		vector<string> re;
		for (auto& s : m_jsonDoc[name.c_str()].GetArray())
		{
			re.emplace_back(s.GetString());
		}
		return re;
	}

	void Constants::cleanup()
	{
	}
}
