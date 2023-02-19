#include "core/system.h"

namespace emp {
    class Vector2;
    class ComponentManager;
    class Transform;
	class RigidBody2DManager : public System {
	public:
		RigidBody2DManager(Engine& engine, string name);
       
        void Init();
        void Start() {}

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
        Vector2* direction = nullptr;
        Transform* transform = nullptr;
        Vector2* position = nullptr;
        float velocity;
    };
}