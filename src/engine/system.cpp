#include "engine/system.h"
#include "engine/engine.h"

namespace emp
{
	System::System(Engine& engine, string name)
	{
		this->name = name;
		this->engine = &engine;
	}
}
