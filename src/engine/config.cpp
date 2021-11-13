#include <engine/config.h>

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
		case emp::Console:
			
			width = 192;
			height = 192;
			window_width = 192;
			window_height = 192;
			x = horizontal - width;
			y = vertical - height - SPACE_BORDER;
			decorated = false;
			transparent = true;
			break;
		case emp::Launcher:
			width = 192;
			height = 192;
			window_width = 640;
			window_height = 640;
			x = horizontal / 2 - width / 2;
			y = vertical / 2 - height / 2 - SPACE_BORDER;
			decorated = false;
			transparent = true;
			break;
		case emp::Editor:
		
			window_width = horizontal;
			window_height = vertical - SPACE_BORDER - 30;
			width = window_width;
			height = window_height;
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
		mode = Console;
	}

	ConfigEngine::ConfigEngine(string name, Mode& mode) : Configuration(name)
	{

		switch (mode)
		{
		case emp::Console:
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
