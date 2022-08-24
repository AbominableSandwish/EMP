#include <components/rigidbody.h>
#include <components/transform.h>
#include <core/component.h>
#include <core/engine.h>
#include <math/matrice.h>

namespace emp {
	RigidBody2DManager::RigidBody2DManager(Engine& engine, string name) : System(engine, name) {
		
	}

	void RigidBody2DManager::Init() {
		this->m_component = engine->GetComponentManager();
		//Alloc Memory 
		int size = 4096;
		auto ptr = reinterpret_cast<RigidBody2D*>(std::malloc(size * sizeof(RigidBody2D)));
	}

	
	

	void RigidBody2DManager::Update(float dt) {
		auto& arrayElement = this->m_component->GetComponents<RigidBody2D>();
		for (auto& element : arrayElement)
		{
			if (element.transform == nullptr) {
				
				element.transform = &this->m_component->GetComponent<Transform>(element.entity);
			}
			Vector3 position = element.transform->GetPosition();
			Vector3 move = Vector3(element.direction->x * 0.5f * element.velocity, 0.0f, 0.0f);
			if (position.x >= 100)
				element.direction = new Vector2(-1.0f, 1.0f);
			if (position.x <= -100)
				element.direction =new Vector2(1.0f, 1.0f);
			element.transform->SetPosition(position + move);
		}
	}


	void RigidBody2DManager::Destroy() {
		//std::free(ptr);
	}



	RigidBody2D::RigidBody2D(int entity)
	{
		this->entity = entity;		
		direction = new Vector2(1.0f, 0.0f);
		velocity = 1 + (rand() % 10);
		
	}

	void RigidBody2D::Init() {

	}
}