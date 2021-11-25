#include "core/system.h"
#include "core/engine.h"
#include "core/log.h"

namespace emp
{
	System::System(Engine& engine, string name)
	{
		this->name = name;
		this->engine = &engine;
		LOG::Info(name +" initialized");
	}
}
