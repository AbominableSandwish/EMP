/*
 * Author: Dylan von Arx
 * Time: 07.10.2021
*/
#include <engine/system.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <windows.h>


#include "engine/config.h"


namespace emp {
    struct ColorRGB
    {
        float r;
        float g;
        float b;
        float a;

        ColorRGB()
        {
            float r = 0.0f;
            float g = 0.0f;
            float b = 0.0f;
            float a = 1.0f;
        }

        ColorRGB(float r, float g, float b)
        {
            this->r = r;
            this->g = g;
            this->b = b;
            this->a = 1.0f;
        }

        ColorRGB(float r, float g, float b, float a)
        {
            this->r = r;
            this->g = g;
            this->b = b;
            this->a = a;
        }
    };
    struct Screen
    {
    public:
        int _horizontal = 0;
        int _vertical = 0;
        struct ColorRGB _backgroundColor;

        Screen()
        {
            //Get Info Screen Resolution
            GetDesktopWindowResolution(_horizontal, _vertical);
        }

        Screen(int& horizontal, int& vertical)
        {
            this->_horizontal = horizontal;
            this->_vertical = vertical;
        }

        void SetScreenResolution(int& horizontal, int& vertical)
        {
            this->_horizontal = horizontal;
            this->_vertical = vertical;
        }

        // Get the horizontal and vertical screen sizes in pixel
        void GetDesktopWindowResolution(int& horizontal, int& vertical)
        {
            RECT desktop;
            // Get a handle to the desktop window
            const HWND hDesktop = GetDesktopWindow();
            // Get the size of screen to the variable desktop
            GetWindowRect(hDesktop, &desktop);
            // The top left corner will have coordinates (0,0)
            // and the bottom right corner will have coordinates
            // (horizontal, vertical)
            horizontal = desktop.right;
            vertical = desktop.bottom;
        }
    };
	
	class GraphicManager : public System
	{
	public:
		GraphicManager(Engine&, string, ConfigGraphic&);
		GraphicManager(Engine&, string);
		void Init() override;
		void Update(float) override;
		void Draw();
		void Destroy() override;

		GLFWwindow* GetWindow();

	protected:
		GLFWwindow* window = nullptr;
		ConfigGraphic* config = nullptr;

		Screen screen;

		int width = 64;
		int height = 64;
	};

	template<typename T>
	class Element
	{
	public:
		virtual void Init() = 0;
		virtual void Draw() = 0;
		virtual void Destroy() = 0;
	protected:
		virtual ~Element() {};
		string name;
	};
}
