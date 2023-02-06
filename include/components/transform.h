#include <glm/glm.hpp>

namespace emp {
	class Matrice4;
	class Vector3;
	class Vector4;
	class Transform
	{
	public:
		float angle_x, angle_y, angle_z;
		float scale_x, scale_y, scale_z;
		Matrice4* matrice;
		Vector4* position; //Local Position
		//int parent = null;

		Transform();
		Transform(float x, float y, float z);
		Transform(float x, float y, float z, float w, float l);
		Transform(float x, float y, float z, float angle_x, float angle_y, float angle_z, float w, float l, float h);

		void Init();
		void Reset();
		Vector3 GetPosition();
		glm::vec3 Position();
		Vector4 GetPositionPosition();

		void SetPosition(glm::vec3);
		void SetPosition(Vector3 position);
		void SetPosition(float x, float y);

		Vector3 GetRotation();
		void SetRotation(float angle, Vector3 axis);

		Vector3 GetScale();

		void SetScale(float w, float l);
		void SetScale(float w, float h, float p);

		/*void SetParent(int entity_parent)
		{
			this->parent = entity_parent;
		}*/

		Matrice4 TranslationMatrix(Vector3 position);
		Matrice4 RotationMatrixFrom(const float angle, Vector3 axis);
		Matrice4 ScalingMatrix(Vector3 scale);
	};


	//class TransformManager : public System {
	//public:
	//	TransformManager(Engine& engine, string name);

	//	void Init();

	//	void Destroy() override;

	//	void Update(float) override;

	//private:
	//	ComponentManager* m_component = nullptr;
	//};
}