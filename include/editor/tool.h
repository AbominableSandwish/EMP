#pragma once
#include <string>
#include <list>
#include "imgui.h"
#include "imgui_impl_sdl.h"
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

            this->is_open = true;
        }
        virtual void Init() = 0;
        virtual void Update(float) = 0;
        virtual void Draw() = 0;
        virtual void Destroy() = 0;
        std::string name;
        bool is_open;
    protected:
        virtual ~Tool() {};
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
        SCENE,
        VIEW,
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

