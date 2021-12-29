#include <vector>

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
        GraphicManager* graphic = nullptr;
        std::vector<Tool*> tools;
    };
}
