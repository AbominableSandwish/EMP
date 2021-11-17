#include "core/system.h"
#include "core/engine.h"

namespace emp
{
	System::System(Engine& engine, string name)
	{
		this->name = name;
		this->engine = &engine;
	}
}
