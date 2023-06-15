#include <components/light.h>
#include <core/engine.h>
#include <core/config.h>
#include "core/component.h"
#include <graphic/shader.h>
#include <graphic/graphic.h>
#include <components/transform.h>
#include <components/camera.h>
#include <math/matrice.h>
#include <core/file.h>
#include <fstream>
#include "imgui.h"
#include "imgui_internal.h"


namespace emp {

    PointLight::PointLight(int entity, float r, float g, float b, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic)
    {
        this->entity = entity;
        this->constant = constant;
        this->linear = linear;
        this->quadratic = quadratic;

        this->ambient = ambient;
        this->diffuse = diffuse;
        this->specular = specular;
    }

    void PointLight::Init()
    {
       
    }

    void PointLight::Inspect()
    {
        bool value_changed = false;

        const char* format[3] = { "X:%1d",  "Y:%1d",  "Z:%1d" };
        static const char* fmt_table_float[3] = { "%0.1f",   "%0.1f",   "%0.1f" }; // Short display 
        ImGuiContext& g = *GImGui;

        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
        ImGui::Text(" Point Light: ");
        ImGui::PopStyleColor();

        
        float specular[3] = { this->specular.r, this->specular.g, this->specular.b };
        float diffuse[3] = { this->diffuse.r,  this->diffuse.g,  this->diffuse.b };
        float ambient[3] = { this->ambient.r,  this->ambient.g,  this->ambient.b };

        ImGui::Text("   Ambiant:  ");
        ImGui::SameLine();
        value_changed = ImGui::ColorEdit3("##AmbientLight", ambient);
        if (value_changed) {
            this->SetAmbient(glm::vec3(ambient[0], ambient[1], ambient[2]));
            value_changed = false;
        }

        ImGui::Text("   Diffuse:  ");
        ImGui::SameLine();
        value_changed = ImGui::ColorEdit3("##DiffuseLight", diffuse);
        if (value_changed) {
            this->SetDiffuse(glm::vec3(diffuse[0], diffuse[1], diffuse[2]));
            value_changed = false;
        }

        ImGui::Text("   Specular: ");
        ImGui::SameLine();
        value_changed = ImGui::ColorEdit3("##SpecularLight", specular);
        if (value_changed) {
           this->SetSpecular(glm::vec3(specular[0], specular[1], specular[2]));
            value_changed = false;
        }
    }

    SpotLight::SpotLight(int entity, float r, float g, float b, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic, float cutOff, float outerCutOff)
    {
        this->entity = entity;
        this->direction = direction;
        this->constant = constant;
        this->linear = linear;
        this->quadratic = quadratic;

        this->ambient = ambient;
        this->diffuse = diffuse;
        this->specular = specular;

        this->cutOff = cutOff;
        this->outerCutOff = outerCutOff;
    }

    void SpotLight::Init()
    {

    }

    void SpotLight::Inspect()
    {

    }

    DirectionalLight::DirectionalLight(int entity, glm::vec3 direction , glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
    {
        this->entity = entity;
        this->direction = direction;
        this->ambient = ambient;
        this->diffuse = diffuse;
        this->specular = specular;
    }

    void DirectionalLight::Init()
    {

    }

    void DirectionalLight::Inspect()
    {
        bool value_changed = false;

        const char* format[3] = { "X:%1d",  "Y:%1d",  "Z:%1d" };
        static const char* fmt_table_float[3] = { "%0.1f",   "%0.1f",   "%0.1f" }; // Short display 
        ImGuiContext& g = *GImGui;

        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
        ImGui::Text(" Directional Light: ");
        ImGui::PopStyleColor();

        float direction[3] = { this->direction.x, this->direction.y, this->direction.z };

        float specular[3] = { this->specular.r, this->specular.g, this->specular.b };
        float diffuse[3] = { this->diffuse.r,  this->diffuse.g, this->diffuse.b };
        float ambient[3] = { this->ambient.r,  this->ambient.g, this->ambient.b };

        ImGui::BeginGroup();
        ImGui::Text("   Direction:");
        ImGui::SameLine();


        ImGui::PushID("##Direction");
        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        for (int i = 0; i < 3; i++)
        {
            ImGui::PushID(i);
            if (i > 0)
                ImGui::SameLine(0, g.Style.ItemInnerSpacing.x);
            value_changed |= ImGui::DragFloat("", &direction[i], 0.1f, -1.0f, 1.0f, fmt_table_float[i]);
            ImGui::PopID();
            ImGui::PopItemWidth();
        }
        ImGui::PopID();
        const char* label_end = ImGui::FindRenderedTextEnd("##Direction");
        if ("##Direction" != label_end)
        {
            ImGui::SameLine(0.0f, g.Style.ItemInnerSpacing.x);
            ImGui::TextEx("##Direction", label_end);
        }
        ImGui::EndGroup();
        if (value_changed) {
            SetDirection(glm::vec3(direction[0], direction[1], direction[2]));
            value_changed = false;
        }


        ImGui::Text("   Ambiant:  ");
        ImGui::SameLine();
        value_changed = ImGui::ColorEdit3("##AmbientLight", ambient);
        if (value_changed) {
            SetAmbient(glm::vec3(ambient[0], ambient[1], ambient[2]));
            value_changed = false;
        }

        ImGui::Text("   Diffuse:  ");
        ImGui::SameLine();
        value_changed = ImGui::ColorEdit3("##DiffuseLight", diffuse);
        if (value_changed) {
            SetDiffuse(glm::vec3(diffuse[0], diffuse[1], diffuse[2]));
            value_changed = false;
        }

        ImGui::Text("   Specular: ");
        ImGui::SameLine();
        value_changed = ImGui::ColorEdit3("##SpecularLight", specular);
        if (value_changed) {
            SetSpecular(glm::vec3(specular[0], specular[1], specular[2]));
            value_changed = false;
        }
    }

    LightManager::LightManager(Engine& engine, ConfigGraphic& config) : System(engine, "LightManager")
    {
        this->config = &config;
        m_component = engine.GetComponentManager();
    }

    void LightManager::Init()
    {
        bool warning = false;
        m_component = engine->GetComponentManager();

        {
            std::string vertexShaderSource = FileSystem::ReadShader("./shader/model2.vs");
            std::string fragmentShaderSource = FileSystem::ReadShader("./shader/light/light.fs");  //multiplelight  

            this->shader = new Shader();
            warning = this->shader->Init(vertexShaderSource, fragmentShaderSource);
            if (warning) {
                LOG::Warning(name + " help!");
            }

            // set up vertex data (and buffer(s)) and configure vertex attributes
            // ------------------------------------------------------------------
            float vertices[] = {
                -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
                 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
                 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
                 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
                -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

                -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
                 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
                 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
                 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
                -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
                -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

                -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
                -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
                -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
                -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
                -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
                -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

                 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
                 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
                 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
                 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
                 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
                 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

                -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
                 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
                 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
                 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

                -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
                 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
                 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
                 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
                -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
                -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
            };

            glGenVertexArrays(1, &this->shader->VAO);
            glGenBuffers(1, &this->shader->VBO);
            // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s). 

            glBindBuffer(GL_ARRAY_BUFFER, this->shader->VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glBindVertexArray(this->shader->VAO);
            // position attribute 
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            // normal attribut; 
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);
            //SALUT MON  pote 
            glBindBuffer(GL_ARRAY_BUFFER, this->shader->VBO);
            // note that we update the lamp's position attribute's stride to reflect the updated buffer data 
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            // at init time.

            glm::vec4 color = glm::vec4(1, 1, 1, 1);

            glGenTextures(0, &diffuse_map);
            glBindTexture(GL_TEXTURE_2D, diffuse_map);

            // set texture wrap parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            // set texture filter parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // set image data
            int size = 156;
            unsigned char* data = new unsigned char[3 * size * size * sizeof(unsigned char)];
            for (unsigned int i = 0; i < size * size; i++)
            {
                data[i * 3] = (unsigned char)(color.x * 255.0f);
                data[i * 3 + 1] = (unsigned char)(color.y * 255.0f);
                data[i * 3 + 2] = (unsigned char)(color.z * 255.0f);
            }

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            delete[] data;

            glGenTextures(1, &specular_map);
            glBindTexture(GL_TEXTURE_2D, specular_map);

            // set texture wrap parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            // set texture filter parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


            data = new unsigned char[3 * size * size * sizeof(unsigned char)];
            for (unsigned int i = 0; i < size * size; i++)
            {
                data[i * 3] = (unsigned char)(color.x * 255.0f);
                data[i * 3 + 1] = (unsigned char)(color.y * 255.0f);
                data[i * 3 + 2] = (unsigned char)(color.z * 255.0f);
            }

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            delete[] data;

            this->shader->UseProgram();

            this->shader->SetInt("material.diffuse", 0);
            this->shader->SetInt("material.specular", 1);
        }

    }

    void LightManager::Start() {
        this->uniformBlockIndex = glGetUniformBlockIndex(shader->shaderProgram, "Matrices");
        // then we link each shader's uniform block to this uniform binding point
        glUniformBlockBinding(shader->shaderProgram, uniformBlockIndex, 0);
        // Now actually create the buffer
        glGenBuffers(1, &uboMatrices);
        glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
        glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        // define the range of the buffer that links to a uniform binding point
        glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4));
        //CAMERA       
        auto& list_camera = m_component->GetComponents<Camera>();
        Camera& MainCamera = list_camera[0];
        glm::mat4& view = MainCamera.GetView();
        //this->shader->SetMat4("view", view);
        //this->shader->SetMat4("projection", MainCamera.projection);

        // set the view and projection matrix in the uniform block - we only have to do this once per loop iteration.
        //glm::mat4 view = camera.GetViewMatrix();
        glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &view);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        // store the projection matrix
        //glm::mat4 projection = glm::perspective(45.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &MainCamera.projection);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    void LightManager::Destroy()
    {
    }

    void LightManager::Update(float dt)
    {
        time += dt;
    }

    void LightManager::Draw()
    {
        this->shader->UseProgram();

        auto& arrayElement = engine->GetComponentManager()->GetComponents<PointLight>();
        // render boxes

        auto& list_camera = m_component->GetComponents<Camera>();
        Camera& MainCamera = list_camera[0];
        glm::vec3 pos = MainCamera.GetPosition();
        // this->shader->SetVec3("viewPos", pos);

        this->shader->BindVertexArray(this->shader->VAO);

        glm::mat4& view = MainCamera.GetView();
        glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &view);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        for (auto& element : arrayElement)
        {
            //Matrice Transform
            glm::mat4& transf = m_component->GetComponent<Transform>(element.entity).matrice->GetMatrice();
            // transf = glm::rotate(transf, glm::radians(time * 50), glm::vec3(0.0f, 1.0f, 0.0f));

            //transf = glm::rotate(transf, glm::radians(sin(time)*180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            this->shader->SetVec3("color", glm::vec3(element.specular.r, element.specular.g, element.specular.b));
            this->shader->SetMat4("transform", transf);
            if (update) {
               
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, diffuse_map);
                // at init time.
                //glm::vec4 whiteColor = glm::vec4(1, 1, 1, 1);    
                glGenTextures(1, &specular_map);
                glBindTexture(GL_TEXTURE_2D, specular_map);
                // bind specular map
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, specular_map);
               
            }
            this->shader->DrawArrays(GL_TRIANGLES, 0, 36);
        }
        update = false;
    }
}