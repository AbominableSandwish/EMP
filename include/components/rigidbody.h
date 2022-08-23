#include "core/system.h"

namespace emp {
    class ComponentManager;
	class RigidBody2DManager : public System {
	public:
		RigidBody2DManager(Engine& engine, string name);
       
        void Init();

        void Destroy() override;

        void Update(float) override;

    private:
        ComponentManager* m_component = nullptr;
	};

    class RigidBody2D {
    public:
        RigidBody2D(int entity);

        void Init();

        int entity;
        Vector2* direction;
    };
}