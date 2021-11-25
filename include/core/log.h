#include <string>
#include <vector>
#include <core/system.h>


namespace  emp
{
	class LogManager;


	enum class LogType
	{
		NONE = 0,
		DEBUG = 1,
		INFO = 2,
		WARNING = 3,
		ERROOR = 4,
		CRITICAL = 5
	};
	
	struct Log
	{
	public:
		Log(LogType type, std::string msg)
		{
			this->type = type;
			this->msg = msg;		
		}

		LogType type;
		string msg;
	};
	
	static LogManager* instance;
	class LogManager : public System
	{
	public:
		//Constructor
		LogManager();
		LogManager(Engine&, string);
		//Function
		void Init();
		void Update(float);
		void Destroy();
		static LogManager* GetInstance();
		
		void AddLog(Log log);
		void ClearLogs();
		std::vector<Log>& GetLogs();
	protected:
		std::vector<Log> m_logs;
	};

	namespace LOG
	{
		static void Critical(std::string msg)
		{
			LogManager::GetInstance()->AddLog(Log(emp::LogType::CRITICAL, msg));
		}
		static void Error(std::string msg)
		{
			LogManager::GetInstance()->AddLog(Log(emp::LogType::ERROOR, msg));
		}
		static void Warning(std::string msg)
		{
			LogManager::GetInstance()->AddLog(Log(emp::LogType::WARNING, msg));
		}
		static void Info(std::string msg)
		{
			LogManager::GetInstance()->AddLog(Log(emp::LogType::INFO, msg));
		}
		static void Debug(std::string msg)
		{
			LogManager::GetInstance()->AddLog(Log(emp::LogType::DEBUG, msg));
		}
	}
}
