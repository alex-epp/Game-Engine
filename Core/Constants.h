#pragma once
#include <map>
#include <vector>
#include "Logger.h"
using namespace std;

extern "C"
{
	#include <lua.h>
	#include <lauxlib.h>
	#include <lualib.h>
}

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
		Constants::~Constants() = default;
		Constants::Constants(const Constants&) = delete;
		void operator=(const Constants&) = delete;

		
		void cleanup();

		bool getBool(const string& name) const noexcept
		{
			auto it = boolVals.find(name);
			if (it != boolVals.end())
			{
				return it->second;
			}
			else
			{
				LOG_ERR("Key ", name, " is not in boolVals");
				return false;
			}
		}
		const string &getString(const string& name) const noexcept
		{
			auto it = stringVals.find(name);
			if (it != stringVals.end())
			{
				return it->second;
			}
			else
			{
				LOG_ERR("Key ", name, " is not in stringVals");
				static string blank = "";
				return blank;
			}
		}

		template<typename T = float, typename = typename std::enable_if_t< std::is_arithmetic<T>::value >>
		T getNum(const string& name) const noexcept
		{
			static_assert(std::is_arithmetic<T>::value, "Invalid template argument to Constants::getNum - T must be arithmetic (integral or floating-point)");

			auto it = numVals.find(name);
			if (it != numVals.end())
			{
				return static_cast<T>(it->second);
			}
			else
			{
				LOG_ERR("Key ", name, " is not in numVals");
				return T();
			}
		}

		const vector<bool>& getBoolArray(const string& name) const noexcept
		{
			auto it = boolArrayVals.find(name);
			if (it != boolArrayVals.end())
			{
				return it->second;
			}
			else
			{
				LOG_ERR("Key ", name, " is not in boolArrayVals");
				return vector<bool>();
			}
		}
		const vector<string>& getStringArray(const string& name) const noexcept
		{
			auto it = stringArrayVals.find(name);
			if (it != stringArrayVals.end())
			{
				return it->second;
			}
			else
			{
				LOG_ERR("Key ", name, " is not in stringArrayVals");
				static vector<string> v;
				return v;
			}
		}
		
		const vector<double>& getNumArray(const string& name) const noexcept // No template here, because each element in the vector would have to be converted
		{
			auto it = numArrayVals.find(name);
			if (it != numArrayVals.end())
			{
				return it->second;
			}
			else
			{
				LOG_ERR("Key ", name, " is not in numArrayVals");
				return vector<double>();
			}
		}
	private:
		Constants::Constants() { init(); }


		void init();
		void setVarNames();
		void loadTable(const string& name, int index);

		map<string, double> numVals;
		map<string, string> stringVals;
		map<string, bool> boolVals;
		map<string, vector<double>> numArrayVals;
		map<string, vector<string>> stringArrayVals;
		map<string, vector<bool>> boolArrayVals;

		lua_State* L;
		vector<string> varNames;

		static const char* FILENAME;
	};
}

