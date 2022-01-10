//enum ComponentType
//{
//	Transform = 0,
//	Sprite2D = 1
//};
namespace emp {
	class Transform
	{
	public:
		float x, y;
		float scale_x, scale_y;
		
		Transform()
		{
			this->x = 0.0f;
			this->y = 0.0f;
			this->scale_x = 1;
			this->scale_y = 1;
		}

		Transform(float x, float y)
		{
			this->x = x;
			this->y = y;
			this->scale_x = 1;
			this->scale_y = 1;
		}

		void SetPosition(float x, float y)
		{
			this->x = x;
			this->y = y;
		}

		void SetScale(float x, float y)
		{
			this->scale_x = x;
			this->scale_y = y;
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
		}
		
		void InsertData(int entity, T component)
		{
			components.push_back(component);
		}

		T& GetComponent(int entity)
		{
			//todo 
			return components[0];
		}

		std::vector<T>& GetComponents()
		{
			return components;
		}
		

	protected:
		std::vector<T> components;
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


//
//class Sprite2D : Component
//{
//
//};