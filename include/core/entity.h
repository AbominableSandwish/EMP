#include <string>
#include <vector>
#include "system.h"

namespace emp
{
	class Component;
	struct Entity
	{
	public:
		int id = -1;
	
		Entity();
		Entity(int);
		Entity(int, std::string);
		~Entity();
		
		//void operator= (Entity entity);

		void SetName(char[]);
		void SetName(std::string);
		std::string GetName();
		void SetParent(int);
		int GetParent();
		
		void AddComponent(Component* c);
		void AddComponent(string type);
		vector<string> GetComponents();


		//void AddComponent(Component);
		//void RemoveComponent(Component);

	private:
		//std::vector<Component> components;
		std::string name = "";
		int parent = -1;
		std::vector<string> components;
	};

	class EntityManager : public System
	{
	public:
		EntityManager();
		EntityManager(Engine&, string);
		Entity& CreateEntity();
		Entity& CreateEntity(std::string);
		void RemoveEntity(int);
		void MoveEntity(int, int);

		void Init() override;
		void Start() {}
		void Update(float) override;
		void Destroy() override;
		vector<string> GetEntityComponents(int entity);
		
		Entity* GetEntity(int);
		std::vector<Entity*> GetEntities();
		int GetEntitesCount() {
			return this->entities.size();
		}
			
	private:
		std::vector<Entity*> entities;
	};

        
}
