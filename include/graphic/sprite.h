#include "core/system.h"
#include "graphic/texture.h"

namespace emp
{
	class Engine;
	class ConfigGraphic;
	class ComponentManager;

	class SpriteRenderer
    {
    public:
        SpriteRenderer(int entity)
        {
            this->entity = entity;

            this->vs = "#version 330 core\n"
                "layout(location = 0) in vec3 aPos;\n"
                "layout(location = 1) in vec3 aColor;\n"
                "layout(location = 2) in vec2 aTexCoord;\n"
                "layout(location = 3) in vec2 aresolution;\n"

                "out vec3 ourColor;\n"
                "out vec2 TexCoord;\n"
                "uniform mat4 transform;"

                "void main()\n"
                "{\n"

                "gl_Position = transform * vec4(aPos, 1.0);\n"
                "ourColor = aColor;\n"
                "TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n"
                "}\0";

            this->frag = "#version 330 core\n"
                "out vec4 FragColor;\n"

                "in vec3 ourColor;\n"
                "in vec2 TexCoord;\n"

                // texture sampler
                "uniform sampler2D texture1;\n"

                "void main()\n"
                "{\n"

                "FragColor = texture2D(texture1, TexCoord);\n"
                "}\0";

            // set up vertex data (and buffer(s)) and configure vertex attributes
           // ------------------------------------------------------------------
            float vertices[] = {
                // positions            // colors           // texture coords
                1.0f,  1.0f, 0.0f,      1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
                1.0f, -1.0f, 0.0f,       0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
                -1.0f, -1.0, 0.0f,      0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
                -1.0f,  1.0f, 0.0f,     1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
            };

            unsigned int indices[] = {
                0, 1, 3, // first triangle
                1, 2, 3  // second triangle
            };

            texture = 0;
        }

        SpriteRenderer(int entity, std::string path);

        void Init();
        int entity;
        std::string path;

        unsigned int shaderProgram;
        unsigned int VBO, VAO, EBO;
        unsigned int texture;

        const char* vs;
        const char* frag;
    };

    class SpriteManager : public System
    {
    public:
        SpriteManager(Engine& engine, ConfigGraphic& config);

        void Init() override;
        void Update(float) override;
        void Draw();
        void Destroy() override;

    private:
        ComponentManager* m_component;
        ConfigGraphic* config = nullptr;
    };
}
