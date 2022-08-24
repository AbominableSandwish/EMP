
namespace emp {
	class Matrice3;
	class Vector2;
	class Transform
	{
	public:
		float scale_x, scale_y;
		Matrice3* matrice;

		Transform();

		Transform(float x, float y);

		Transform(float x, float y, float w, float l);

		void Init();

		Vector2 GetPosition();

		void SetPosition(Vector2 position);
		void SetPosition(float x, float y);

		Vector2 GetRotation();
		void SetRotation(float axis_x, float axis_y);

		Vector2 GetScale();

		void SetScale(float w, float l);
	};
}