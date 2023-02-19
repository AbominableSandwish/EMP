#include "tool/view.h"
#include "GL/glew.h"
#include "core/engine.h"
#include "graphic/graphic.h"

namespace emp
{
}
	void View::Init()
	{
	}

	View::View(emp::Engine& engine, std::string name) : Tool(engine, name)
	{
	}

	void View::Update(float)
	{
	}

    static void loadTexture(unsigned char* file, int& width, int& height, unsigned int& id, const char* path, int level)
    {
        // load and create a texture 
        // -------------------------
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object

        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        if (file)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, file);
        }
        else
        {
           // std::cout << "Failed to load texture" << std::endl;
        }
    }

	void View::Draw()
	{

        // Using a Child allow to fill all the space of the window.
        // It also alows customization
        ImGui::BeginChild("GameRender");
        // Get the size of the child (i.e. the whole draw size of the windows).
        ImVec2 wsize = ImGui::GetWindowSize();
        float delta = wsize.x / wsize.y;
        // Because I use the texture from OpenGL, I need to invert the V from the UV.
        ImGui::Image((ImTextureID)this->m_engine->GetGraphicManager()->textureColorbuffer, ImVec2((int)wsize.x / delta, (int)wsize.y), ImVec2(0, 1), ImVec2(1, 0));
        ImGui::EndChild();
	}

	void View::Destroy()
	{
	}
