//enum ComponentType
//{
//	Transform = 0,
//	Sprite2D = 1
//};
#pragma once
#include <unordered_map>
#include <memory>
#include <core/system.h>

namespace emp {
	class Component{
	public:
		Component() = default;
		void virtual Inspect() = 0;
	};

	class IComponent
	{
	public:
		IComponent() = default;
	};

	template<typename T>
	class ComponentArray: public IComponent
	{
	public:
		ComponentArray()
		{
			components = std::vector<T>();
			mSize = 0;
		}
		
		void InsertData(int entity, T component)
		{
			// Put new entry at end and update the maps
			size_t newIndex = mSize;
			mEntityToIndexMap[entity] = newIndex;
			mIndexToEntityMap[newIndex] = entity;
			components.push_back(component);
			++mSize;
		}

		T& GetComponent(int entity)
		{
			return components[mEntityToIndexMap[entity]];
		}

		std::vector<T>& GetComponents()
		{
			return components;
		}
		

	protected:
		std::vector<T> components;
		// Map from an entity ID to an array index.
		std::unordered_map<int, size_t> mEntityToIndexMap;

		// Map from an array index to an entity ID.
		std::unordered_map<size_t, int> mIndexToEntityMap;

		// Total size of valid entries in the array.
		size_t mSize;
	};
	
	class ComponentManager : public System
	{
	public:

		void Init() override
		{
			
		}

		void Update(float) override
		{
			
		}

		void Destroy() override
		{
			
		}

		void Start() override{}

		template<typename T>
		void RegisterComponent()
		{
			const char* typeName = typeid(T).name();
			mComponentArrays.insert({ typeName, std::make_shared<ComponentArray<T>>() });
		}
		
		template<typename T>
		void AddComponent(int entity,  T component)
		{
			component.Init();
			GetComponentArray<T>()->InsertData(entity, component);
			
		}

		template<typename T>
		std::vector<T>& GetComponents()
		{
			return GetComponentArray<T>()->GetComponents();
		}

		template<typename T>
		T& GetComponent(int entity)
		{
			return GetComponentArray<T>()->GetComponent(entity);
		}

		//template<typename T>
		//T* GetComponent(int entity)
		//{
		//	return GetComponentArray<T>()->GetComponent(entity)
		//}
		
		
	private:
		std::unordered_map<const char*, std::shared_ptr<IComponent>> mComponentArrays{};
		// Convenience function to get the statically casted pointer to the ComponentArray of type T.
		template<typename T>
		std::shared_ptr<ComponentArray<T>> GetComponentArray()
		{
			const char* typeName = typeid(T).name();

			return std::static_pointer_cast<ComponentArray<T>>(mComponentArrays[typeName]);
		}
	};
	
}