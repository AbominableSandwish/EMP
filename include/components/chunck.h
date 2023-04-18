#include "core/system.h"
#include "glm/gtx/transform.hpp"

// number of elements in one row
#define COLS 64
// number of rows
#define ROWS 64

namespace emp {
    class Engine;
    class ConfigGraphic;
    class Shader;
    class Transform;




    class ComponentManager;
    class Chunck {


    public:
        Chunck(int entity)
        {
            this->entity = entity;
        }

        Chunck(int entity, int x, int y);

        std::vector<glm::vec3> LoadChunck(int x, int y);

        void Init();

        void Draw(ComponentManager* m_component);

        const unsigned int amount = 2048;
        int entity;

        Shader* shader = nullptr;
        glm::mat4* transformMatrices;
        // first. We get the relevant block indices
        unsigned int uniformBlockIndex;
        // Now actually create the buffer
        unsigned int uboMatrices;
        unsigned int diffuse_map;
        unsigned int specular_map;

        glm::vec2 position;

    };


  


    class ChunckManager : public System
    {
    public:
        ChunckManager(Engine& engine, ConfigGraphic& config);

        void Init();

        void Start();

        void Refresh();

        void Destroy() override;

        void Update(float) override;

        void Draw();
    private:
       
        ComponentManager* m_component = nullptr;
        ConfigGraphic* config = nullptr;

        std::vector<Chunck> chuncks;
        float project = 45.0f;

        float time = 0.0f;
        bool update = true;
        glm::vec3 offset;
        int seed;


    };
}