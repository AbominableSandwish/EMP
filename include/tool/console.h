#include <editor/tool.h>
#include "core/log.h"


namespace emp
{
	class Console : public Tool
	{
	public:
		Console(Engine&, string);
		void Init() override;
		void Update(float) override;
		void Draw() override;
		void Destroy() override;
	protected:
		LogManager* m_manager = nullptr;
		bool is_open = true;

		bool* collapse;
		// Filter
		bool* debug;
		bool* info;
		bool* warning;
		bool* error;
		bool* critical;
		
		const int HEADER_HEIGHT = 40;
		const int CHARACTER_SIZE = 16;// 16 Pixel
	};
}
