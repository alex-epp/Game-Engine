#pragma once

#include <algorithm>
#include <string>
#include <vector>

using namespace std;

namespace renderSystem
{
	string getPathName(const string& s)
	{
		size_t i = s.rfind('/', s.length());

		if (i != string::npos)
			return(s.substr(0, i));

		return("");
	}

	
}