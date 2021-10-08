#include <engine/config.h>
using namespace std;

namespace emp
{
	Configuration::Configuration(string name)
	{
		this->name = name;
	}

	string Configuration::GetName()
	{
		return this->name;
	}

	ConfigEngine::ConfigEngine(string name) : Configuration(name)
	{
	}
}
