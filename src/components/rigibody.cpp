#include <components/rigidbody.h>
#include <components/transform.h>
#include <core/component.h>
#include <core/engine.h>
#include <math/vector.h>

namespace emp {
	RigidBody2DManager::RigidBody2DManager(Engine& engine, string name) : System(engine, name) {
	}

	void RigidBody2DManager::Init() {
		
		
	}

	
	float velocity = 1.0f;

	void RigidBody2DManager::Update(float dt) {
		auto& arrayElement = engine->GetComponentManager()->GetComponents<RigidBody2D>();
		for (auto& element : arrayElement)
		{
			auto& transform = engine->GetComponentManager()->GetComponent<Transform>(element.entity);
			Vector2 position = transform.GetPosition();
			Vector2 scale = transform.GetScale();
			Vector2 move = Vector2(0.5f * velocity, 0.0f);
			if (position.x >= 500)
				element.direction = new Vector2(-1.0f, 1.0f);
			if (position.x <= -500)
				element.direction =new Vector2(1.0f, 1.0f);
			transform.SetPosition(position + move);
		}
	}


	void RigidBody2DManager::Destroy() {
	}



	RigidBody2D::RigidBody2D(int entity)
	{
		this->entity = entity;
		direction = new Vector2(1.0f, 0.0f);
	}

	void RigidBody2D::Init() {

	}
}