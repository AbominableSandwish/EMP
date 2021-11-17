#include <string>
#include <list>
#include <core/system.h>


namespace  emp
{
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
		Log(int id, LogType type, std::string msg)
		{
			this->id = id;
			this->type = type;
			this->msg = msg;		
		}
		
		int id;
		LogType type;
		std::string msg;
	};
	
	class LogManager : public System
	{
	public:
		LogManager(Engine&, string);
		void Init();
		void Update(float);
		void Destroy();
		
		void AddLog(Log log);
		std::list<Log> GetLogs();
	protected:
		std::list<Log> m_logs;
	};
}
