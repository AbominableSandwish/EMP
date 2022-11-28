#include "core/system.h"
#include "glm/gtx/transform.hpp"
namespace emp {
	class Transform;
	class Camera {
	public:
		
		int entity;

		Transform& transform;
		glm::mat4 projection;
		float angle_proj;

		void SetProjection(float angle) {
			this->angle_proj = angle;
			this->projection = glm::mat4(1.0f);
			projection = glm::perspective(glm::radians(angle), (float)1000 / (float)1000, 0.1f, 100.0f);
		}

		Camera(int entity, Transform& transform);

		Camera(int entity, Transform& transform, float projection) : entity(entity), transform(transform) {
			SetProjection(projection);
		}

		glm::vec3 GetPosition();

		void Init() {

		}
	};
}