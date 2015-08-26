#include "Logger.h"

namespace core
{
	void FileLogPolicy::openStream(const string & name)
	{
		outStream->open(name.c_str(), ios_base::binary | ios_base::out);
		if (!outStream->is_open())
			throw(runtime_error("Logger is unable to open an output stream"));
	}
	void FileLogPolicy::closeStream()
	{
		if (outStream)
			outStream->close();
	}
	void FileLogPolicy::write(const string& message)
	{
		(*outStream) << message << endl;
	}
	FileLogPolicy::~FileLogPolicy()
	{
		if (outStream)
			closeStream();
	}
}