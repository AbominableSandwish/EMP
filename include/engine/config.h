#include <string>
#include <windows.h>
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

	class ConfigGraphic : Configuration
	{
	public:
		ConfigGraphic(string name);
		ConfigGraphic(string name, Mode& mode);

		int GetWidth()
		{
			return this->width;
		}

		// Get the horizontal and vertical screen sizes in pixel
		void GetDesktopWindowResolution(int& horizontal, int& vertical)
		{
			RECT desktop;
			// Get a handle to the desktop window
			const HWND hDesktop = GetDesktopWindow();
			// Get the size of screen to the variable desktop
			GetWindowRect(hDesktop, &desktop);
			// The top left corner will have coordinates (0,0)
			// and the bottom right corner will have coordinates
			// (horizontal, vertical)
			horizontal = desktop.right;
			vertical = desktop.bottom;
		}

		//Resolution texture
		int width, height;
		//Size Window;
		int window_width, window_height;
		//position Window
		int x, y;
		//other config window
		bool decorated;
		bool transparent;
	};
	

	class ConfigEngine : Configuration
	{
	public:
		ConfigEngine(string name);
		ConfigEngine(string name, Mode& mode);
		Mode mode;

		bool editor;
	};
	
}

//(templateSystem, fileSystem, logSystem)
//self._templateSystem = templateSystem
//self._fileSystem = fileSystem
//self._logSystem = logSystem
//def get_mode(self) :
//return self._mode