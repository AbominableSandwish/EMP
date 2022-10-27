
namespace emp {
	class Matrice4;
	class Vector3;
	class Vector4;
	class Transform
	{
	public:
		float angle_x, angle_y, angle_z;
		float scale_x, scale_y = 1.0f;
		Matrice4* matrice;
		Vector4* position;

		Transform();
		Transform(float x, float y, float z);
		Transform(float x, float y, float z, float w, float l);
		Transform(float x, float y, float z, float angle_x, float angle_y, float angle_z, float w, float l, float h);

		void Init();
		void Reset();
		Vector3 GetPosition();
		Vector4 GetPositionPosition();

		void SetPosition(Vector3 position);
		void SetPosition(float x, float y);

		Vector3 GetRotation();
		void SetRotation(float angle, Vector3 axis);

		Vector3 GetScale();

		void SetScale(float w, float l);
		void SetScale(float w, float h, float p);

		Matrice4 TranslationMatrix(Vector3 position);
		Matrice4 RotationMatrixFrom(const float angle, Vector3 axis);
		Matrice4 ScalingMatrix(Vector3 scale);
	};
}