#include <core/config.h>

#include "GLFW/glfw3.h"
//#include <graphic/graphic.h>
using namespace std;

namespace emp
{
	const int SPACE_BORDER = 40;
	Configuration::Configuration(string name)
	{
		this->name = name;
	}

	string Configuration::GetName()
	{
		return this->name;
	}

	ConfigGraphic::ConfigGraphic(string name) : Configuration(name)
	{
	}

	ConfigGraphic::ConfigGraphic(string name, Mode& mode) : Configuration(name)
		{
			int horizontal, vertical;
			GetDesktopWindowResolution(horizontal, vertical);
		switch (mode)
		{
		case emp::Empty:
			
			break;
		case emp::Launcher:
			this->horizontal = horizontal;
			this->vertical = vertical;
			window_width = 640;
			window_height = 640;
			x = horizontal / 2 - window_width / 2;
			y = vertical / 2 - window_height / 2 - SPACE_BORDER;
			decorated = false;
			transparent = true;
			break;
		case emp::Editor:
		
			this->horizontal = horizontal;
			this->vertical = vertical;
			window_height = this->vertical - SPACE_BORDER - 30;
			window_width = this->horizontal;
			decorated = true;
			transparent = false;
			x = 0;
			y = 30;
			break;
		case emp::Game:
			break;
		}

	}

	ConfigEngine::ConfigEngine(string name) : Configuration(name)
	{
		mode = Empty;
	}

	ConfigEngine::ConfigEngine(string name, Mode& mode) : Configuration(name)
	{
		switch (mode)
		{
		case emp::Empty:
			editor = false;
			break;
		case emp::Launcher:
			editor = false;
			break;
		case emp::Editor:
			editor = true;
			break;
		case emp::Game:
			break;
		}
		this->mode = mode;
	}
}
