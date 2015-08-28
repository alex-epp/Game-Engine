// Based heavily off of http://www.drdobbs.com/cpp/a-lightweight-logger-for-c/240147505?pgno=1

#pragma once
#include <string>
#include <memory>
#include <mutex>
#include <fstream>
#include <iostream>
#include <sstream>
#include <chrono>
#include <iomanip>
using namespace std;

namespace core
{
	// Change this to change logging level
#define LOGGING_LEVEL_1

	class ILogPolicy
	{
	public:
		virtual void openStream(const string& name) = 0;
		virtual void closeStream() = 0;
		virtual void write(const string& message) = 0;
	};

	class FileLogPolicy : public ILogPolicy
	{
	public:
		FileLogPolicy() : outStream(new ofstream) {}
		~FileLogPolicy();

		void openStream(const string& name) override;
		void closeStream() override;
		void write(const string& message) override;

	private:
		unique_ptr<ofstream> outStream;
	};

	class ConsoleLogPolicy : public ILogPolicy
	{
	public:
		
		void openStream(const string& name) override {}
		void closeStream() override {}
		void write(const string& message) override;
	};

	enum class SeverityType { Debug = 1, Error, Warning }; // ERROR is defined already, so this has can't be uppercase

	template <typename LogPolicy>
	class Logger
	{
	public:
		explicit Logger(string name);
		~Logger();

		template<SeverityType severity, typename...Args>
		void print(Args...args);

	private:
		unsigned int lineNumber;
		string getTime();
		string getLoglineHeader();
		stringstream logStream;
		unique_ptr<LogPolicy> policy;
		std::mutex writeMutex;

		//template<typename...Args>
		void print_impl();

		template<typename First, typename...Args>
		void print_impl(First first, Args...args);
	};


	template<typename LogPolicy>
	inline Logger<LogPolicy>::Logger(string name)
	{
		lineNumber = 0;
		policy = make_unique<LogPolicy>();
		if (!policy)
			throw runtime_error("Logger: Unable to create logger instance");

		policy->openStream(name);
	}

	template<typename LogPolicy>
	inline Logger<LogPolicy>::~Logger()
	{
		if (policy)
		{
			policy->closeStream();
		}
	}

	template<typename LogPolicy>
	string Logger<LogPolicy>::getTime()
	{
		auto now = chrono::system_clock::now();
		auto now_c = chrono::system_clock::to_time_t(now);

		stringstream ss;
		ss << put_time(localtime(&now_c), "%c");
		return ss.str();
	}

	template<typename LogPolicy>
	inline string Logger<LogPolicy>::getLoglineHeader()
	{
		stringstream header;
		header.str("");
		header.fill('0');
		header.width(7);
		header << lineNumber++ << " < " << getTime() << " - ";
		header.fill('0');
		header.width(7);
		header << clock() << " > ~ ";
		return header.str();
	}

	template<typename LogPolicy>
	template<SeverityType severity, typename ...Args>
	void Logger<LogPolicy>::print(Args ...args)
	{
		writeMutex.lock();
		logStream.str("");
		switch (severity)
		{
		case SeverityType::Debug:
			logStream << "<DEBUG>: ";
			break;
		case SeverityType::Warning:
			logStream << "<WARNING>: ";
			break;
		case SeverityType::Error:
			logStream << "<ERROR>: ";
			break;
		}
		print_impl(args...);
		//writeMutex.unlock();
	}

	template<typename LogPolicy>
	//template<typename ...Args>
	void Logger<LogPolicy>::print_impl()
	{
		policy->write(getLoglineHeader() + logStream.str());
		logStream.str("");
	}

	template<typename LogPolicy>
	template<typename First, typename ...Args>
	void Logger<LogPolicy>::print_impl(First first, Args...args)
	{
		logStream << first;
		print_impl(args...);
	}

	static Logger<ConsoleLogPolicy> logInst("execution.log");

#ifdef LOGGING_LEVEL_1

#define LOG core::logInst.print<core::SeverityType::Debug>
#define LOG_ERR core::logInst.print<core::SeverityType::Error>
#define LOG_WARN core::logInst.print<core::SeverityType::Warning>

#else

#define LOG(...)
#define LOG_ERR(...)
#define LOG_WARN(...)

#endif

#ifdef LOGGING_LEVEL_2

# Error "I'm not sure if this works"

#define ELOG core::logInst.print< core::SeverityType::Debug >
#define ELOG_ERR core::logInst.print< core::SeverityType::Error >
#define ELOG_WARN core::logInst.print< core::SeverityType::Warning >

#else

#define ELOG(...) 
#define ELOG_ERR(...)
#define ELOG_WARN(...)

#endif

}