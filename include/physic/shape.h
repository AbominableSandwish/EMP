
namespace emp {
	class Transform;
	class Shape {

	};

	class Square : public Shape {

	};

	class Circle : public Shape {

	};

	class Triangle : public Shape {

	};

	class Collider2D
	{
		Shape type;
		int entity;
		Transform* transform = nullptr;
	};

	class ColliderManager:  {


		
	};

	
}