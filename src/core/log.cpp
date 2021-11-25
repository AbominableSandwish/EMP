#include <core/log.h>
#include <iostream>
#include "core/engine.h"

namespace emp
{
	LogManager::LogManager() : System() {}

	LogManager::LogManager(Engine& engine, string name) : System(engine, name) { instance = this; }

	void LogManager::Init() { instance = this; }

	void LogManager::Update(float dt) {}

	void LogManager::Destroy() {}

	LogManager* LogManager::GetInstance()
	{
		if (instance == nullptr)
		{
			instance = new LogManager();
		}
		return instance;
	}

	void LogManager::AddLog(Log log)
	{
		string type;
		const char* color = "\033[39m";
		switch (log.type)
		{
		case LogType::NONE:
			type = "   [ NONE ]";
			break;
		case LogType::DEBUG:
			type = "  [ DEBUG    ]";
			color = "\033[36m";
			break;
		case LogType::INFO:
			type = "  [ INFO     ]";
			break;
		case LogType::WARNING:
			type = "  [ WARNING  ]";
			color = "\033[33m";
			break;
		case LogType::ERROOR:
			type = "  [ ERROR    ]";
			color = "\033[31m";
			break;
		case LogType::CRITICAL:
			type = "  [ CRITICAL ]";
			color = "\033[35m";
			break;
		}
		std::cout << "[ LOG ]" << color << type << "\033[39m"  <<"\t" << log.msg << std::endl;
		this->m_logs.push_back(log);
	}

	std::vector<Log>& LogManager::GetLogs()
	{
		return this->m_logs;
	}

	void LogManager::ClearLogs()
	{
		this->m_logs.clear();
	}
}
