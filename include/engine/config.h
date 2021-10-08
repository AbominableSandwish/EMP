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

	class ConfigEngine : Configuration
	{
	public:
		ConfigEngine(string name);
		
		int GetWidth()
		{
			return this->width;
		}
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