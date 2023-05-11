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

        const int field_of_view = 2;
    public:
        Chunck(int entity)
        {
            this->entity = entity;
        }

        Chunck(int entity, int x, int y);
        Chunck(int entity, int x, int y, int seed);
        ~Chunck();

        std::vector<glm::vec3> LoadChunck(int x, int y);
        //std::queue<glm::vec3> LoadChunck(glm::vec3 pos, glm::vec2 parent);
        std::vector<glm::vec3> LoadChunck(int seed, int x, int y);
       
        void Init();
        std::vector<glm::vec3> LoadChunk();
        void Draw(ComponentManager* m_component);
        void Destroy();

        bool loaded = false;
        const unsigned int amount = 4096;
        int entity;

        Shader* shader = nullptr;
        glm::mat4* transformMatrices;
        // first. We get the relevant block indices
        unsigned int uniformBlockIndex;
        // Now actually create the buffer
        unsigned int uboMatrices;
        
        glm::vec2 position;
        int distance;
        int number;
        int seed = 0;

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

        //std::queue<glm::vec3>* chunk_to_load = new  std::queue<glm::vec3>();
    private:
       
        ComponentManager* m_component = nullptr;
        ConfigGraphic* config = nullptr;

      
        float project = 45.0f;
        float time = 0.0f;
        bool update = true;
        glm::vec3 offset;
        int seed;
        bool refresh = false;
        std::vector<Chunck*> chunks;
      
    };
}