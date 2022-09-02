#include <components/renderer2D.h>
#include <core/engine.h>
#include "core/component.h"
#include <graphic/graphic.h>
#include <components/transform.h>
#include <math/matrice.h>

namespace emp
{
	Renderer2D::Renderer2D(Engine& engine, string name) : System(engine, name)
    {

    }

	Line::Line(glm::vec3 start, glm::vec3 end)
    {
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

	SquareManager::SquareManager(Engine& engine) : Renderer2D(engine, "")
    {
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
    
    const int PixelPerSize = 100;

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
        float pi = 3.14f;
        float r = 0.25f;
        float vertices[22 * 3] = { 0.0f };
        for (int ii = 0; ii < 22; ii++)
        {
            float theta = (4.0f * pi) * float(ii) / float(40);//get the current angle
            if (ii == 0) {
                vertices[ii * 3] = 0;
                vertices[ii * 3 + 1] = 0;
                vertices[ii * 3 + 2] = 0;
            }
            else {
                vertices[ii * 3] = (r * cosf(theta))/1.5f;
                vertices[ii * 3 + 1] = r * sinf(theta);
                vertices[ii * 3 + 2] = 0;
            }
            

        }
        //unsigned int indices[40 * 3] = { 0.0f };
        //for (int ii = 0; ii < 40 ;ii++)
        //{
        //    indices[ii*3] = ii;
        //    indices[ii*3 + 1] = ii + 1;
        //    indices[ii*3 + 2] = ii + 2;

        //}

        unsigned int indices[] = {  // note that we start from 0!
             0, 1, 2,  // first Triangle
             0, 2, 3,   // second Triangle
             0, 3, 4,
             0, 4, 5,
             0, 5, 6,
             0, 6, 7,
             0, 7, 8,
             0, 8, 9,
             0, 9, 10,
             0, 10, 11,
             0, 11, 12,
             0, 12, 13,
             0, 13, 14,
             0, 14, 15,
             0, 15, 16,
             0, 16, 17,
             0, 17, 18,
             0, 18, 19,
             0, 19, 20,
             0, 20, 21,
             0, 21, 22
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

	CircleManager::CircleManager(Engine& engine) : Renderer2D(engine, "")
    {
      
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
    void DrawCircle(float cx, float cy, float r, int amm_triangles)
    {
        glBegin(GL_TRIANGLE_FAN);
        glColor3b(255, 0, 0);
        glVertex2f(cx, cy);
        float vertices2[20*3] = {0.0f};
        for (int ii = 0; ii < amm_triangles; ii++)
        {
            float theta = (2.0f * pi) * float(ii) / float(amm_triangles);//get the current angle

            float x = r * cosf(theta);//calculate the x component
            float y = r * sinf(theta);//calculate the y component
            vertices2[ii] = r * cosf(theta);
            vertices2[ii+1] = r * sinf(theta);
            vertices2[ii+2] = 0;
            glVertex2f(x + cx, y + cy);//output vertex

        }
        glEnd();
    }
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
            //DrawCircle(position.x, position.x, element.radius, 40);
            glm::mat4 transf = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
            //transf = glm::translate(transf, glm::vec3(position.x / 100, position.y / 100, 0.0f));
            //transf = glm::scale(transf, glm::vec3(position.scale_x, position.scale_y, 0.0f));
            //transf = glm::rotate(transf, (float)glfwGetTime() / speedRotate, glm::vec3(0.0f, 0.0f, 1.0));

            // draw our first triangle
            glUseProgram(element.shaderProgram);
            glBindVertexArray(element.VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

            // get matrix's uniform location and set matrix
            unsigned int transformLoc = glGetUniformLocation(element.shaderProgram, "transform");
            unsigned int colorLoc = glGetUniformLocation(element.shaderProgram, "color");

            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transf));
            glUniform4f(colorLoc, element.color.r, element.color.g, element.color.b, element.color.a);
            //glDrawArrays(GL_TRIANGLES, 0, 6);
            glDrawElements(GL_TRIANGLES, 20*3, GL_UNSIGNED_INT, 0);
            // glBindVertexArray(0); // no need to unbind it every time
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
                0, 1, 3,  // first Triangle
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