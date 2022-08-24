
namespace emp {
	class Matrice3;
	class Vector3;
	class Transform
	{
	public:
		float angle_x;
		float scale_x, scale_y;
		Matrice3* matrice;

		Transform();

		Transform(float x, float y);

		Transform(float x, float y, float w, float l);

		void Init();
		
		Vector3 GetPosition();

		void SetPosition(Vector3 position);
		void SetPosition(float x, float y);

		Vector3 GetRotation();
		void SetRotation(float angle, Vector3 axis);

		Vector3 GetScale();

		void SetScale(float w, float l);
	};
}