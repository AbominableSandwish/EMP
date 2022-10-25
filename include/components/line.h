#include "glm/gtx/transform.hpp"

namespace emp {
    class ConfigGraphic;
    class Line {
        int shaderProgram;
        unsigned int VBO, VAO;
       
        glm::vec3 startPoint;
        glm::vec3 endPoint;
        glm::mat4 MVP;
        glm::vec3 lineColor;
        ConfigGraphic* config = nullptr;

    public:
        Line(ConfigGraphic& config, glm::vec3 start, glm::vec3 end);

        int setColor(glm::vec3 color) {
            lineColor = color;
            return 1;
        }

        int Draw();

        ~Line();
    };

}