//enum ComponentType
//{
//	Transform = 0,
//	Sprite2D = 1
//};
namespace emp {
	class Component
	{
	public:
		Component() = default;
		Component(string name)
		{
			this->name = name;
		}
		string name;
	};

	class Transform : public Component
	{
	public:
		float x, y;
		
		Transform() : Component("Transform")
		{
			this->x = 0.0f;
			this->y = 0.0f;
		}

		Transform(float x, float y) : Component("Transform")
		{
			this->x = x;
			this->y = y;
		}

		void SetPosition(float x, float y)
		{
			this->x = x;
			this->y = y;
		}
	};
}


//
//class Sprite2D : Component
//{
//
//};