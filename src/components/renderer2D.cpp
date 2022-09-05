#include <components/renderer2D.h>
#include <core/engine.h>
#include <core/config.h>
#include "core/component.h"
#include <graphic/graphic.h>
#include <components/transform.h>
#include <math/matrice.h>
#include "glm/gtx/transform.hpp"


namespace emp
{
	Renderer2D::Renderer2D(Engine& engine, string name) : System(engine, name)
    {

    }

	Line::Line(ConfigGraphic& config, glm::vec3 start, glm::vec3 end)
    {
        this->config = &config;
        startPoint = start;
        endPoint = end;
        lineColor = glm::vec3(1, 1, 1);
        MVP = glm::mat4(1.0f);

        const char* vertexShaderSource = "#version 330 core\n"
            "layout (location = 0) in vec3 aPos;\n"
            "uniform mat4 MVP;\n"
            "void main()\n"
            "{\n"
            "   gl_Position = MVP * vec4(aPos, 1.0);\n"
            "}\0";
		
        const char* fragmentShaderSource = "#version 330 core\n"
            "out vec4 FragColor;\n"
            "uniform vec3 color;\n"
		
            "void main()\n"
            "{\n"
            "   FragColor = vec4(color, 1.0f);\n"
            "}\n\0";

        // vertex shader
        int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);
        // check for shader compile errors

        // fragment shader
        int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
        // check for shader compile errors

        // link shaders
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        // check for linking errors

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        vertices = {
             start.x, start.y, start.z,
             end.x, end.y, end.z,

        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

    }

    int Line::Draw()
    {
        glUseProgram(shaderProgram);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "MVP"), 1, GL_FALSE, &MVP[0][0]);
        glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1, &lineColor[0]);

        glBindVertexArray(VAO);
        glDrawArrays(GL_LINES, 0, 2);
        return 1;
    }

	Line::~Line()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteProgram(shaderProgram);
    }
    //
    TriangleManager::TriangleManager(Engine& engine, ConfigGraphic& config) : Renderer2D(engine, "")
    {
        this->config = &config;
        m_component = engine.GetComponentManager();
    }

    void TriangleManager::Init()
    {
        m_component = engine->GetComponentManager();
    }


    void TriangleManager::Destroy()
    {
    }

    void TriangleManager::Update(float)
    {
    }

    const int PixelPerSize = 100;

    void TriangleManager::Draw()
    {

        auto arrayElement = engine->GetComponentManager()->GetComponents<Triangle>();
        for (auto element : arrayElement)
        {
            auto transform = m_component->GetComponent<Transform>(element.entity);
            Vector3 position = transform.GetPosition();
            Vector3 scale = transform.GetScale();
            std::vector<Vector4> matrice = transform.matrice->matrice4;
            glm::mat4 transf = glm::mat4(matrice[0].r, matrice[0].g, matrice[0].b, matrice[0].a,
                matrice[1].r, matrice[1].g, matrice[1].b, matrice[1].a,
                matrice[2].r, matrice[2].g, matrice[2].b, matrice[2].a,
                position.x / PixelPerSize, position.y / PixelPerSize, position.z / PixelPerSize, matrice[3].a); // make sure to initialize matrix to identity matrix first

            // draw our first triangle
            glUseProgram(element.shaderProgram);
            glBindVertexArray(element.VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

            // get matrix's uniform location and set matrix
            unsigned int transformLoc = glGetUniformLocation(element.shaderProgram, "transform");
            unsigned int colorLoc = glGetUniformLocation(element.shaderProgram, "color");


            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transf));
            glUniform4f(colorLoc, element.color.r, element.color.g, element.color.b, element.color.a);
            //glDrawArrays(GL_TRIANGLES, 0, 6);
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
            // glBindVertexArray(0); // no need to unbind it every time
        }
    }

    //
	SquareManager::SquareManager(Engine& engine, ConfigGraphic& config) : Renderer2D(engine, "")
    {
        this->config = &config;
        m_component = engine.GetComponentManager();
    }

    void SquareManager::Init()
    {
        m_component = engine->GetComponentManager();
    }


    void SquareManager::Destroy()
    {
    }

    void SquareManager::Update(float)
    {
    }

    void SquareManager::Draw()
    {
		
        auto arrayElement = engine->GetComponentManager()->GetComponents<Square>();
        for (auto element : arrayElement)
        {
            auto transform = m_component->GetComponent<Transform>(element.entity);
            Vector3 position = transform.GetPosition();
            Vector3 scale = transform.GetScale();
            std::vector<Vector4> matrice = transform.matrice->matrice4;
            glm::mat4 transf = glm::mat4(matrice[0].r, matrice[0].g, matrice[0].b, matrice[0].a,
                                         matrice[1].r, matrice[1].g, matrice[1].b, matrice[1].a,
                                         matrice[2].r, matrice[2].g, matrice[2].b, matrice[2].a,
                                         position.x / PixelPerSize, position.y / PixelPerSize, position.z / PixelPerSize, matrice[3].a); // make sure to initialize matrix to identity matrix first
            //transf = glm::translate(transf, glm::vec3(position.x / 100, position.y / 100, 0.0f));
            //transf = glm::scale(transf, glm::vec3(scale.x, scale.y, 0.0f));
            //transf = glm::rotate(transf, transform.angle_z, glm::vec3(0.0f, 0.0f, 1.0));
            

            // draw our first triangle
            glUseProgram(element.shaderProgram);
            glBindVertexArray(element.VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

            // get matrix's uniform location and set matrix
            unsigned int transformLoc = glGetUniformLocation(element.shaderProgram, "transform");
            unsigned int colorLoc = glGetUniformLocation(element.shaderProgram, "color");

        
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transf));
            glUniform4f(colorLoc, element.color.r, element.color.g, element.color.b, element.color.a);
            //glDrawArrays(GL_TRIANGLES, 0, 6);
            glDrawElements(GL_TRIANGLE_FAN, 40, GL_UNSIGNED_INT, 0);
            // glBindVertexArray(0); // no need to unbind it every time
        }
    }

	void Circle::Init()
	{
        float red = (std::rand() % 10);
        float green = (std::rand() % 10);
        float blue = (std::rand() % 10);

        color = glm::vec4(red / 10, green / 10, blue / 10, 1.0f);
        vertexShaderSource = "#version 330 core\n"
            "layout (location = 0) in vec3 aPos;\n"
            "uniform mat4 transform;\n"
            "void main()\n"
            "{\n"
            "   gl_Position = transform *vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
            "}\0";
        fragmentShaderSource = "#version 330 core\n"
            "uniform vec4 color;\n"

            "out vec4 FragColor;\n"
            "void main()\n"
            "{\n"
            "   FragColor = color;\n"
            "}\n\0";
        // build and compile our shader program
        // ------------------------------------
        // vertex shader
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);
        // check for shader compile errors
        emp::Shader::CheckVertexCompile(vertexShader);
        // fragment shader
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
        // check for shader compile errors
        emp::Shader::CheckFragmentCompile(fragmentShader);
        // link shaders
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        // check for linking errors
        emp::Shader::ChechShaderCompile(shaderProgram);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        // set up vertex data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------
        float pi = 3.141569265f;
        float r = 0.25f;
        float theta = (2 * pi) /359;
        float vertices[361 * 3] = { 0.0f };
        for (int ii = 0; ii < 361; ii++)
            
        {
            if (ii == 0) {
                vertices[ii * 3] = 0;
                vertices[ii * 3 + 1] = 0;
                vertices[ii * 3 + 2] = 0;
            }
            else {
                vertices[ii * 3] = (r * cosf(theta * (ii+1)));///1.5f
                vertices[ii * 3 + 1] = r * sinf(theta* (ii + 1));
                vertices[ii * 3 + 2] = 0;
            }
            

        }
       
        unsigned int indices[360 * 3*3+1] = { 0 };
        for (size_t i = 0; i < 360*3; i++)
        {
            if (i == 0) {
                indices[0] = 0;
                indices[1] = 0;
                indices[2] = 0;
            }
            else {
                indices[i * 3] = 0;
                indices[i * 3 + 1] = i;
                indices[i * 3 + 2] = i+ 1;
            }
        }

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
        // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        glBindVertexArray(0);
	}

	CircleManager::CircleManager(Engine& engine, ConfigGraphic& config) : Renderer2D(engine, "")
    {
        this->config = &config;
    }

	void CircleManager::Init()
	{
        m_component = engine->GetComponentManager();
	}

	void CircleManager::Destroy()
	{
	}

	void CircleManager::Update(float)
	{
	}

    float pi = 3.1415926f;
    //void DrawCircle(float cx, float cy, float r, int amm_triangles)
    //{
    //    glBegin(GL_TRIANGLE_FAN);
    //    glColor3b(255, 0, 0);
    //    glVertex2f(cx, cy);
    //    float vertices2[20*3] = {0.0f};
    //    for (int ii = 0; ii < amm_triangles; ii++)
    //    {
    //        float theta = (2.0f * pi) * float(ii) / float(amm_triangles);//get the current angle

    //        float x = r * cosf(theta);//calculate the x component
    //        float y = r * sinf(theta);//calculate the y component
    //        vertices2[ii] = r * cosf(theta);
    //        vertices2[ii+1] = r * sinf(theta);
    //        vertices2[ii+2] = 0;
    //        glVertex2f(x + cx, y + cy);//output vertex

    //    }
    //    glEnd();
    //}
    //void CircleManager::Draw()
    //{
    //    auto arrayElement = engine->GetComponentManager()->GetComponents<Circle>();
    //    for each (auto element in arrayElement)
    //    {
    //        auto transform = m_component->GetComponent<Transform>(element.entity);
    //        Vector3 position = transform.GetPosition();
    //        DrawCircle(position.x, position.x, element.radius, 40);
    //    }
    //}

	void CircleManager::Draw()
	{
        auto arrayElement = engine->GetComponentManager()->GetComponents<Circle>();
        for (auto element : arrayElement)
        {
            auto transform = m_component->GetComponent<Transform>(element.entity);
            Vector3 position = transform.GetPosition();
            Vector3 scale = transform.GetScale();
            std::vector<Vector4> matrice = transform.matrice->matrice4;
            glm::mat4 transf = glm::mat4(matrice[0].r, matrice[0].g, matrice[0].b, matrice[0].a,
                matrice[1].r, matrice[1].g, matrice[1].b, matrice[1].a,
                matrice[2].r, matrice[2].g, matrice[2].b, matrice[2].a,
                position.x / PixelPerSize, position.y / PixelPerSize, position.z / PixelPerSize, matrice[3].a);
            // draw our first triangle
            glUseProgram(element.shaderProgram);
            glBindVertexArray(element.VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

            // get matrix's uniform location and set matrix
            unsigned int transformLoc = glGetUniformLocation(element.shaderProgram, "transform");
            unsigned int colorLoc = glGetUniformLocation(element.shaderProgram, "color");

            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transf));
            glUniform4f(colorLoc, element.color.r, element.color.g, element.color.b, element.color.a);
            //glDrawArrays(GL_TRIANGLES, 0, 6);
            glDrawElements(GL_TRIANGLES, 360*3, GL_UNSIGNED_INT, 0);
            // glBindVertexArray(0); // no need to unbind it every time
        }
	}

    void Triangle::Init()
    {
        {
            float red = (std::rand() % 10);
            float green = (std::rand() % 10);
            float blue = (std::rand() % 10);

            color = glm::vec4(red / 10, green / 10, blue / 10, 1.0f);
            vertexShaderSource = "#version 330 core\n"
                "layout (location = 0) in vec3 aPos;\n"
                "uniform mat4 transform;\n"
                "void main()\n"
                "{\n"
                "   gl_Position = transform *vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                "}\0";
            fragmentShaderSource = "#version 330 core\n"
                "uniform vec4 color;\n"

                "out vec4 FragColor;\n"
                "void main()\n"
                "{\n"
                "   FragColor = color;\n"
                "}\n\0";
            // build and compile our shader program
            // ------------------------------------
            // vertex shader
            vertexShader = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
            glCompileShader(vertexShader);
            // check for shader compile errors
            emp::Shader::CheckVertexCompile(vertexShader);
            // fragment shader
            fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
            glCompileShader(fragmentShader);
            // check for shader compile errors
            emp::Shader::CheckFragmentCompile(fragmentShader);
            // link shaders
            shaderProgram = glCreateProgram();
            glAttachShader(shaderProgram, vertexShader);
            glAttachShader(shaderProgram, fragmentShader);
            glLinkProgram(shaderProgram);
            // check for linking errors
            emp::Shader::ChechShaderCompile(shaderProgram);
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);

            // set up vertex data (and buffer(s)) and configure vertex attributes
            // ------------------------------------------------------------------
            float vertices[] = {
                 0.0f, 0.5f, 0.0f,  // bottom right
                 cos(pi/6) * 0.5f, -sin(pi / 6) * 0.5f, 0.0f,  // middle top
                -cos(pi/6) * 0.5f, -sin(pi / 6) * 0.5f, 0.0f,  // bottom left
            };
            unsigned int indices[] = {  // note that we start from 0!
                0, 1, 2,  // first Triangle
            };

            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);
            // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
            //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

            // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
            // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
            glBindVertexArray(0);
        }
    }



	void Square::Init()
    {
        {
            float red = (std::rand() % 10);
            float green = (std::rand() % 10);
            float blue = (std::rand() % 10);
        	
            color = glm::vec4(red / 10 , green/10, blue/10, 1.0f);
            vertexShaderSource = "#version 330 core\n"
                "layout (location = 0) in vec3 aPos;\n"
                "uniform mat4 transform;\n"
                "void main()\n"
                "{\n"
                "   gl_Position = transform *vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                "}\0";
            fragmentShaderSource = "#version 330 core\n"
                "uniform vec4 color;\n"
        	
                "out vec4 FragColor;\n"
                "void main()\n"
                "{\n"
                "   FragColor = color;\n"
                "}\n\0";
            // build and compile our shader program
            // ------------------------------------
            // vertex shader
            vertexShader = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
            glCompileShader(vertexShader);
            // check for shader compile errors
            emp::Shader::CheckVertexCompile(vertexShader);
            // fragment shader
            fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
            glCompileShader(fragmentShader);
            // check for shader compile errors
            emp::Shader::CheckFragmentCompile(fragmentShader);
            // link shaders
            shaderProgram = glCreateProgram();
            glAttachShader(shaderProgram, vertexShader);
            glAttachShader(shaderProgram, fragmentShader);
            glLinkProgram(shaderProgram);
            // check for linking errors
            emp::Shader::ChechShaderCompile(shaderProgram);
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);

            // set up vertex data (and buffer(s)) and configure vertex attributes
            // ------------------------------------------------------------------
            float vertices[] = {
                 0.5f,  0.5f, 0.0f,  // top right
                 0.5f, -0.5f, 0.0f,  // bottom right
                -0.5f, -0.5f, 0.0f,  // bottom left
                -0.5f,  0.5f, 0.0f   // top left 
            };
            unsigned int indices[] = {  // note that we start from 0!
                0, 1, 2,  // first Triangle
                1, 2, 3   // second Triangle
            };

            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);
            // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
            //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

            // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
            // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
            glBindVertexArray(0);
        }
    }
}