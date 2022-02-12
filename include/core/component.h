//enum ComponentType
//{
//	Transform = 0,
//	Sprite2D = 1
//};

#include <math/matrice.h>
namespace emp{
	class Component {
public:
	Component() = default;
};
	class Transform: public Component
	{
	public:
		float scale_x, scale_y;
		Math::Matrice3 matrice;

		Math::Vector2 GetPosition()
		{
			return matrice.GetPosition();
		}


		Math::Vector2 GetRotation()
		{
			return matrice.GetRotation();
		}

		void SetPosition(Math::Vector2 position)
		{
			matrice.SetScale(position);
		}

		Math::Vector2 GetScale()
		{
			return matrice.GetScale();
		}
		
		Transform()
		{
			matrice = Math::Matrice3();
			
			this->scale_x = 1;
			this->scale_y = 1;
		}

		Transform(float x, float y)
		{
			matrice = Math::Matrice3(x, y);
			this->scale_x = 1;
			this->scale_y = 1;
		}

		Transform(float x, float y, float w, float l)
		{
			matrice = Math::Matrice3(x, y, w, l);
			
			this->scale_x = w;
			this->scale_y = l;
		}

		void SetPosition(float x, float y)
		{
			this->matrice.SetPosition(Math::Vector2(x, y));
		}

		void SetRotation(float axis_x, float axis_y)
		{
			this->matrice.SetRotation(Math::Vector2(axis_x, axis_y));
		}

		

		void SetScale(float w, float l)
		{
			this->matrice.SetScale(Math::Vector2(w, l));

		}

		void Init()
		{
			
		}

		
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