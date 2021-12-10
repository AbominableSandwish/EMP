#include <string>
#include <vector>

#include "system.h"
#include "glm/gtx/range.hpp"

namespace emp
{
	class Component;
	struct Entity
	{
	public:
		int id;
		std::string name;

		Entity();
		Entity(int);
		Entity(int, std::string);
		Entity operator= (Entity entity);

		void SetName(char[]);
		void SetName(std::string);
		void SetParent(int);

		//void AddComponent(Component);
		//void RemoveComponent(Component);

	private:
		//std::vector<Component> components;
		int parent;
	};

	class EntityManager : public System
	{
	public:
		EntityManager();
		EntityManager(Engine&, string);
		Entity& CreateEntity();
		void RemoveEntity(int);
		void MoveEntity(int, int);

		void Init() override;
		void Update(float) override;
		void Destroy() override;

		
		
		Entity& GetEntity();
		std::vector<Entity>* GetEntities();
		
		
	private:
		std::vector<Entity>* entities;
	};

        
}
