//class FileSystem(TSystem) :
//    # QueueData
//    _queue_data = []
//    _counter_data = 0
//
//    def __init__(self, name) :
//    self._name = name
//
//    # Analyse l'arborescence du moteur (fichiers)
//    # D�fini les types des fichiers
//
//    return
//
//    def AddFile(self, data) :
//    self._counter_data = self._counter_data + 1
//    self._queue_data.append([data[1], data[0], data[2], data[3]])
//    return
//
//    def __update__(self) :
//    while self._counter_data > 0:
//data = self._queue_data.pop()
//if data[2] == 'a' or data[2] == 'w' :
//    self.WriteFile(data)
//    self._counter_data = self._counter_data - 1
//
//    def WriteFile(self, data) :
//    counter = 0
//    filename = str(data[0])
//    f = open(filename, data[2])
//    s = ""
//    type = data[3]
//    if type == "txt" or type == "data":
//for elem in data[1] :
//    counter = counter + 1
//    s = s + " " + str(elem) + "\n"
//    f.write(s)
//    s = ""
//    if type == "json" :
//        f.write(data[1])
//        print(filename + ' saved; number of line saved: ' + str(counter))
//        print(" Data size writen: " + str(os.path.getsize(filename) / 100) + " Ko")
//        f.closed
//
//        def ReadFile(self, filename) :
//        f = open(filename, 'r')
//        lines = f.readlines()
//        f.close()
//        return lines
//
//        def __destroy__(self) :
//        _queue_data = []
//        _counter_data = 0S
//        return
//
#include <core/system.h>
#include <queue>
#include "json.hpp"

namespace emp
{
	using json = nlohmann::json;
        
	enum FormatFile
	{
		TXT = 0,
		JSON = 1
	};

	struct Data
	{
	public:
		Data() = default;
		Data(string name, int action)
		{
			this->name = name;
			this->action = action;	
		}
		string name; //data.txt
		int action; //read, write, append
	};

	struct Data2V
	{
	public:
		Data2V() = default;
		Data2V(string name, int action)
		{
			this->name = name;
			this->action = action;

		}
		string name; //data.txt
		int action; //read, write, append
	};

	class FileManager;
	static FileManager* instance_file;
	class FileManager : public System
	{
	public:
		FileManager(Engine&, string);
		void Init() override;
		void Start() {}
		void Update(float) override;
		void Destroy() override;
		static FileManager* GetInstance();

		Data ReadFile(string, FormatFile);
		string ReadShader(string path);
		string ReadFile(string path);
		void WriteFile(Data, FormatFile);
		void AppendFile(Data, FormatFile);
		void RemoveFile(string);
	private:
		// QueueData
		queue<Data> queue_Data;
		int counter_Data = 0;
			
	};

	namespace FileSystem
	{
		static std::string ReadFile(std::string path)
		{
			return FileManager::GetInstance()->ReadFile(path);
		}

		static std::string ReadShader(std::string path) {
			return FileManager::GetInstance()->ReadShader(path);
		}
	}
}


