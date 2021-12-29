#include <editor/tool.h>


namespace emp
{
	class Entity;
	class EntityManager;
}
	
	class Inspector : public emp::Tool
	{
	public:
		//Constructor
		Inspector(emp::Engine&, std::string);
		void Init() override;
		void Update(float) override;
		//Show Windows
		void Draw() override;
		void Destroy() override;

		static Inspector* GetInstance();

		void SetTarget(int);
	protected:
		emp::EntityManager* manager = nullptr;
		int Target = -1;
		
		bool lock_target = false;
		char buffer_label[32];

		const int HEADER_HEIGHT = 40;
		const int CHARACTER_SIZE = 16;// 16 Pixel
	};
	static Inspector* instance;
	static void SetTarget(int id)
	{
		Inspector::GetInstance()->SetTarget(id);
	}
