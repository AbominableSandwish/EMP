#include "core/system.h"
#include "graphic/texture.h"

namespace emp
{
	class Engine;
	class ConfigGraphic;
	class ComponentManager;

    class Shader;

	class SpriteRenderer
    {
    public:
        SpriteRenderer(int entity)
        {
            this->entity = entity;


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

        Shader* shader = nullptr;
    };
}
