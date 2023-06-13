#include <core/entity.h>
#include <core/log.h>
#include <core/component.h>

namespace emp
{
	Entity::Entity()
	{
		this->name = "";
		this->id = -1;
		this->parent = -1;
		components = std::vector<string>();
	}

	Entity::~Entity()
	{
		components = std::vector<string>();
	}
	
	Entity::Entity(int id)
	{
		this->id = id;
		this->name = "Spore_" + std::to_string(id);
		components = std::vector<string>();
	}

	Entity::Entity(int id, std::string name)
	{
		this->id = id;
		this->name = name;
		components = std::vector<string>();
	}

	/*void Entity::operator=(Entity entity)
	{
		this->id = entity.id;
		this->name = entity.name;
		this->parent = entity.parent;
	}*/

	void Entity::SetName(char name[])
	{
		this->name = name;
	}

	void Entity::SetName(std::string name)
	{
		this->name = name;
	}

	std::string Entity::GetName()
	{
		return this->name;
	}

	void Entity::SetParent(int parent)
	{
		this->parent = parent;
	}

	void Entity::AddComponent(string type)
	{

	}

	vector<string> Entity::GetComponents()
	{
		return components;
	}

	void EntityManager::MoveEntity(int from_to, int move_to)
	{
		//Get from_to
		std::vector<Entity*> buffer = std::vector<Entity*>();
		Entity* entity = entities[from_to];
		
		entities.erase(entities.begin() + from_to);
		
		for (int i = 0; i < entities.size(); ++i)
		{
				buffer.push_back(entities[i]);
		}

		if(from_to < move_to)
		{
			buffer.insert(buffer.begin() + move_to, entity);
		}
		if (from_to > move_to)
		{
			buffer.insert(buffer.begin() + move_to, entity);
		}

		entities = buffer;
	}

	EntityManager::EntityManager(): System()
	{
	}

	EntityManager::EntityManager(Engine& engine, string name) : System(engine, name)
	{
	}

	Entity& EntityManager::CreateEntity()
	{
		entities.push_back(new Entity(entities.size() + 1));
		LOG::Info("New Entity: \"" + entities.back()->GetName() + "\"");
		return *entities[entities.size() - 1];	
	}
	
	Entity& EntityManager::CreateEntity(std::string name)
	{
		entities.push_back(new Entity(entities.size() + 1, name));
		LOG::Info("New Entity: \"" + entities.back()->GetName() + "\"");
		return *entities[entities.size() - 1];
	}

	void EntityManager::RemoveEntity(int id)
	{
		LOG::Info("Entity Removed: \"" + this->entities[id]->GetName() + "\"");
		this->entities.erase(this->entities.begin() + id);
	}

	void EntityManager::Init()
	{
		this->entities = std::vector<Entity*>();
	}

	void EntityManager::Update(float)
	{
		  
	}

	void EntityManager::Destroy()
	{
	}

	vector<string> EntityManager::GetEntityComponents(int entity)
	{
		return this->entities[entity]->GetComponents();
	}

	Entity* EntityManager::GetEntity(int id)
	{
		for (Entity* element : entities)
		{
			if (element->id == id)
				return element;
		}
	}

	std::vector<Entity*> EntityManager::GetEntities()
	{
		return entities;
	}
}
