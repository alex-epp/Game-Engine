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
		template<typename T>
		Constants::Constants(T&& inst) = delete;

		
		void cleanup();

		bool getBool(string name) const noexcept
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
		const string &getString(string name) const noexcept
		{
			auto it = stringVals.find(name);
			if (it != stringVals.end())
			{
				return it->second;
			}
			else
			{
				LOG_ERR("Key ", name, " is not in stringVals");
				return "";
			}
		}

		template<typename T = float, typename = typename std::enable_if_t< std::is_arithmetic<T>::value >>
		T getNum(string name) const noexcept
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

	private:
		Constants::Constants() { init(); }


		void init();
		void setVarNames();

		map<string, double> numVals;
		map<string, string> stringVals;
		map<string, bool> boolVals;
		lua_State* L;
		vector<string> varNames;

		static const char* FILENAME;
	};
}

