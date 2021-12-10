#include <core/entity.h>
#include <core/component.h>

namespace emp
{
	Entity::Entity()
	{
	}
	Entity::Entity(int id)
	{
		this->id = id;
		this->name = "Spore_" + std::to_string(id);
		//components = std::vector<Component>();
	}

	Entity::Entity(int id, std::string name)
	{
		this->id = id;
		this->name = name;
		//components = std::vector<Component>();
	}

	Entity Entity::operator=(Entity entity)
	{
		this->id = entity.id;
		this->name = entity.name;
		this->parent = entity.parent;
		return *this;
	}

	void Entity::SetName(char name[])
	{
		this->name = name;
	}

	void Entity::SetName(std::string name)
	{
		this->name = name;
	}

	void Entity::SetParent(int parent)
	{
		this->parent = parent;
	}

	void EntityManager::MoveEntity(int from_to, int move_to)
	{
		//Get from_to
		std::vector<Entity>* buffer = new std::vector<Entity>();
		Entity entity = (*entities)[from_to];
		
		entities->erase(entities->begin() + from_to);
		
		for (int i = 0; i < entities->size(); ++i)
		{
				buffer->push_back((*entities)[i]);
		}

		if(from_to < move_to)
		{
			buffer->insert((*buffer).begin() + move_to, entity);
		}
		if (from_to > move_to)
		{
			buffer->insert((*buffer).begin() + move_to, entity);
		}
		

		
	   //std::move(entities->end() - 4, entities->end(), new_list->begin() + 1);
		
	    /*
	 	if(move_to == entities->size())
		{
			entities->insert(entities->begin() + move_to, (*this->entities)[from_to]);
		}
		else
		{
			entities->insert(entities->begin() + move_to, (*this->entities)[from_to]);
		}
		
		
		if(from_to - move_to > 0)
			entities->erase(entities->begin() + from_to + 1);
		if (from_to - move_to < 0)
			entities->erase(entities->begin() + from_to);
		*/

		
		
		

		//int dir = 
		// std :: move function
		// move first 4 element from vec1 to starting position of vec2

		//std::move(entities->end()-4, entities->end(), new_list->begin() + 1);
		entities = std::move(buffer);
	}

	EntityManager::EntityManager(): System()
	{
	}

	EntityManager::EntityManager(Engine& engine, string name) : System(engine, name)
	{
	}

	Entity& EntityManager::CreateEntity()
	{
		entities->push_back(Entity(entities->size() + 1));
		return (*entities)[entities->size() - 1];
	}

	void EntityManager::RemoveEntity(int id)
	{
		this->entities->erase(this->entities->begin() + id);
	}

	void EntityManager::Init()
	{
		this->entities = new std::vector<Entity>();
		for (int i = 10 - 1; i >= 0; --i)
		{
			CreateEntity();
		}
		MoveEntity( entities->size() - 1, 0);
		MoveEntity( 0, 5);
	}

	void EntityManager::Update(float)
	{
		  
	}

	void EntityManager::Destroy()
	{
	}

	std::vector<Entity>* EntityManager::GetEntities() {
		return this->entities;
	}
}
