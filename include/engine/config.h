#include <string>
using namespace std;

namespace emp
{	
    class Configuration {
    public:
		Configuration(string name);
		string GetName();
    protected:
        std::string name;

    
      

    };

	enum Mode
	{
		Console,
		Launcher,
		Editor,
		Game
	};

	class ConfigEngine : Configuration
	{
	public:
		ConfigEngine(string name);
		ConfigEngine(string name, Mode& mode);
		
		int GetWidth()
		{
			return this->width;
		}
		Mode mode;
	protected:
		
		int width = 1024;
		int height = 800;
	};
	
}

//(templateSystem, fileSystem, logSystem)
//self._templateSystem = templateSystem
//self._fileSystem = fileSystem
//self._logSystem = logSystem
//def get_mode(self) :
//return self._mode