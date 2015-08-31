#include "Constants.h"
#include <regex>

namespace core
{
	const char* Constants::FILENAME = "../data/constants.lua";
	
	
	void Constants::init()
	{
		// Initialize Lua
		L = luaL_newstate();
		luaL_openlibs(L);

		if (luaL_loadfile(L, FILENAME) || lua_pcall(L, 0, 0, 0))
		{
			LOG_ERR("Cannot run configuration file: ", lua_tostring(L, -1));
			return;
		}

		setVarNames();

		for (const auto& name: varNames)
		{
			lua_getglobal(L, name.c_str());
			if (lua_isnumber(L, -1))
				numVals[name] = lua_tonumber(L, -1);
			else if (lua_isstring(L, -1))
				stringVals[name] = lua_tostring(L, -1);
			else if (lua_isboolean(L, -1))
				boolVals[name] = static_cast<bool>(lua_toboolean(L, -1));
			else if (lua_istable(L, -1))
				loadTable(name, -1);

			lua_pop(L, 1);
		}

		/*lua_getglobal(L, "_G");
		if (lua_isnil(L, -1))
		{
			LOG_ERR("_G cannot be found");
			return;
		}
		lua_pushnil(L);
		while (lua_next(L, -2))
		{
			LOG("Variable: ", lua_tonumber(L, -1));
			lua_pop(L, 1);
		}*/
	}

	void Constants::cleanup()
	{
		lua_pop(L, lua_gettop(L));
		lua_close(L);
	}

	// Mostly from https://eliasdaler.wordpress.com/2013/10/20/lua_and_cpp_pt2/
	void Constants::setVarNames()
	{
		std::string code =
			"function getKeys(name) "
			"s = \"\""
			"for k, v in pairs(_G) do "
			"    s = s..k..\",\" "
			"    end "
			"return s "
			"end"; // function for getting table keys
		luaL_loadstring(L,
			code.c_str()); // execute code
		lua_pcall(L, 0, 0, 0);
		lua_getglobal(L, "getKeys"); // get function
		lua_pushstring(L, ""); // NOT NEEDED - I just don't know enough about Lua to remove it
		lua_pcall(L, 1, 1, 0); // execute function

		std::string test = lua_tostring(L, -1);
		varNames.clear();
		std::string temp = "";

		for (unsigned int i = 0; i < test.size(); i++) {
			if (test.at(i) != ',') {
				temp += test.at(i);
			}
			else {
				varNames.push_back(temp);
				temp = "";
			}
		}
		lua_pop(L, lua_gettop(L));
	}

	void Constants::loadTable(const string& name, int index)
	{
		lua_pushnil(L);

		//index--;

		while (lua_next(L, -2))
		{
			if (lua_isnumber(L, -1))
				numArrayVals[name].push_back(lua_tonumber(L, -1));
			else if (lua_isstring(L, -1))
				stringArrayVals[name].push_back(lua_tostring(L, -1));
			else if (lua_isboolean(L, -1))
				boolArrayVals[name].push_back(lua_toboolean(L, -1));
			else if (lua_istable(L, -1))
				LOG_WARN("Constants::loadTable cannot load multidimensional tables");

			
			lua_pop(L, 1);

		}
	}

}
