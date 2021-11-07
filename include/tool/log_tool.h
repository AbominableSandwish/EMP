#include <editor/tool.h>
#include "engine/log.h"


namespace emp
{
	class Logger : public Tool
	{
	public:
		Logger(Engine&, string);
		void Init() override;
		void Update(float) override;
		void Draw() override;
		void Destroy() override;
	protected:
		LogManager* m_manager = nullptr;
		bool is_open = true;
		
	};
}
