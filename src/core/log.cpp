#include <core/log.h>
#include <iostream>
#include "core/engine.h"

namespace emp
{
	LogManager::LogManager() : System() { instance_log = this; }

	LogManager::LogManager(Engine& engine, string name) : System(engine, name) { instance_log = this; }

	void LogManager::Init() { instance_log = this; }

	void LogManager::Start() {}

	void LogManager::Update(float dt) {}

	void LogManager::Destroy() {}

	LogManager* LogManager::GetInstance()
	{
		if (instance_log == nullptr)
		{
			instance_log = new LogManager();
		}
		return instance_log;
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
