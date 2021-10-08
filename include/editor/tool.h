#pragma once
#include <string>
#include <list>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace emp
{
    class Tool;
    class GraphicManager;
    class Engine;

	enum ToolType
	{
		Sample
	};

	
	class ToolManager
	{
	public:
        ToolManager();
        void Init(Engine&);
        void Update(float);
        void Draw();
        void Destroy();
	private:
        std::list<Tool*> tools;
	};

    // /brief System Tool for editor
	//template<typename T>
    class Tool
    {
    public:
        virtual void Init() = 0;
        virtual void Update(float) = 0;
        virtual void Draw() = 0;
        virtual void Destroy() = 0;
    protected:
        virtual ~Tool() {};
        std::string name;
    };

	class sampleTool : public Tool
	{
        void Init() override;
        void Update(float) override;
        void Draw() override;
        void Destroy() override;
	};
}

