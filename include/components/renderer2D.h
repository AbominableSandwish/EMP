#include "core/system.h"
#include "glm/gtx/transform.hpp"


namespace emp {
	class Renderer2D : public System
	{
	public:
        Renderer2D(Engine& engine, string name);
	};
	
    class Line {
        int shaderProgram;
        unsigned int VBO, VAO;
        std::vector<float> vertices;
        glm::vec3 startPoint;
        glm::vec3 endPoint;
        glm::mat4 MVP;
        glm::vec3 lineColor;
    public:
        Line(glm::vec3 start, glm::vec3 end);

        int setMVP(glm::mat4 mvp) {
            MVP = mvp;
            return 1;
        }

        int setColor(glm::vec3 color) {
            lineColor = color;
            return 1;
        }

        int Draw();

        ~Line();
    };

    class Triangle {
    public:
        Triangle(int entity)
        {
            this->entity = entity;
        }
        void Init();


        const char* vertexShaderSource;
        const char* fragmentShaderSource;
        unsigned int vertexShader;
        unsigned int fragmentShader;
        unsigned int shaderProgram;
        unsigned int VBO, VAO, EBO;

        int entity;
        glm::vec4 color;
        void SetColor(glm::vec4 color)
        {
            color = color;
        }
    };

    class ComponentManager;
    class TriangleManager : public Renderer2D
    {
    public:
        TriangleManager(Engine& engine);

        void Init();

        void Destroy() override;

        void Update(float) override;

        void Draw();


    private:
        ComponentManager* m_component = nullptr;
    };

class Square {
public:
		Square(int entity)
		{
            this->entity = entity;
		}
        void Init();
       
    	
        const char* vertexShaderSource;
        const char* fragmentShaderSource;
        unsigned int vertexShader;
        unsigned int fragmentShader;
        unsigned int shaderProgram;
        unsigned int VBO, VAO, EBO;

        int entity;
        glm::vec4 color;
        void SetColor(glm::vec4 color)
        {
            color = color;
        }
    };

	
	class ComponentManager;
    class SquareManager : public Renderer2D
    {
    public:
        SquareManager(Engine& engine);
    	
        void Init();

        void Destroy() override;

    	void Update(float) override;
    	
        void Draw();
    	

    private:
        ComponentManager* m_component = nullptr;
    };

    class Circle {
    public:
        Circle(int entity)
        {
            this->entity = entity;
        }
        void Init();


        const char* vertexShaderSource;
        const char* fragmentShaderSource;
        unsigned int vertexShader;
        unsigned int fragmentShader;
        unsigned int shaderProgram;
        unsigned int VBO, VAO, EBO;

        int entity;
        glm::vec4 color;
        float radius;
        int ammountTriangle = 40;

        void SetColor(glm::vec4 color)
        {
            color = color;
        }

    	void SetRadius(float radius)
        {
            this->radius = radius;
        }
    };
	
    class CircleManager : public Renderer2D
    {
    public:
        CircleManager(Engine& engine);

        void Init();

        void Destroy() override;

        void Update(float) override;

        void Draw();

    private:
        ComponentManager* m_component = nullptr;
    };
}
