#include <core/component.h>
#include <math/matrice.h>
#include <core/engine.h>

namespace emp {
	void ComponentManager::Init(Engine& engine)
	{
		this->m_entity = engine.GetEntityManager();
	}
}
