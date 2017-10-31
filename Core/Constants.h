#pragma once

#include "Logger.h"

#include <map>
#undef max
#undef min
#include <rapidjson/document.h>
#include <vector>

namespace core
{
	class Constants
	{
	public:

		static Constants& get()
		{
			static Constants inst;
			return inst;
		}
		~Constants() = default;
		Constants(const Constants&) = delete;
		void operator=(const Constants&) = delete;

		
		void cleanup();

		bool getBool(std::string);
		std::string getString(std::string);
		std::vector<string> getStringArray(std::string);
		std::vector<bool> getBoolArray(std::string);
		std::vector<double> getDoubleArray(std::string);

		template<typename T = float, typename = typename std::enable_if_t< std::is_arithmetic<T>::value >>
		T getNum(string name);

	private:
		Constants() { init(); }

		string addJsonFileTexts(string, string);

		void init();
		
		map<string, double> m_numVals;
		map<string, string> m_stringVals;
		map<string, bool> m_boolVals;
		map<string, vector<double>> m_numArrayVals;
		map<string, vector<string>> m_stringArrayVals;
		map<string, vector<bool>> m_boolArrayVals;

		rapidjson::Document m_jsonDoc;
		vector<string> m_varNames;

		static string HEAD;
		static string PATH;
	};
}

