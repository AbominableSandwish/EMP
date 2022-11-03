#include <graphic/graphic.h>
#include <components/model.h>
#include <core/engine.h>
#include <core/component.h>
#include <core/config.h>
#include <math/matrice.h>
#include <components/transform.h>


namespace emp {
	ModelManager::ModelManager(Engine& engine, ConfigGraphic& config) : System(engine, "Model Manager")
	{
        this->config = &config;
        m_component = engine.GetComponentManager();
	}

	void ModelManager::Init() {
        vertexShaderSource = "#version 330 core\n"

            "layout(location = 0) in vec3 aPos;\n"
            "layout(location = 1) in vec3 aNormal;\n"
            "layout(location = 2) in vec2 aTexCoords;\n"

            "out vec2 TexCoords;\n"

            "uniform mat4 transform;\n"
            "uniform mat4 view;\n"
            "uniform mat4 projection;\n"

            "void main()\n"
            "{\n"
            "TexCoords = aTexCoords;\n"
            "gl_Position = projection * view * transform * vec4(aPos, 1.0);\n"
            "}\n";

        fragmentShaderSource = "#version 330 core\n"

            " out vec4 FragColor;\n"

            "in vec2 TexCoords;\n"

            "uniform sampler2D texture_diffuse1;\n"

            "void main()\n"
            "{\n"
            "FragColor = texture(texture_diffuse1, TexCoords);\n"
            "}\n";


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
	}

	void ModelManager::Update(float dt) {
        this->time += dt;
	}

	void ModelManager::Draw() {
        // draw our first triangle
        glUseProgram(shaderProgram);
        int PixelPerSize = config->PixelSize;

        auto arrayElement = engine->GetComponentManager()->GetComponents<Model>();
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(project), (float)1000 / (float)1000, 0.1f, 100.0f);

        //CAMERA
        unsigned int viewPosLoc = glGetUniformLocation(shaderProgram, "viewPos");
        glUniform3f(viewPosLoc, 0.0f, 0.0f, -3.0f);

        // get matrix's uniform location and set matrix
        unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);

        glBindVertexArray(VAO);

        for (auto element : arrayElement)
        {
            //Get Transform Data
            auto transform = m_component->GetComponent<Transform>(element.entity);
            Vector3 position = transform.GetPosition();
            Vector3 scale = transform.GetScale();
            std::vector<Vector4> matrice = transform.matrice->matrice4;

            //OBJECT
            glm::mat4 transf = glm::mat4(matrice[0].r, matrice[0].g, matrice[0].b, matrice[0].a,
                matrice[1].r, matrice[1].g, matrice[1].b, matrice[1].a,
                matrice[2].r, matrice[2].g, matrice[2].b, matrice[2].a,
                position.x / PixelPerSize, position.y / PixelPerSize, position.z / PixelPerSize, matrice[3].a);
            transf = glm::rotate(transf, glm::radians(element.axis_x + time / 10), glm::vec3(0.0f, 1.0f, 0.0f));
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transf));
        
            element.Draw(shaderProgram);
        }
	}

	void ModelManager::Destroy() {

	}

    unsigned int TextureFromFile(const char* path, const string& directory, bool gamma)
    {
        string filename = string(path);
        filename = directory + '/' + filename;

        unsigned int textureID;
        glGenTextures(1, &textureID);

        int width, height, nrComponents;
        unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
        if (data)
        {
            GLenum format;
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        }
        else
        {
            std::cout << "Texture failed to load at path: " << path << std::endl;
            stbi_image_free(data);
        }

        return textureID;
    }
}
