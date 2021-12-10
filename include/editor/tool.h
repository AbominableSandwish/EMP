#pragma once
#include <string>
#include <list>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace emp
{
	class Engine;

	// /brief System Tool for editor
    class Tool
    {
    public:
        Tool(Engine& engine, std::string name)
        {
            this->m_engine = &engine;
            this->name = name;
        }
        virtual void Init() = 0;
        virtual void Update(float) = 0;
        virtual void Draw() = 0;
        virtual void Destroy() = 0;
    protected:
        virtual ~Tool() {};
        std::string name;
        Engine* m_engine = nullptr;
    };
	
    class GraphicManager;
    class Engine;
	
	enum ToolType
	{
		EMPTY,
		CONSOLE,
		PROFILER,  // todo
                HIERARCHY, 
                INSPECTOR, // todo
                VIEW,      // todo
		HEIGHT
	};

    class EmptyTool : public Tool
    {
    public:
        EmptyTool(Engine&, std::string);
        void Init() override;
        void Update(float) override;
        void Draw() override;
        void Destroy() override;
    };
}

