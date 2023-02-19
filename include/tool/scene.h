#include "editor/tool.h"


namespace emp {
	class EntityManager;
}
class Scene : public emp::Tool
{
public:
	Scene(emp::Engine&, std::string);
	void Init() override;
	void Update(float) override;
	void Draw() override;
	void Destroy() override;
	//format sauvegarde à définir
	void SaveScene();
	void LoadScene();
protected:
	emp::EntityManager* m_manager = nullptr;

	bool is_open = true;
	int to_move;
	int from;

	bool is_select = false;
	bool rename = false;
	int modif = -1;
};