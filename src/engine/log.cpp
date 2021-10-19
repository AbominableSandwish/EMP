#include <engine/log.h>
#include <iostream>

namespace emp
{
	void LogManager::Init()
	{
		AddLog(Log((int)this->m_logs.size(), LogType::DEBUG, "Start"));
	}

	void LogManager::Update(float dt)
	{
		LogType type;
		switch ((rand() % 5)+1)
		{
		case 0:
			type = LogType::NONE;
			break;
		case 1:
			type = LogType::DEBUG;
			break;
		case 2:
			type = LogType::INFO;
			break;
		case 3:
			type = LogType::WARNING;
			break;
		case 4:
			type = LogType::ERROR;
			break;
		case 5:
			type = LogType::CRITICAL;
			break;
		}
		AddLog(Log((int)this->m_logs.size(), type, std::to_string(dt)));
	}

	void LogManager::Destroy()
	{
		
	}

	void LogManager::AddLog(Log log)
	{
		string type;
		switch (log.type)
		{
		case LogType::NONE:
			type = "[ NONE ]";
			break;
		case LogType::DEBUG:
			type = "[ DEBUG ]";
			break;
		case LogType::INFO:
			type = "[ INFO ]";
			break;
		case LogType::WARNING:
			type = "[ WARNING ]";
			break;
		case LogType::ERROR:
			type = "[ ERROR ]";
			break;
		case LogType::CRITICAL:
			type = "[ CRITICAL ]";
			break;
		}
		std::cout << "[ LOG ]" <<  type << " " << log.msg << std::endl;
		this->m_logs.push_back(log);
	}

	std::list<Log> LogManager::GetLogs()
	{
		return this->m_logs;
	}

	LogManager::LogManager(Engine& engine, string name) : System(engine, name)
	{
		
	}
}
