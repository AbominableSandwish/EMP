#pragma once
#include "editor/tool.h"

namespace emp
{
	class Editor
	{
    public:
        Editor(Engine&);
        void Init();
        void Update(float);
        void Draw();
        void Destroy();
        void Newtool(ToolType);
    private:
        Engine* m_engine = nullptr;
        std::list<Tool*> tools;
    };
}
