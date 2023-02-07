#include <functional>
#include <map>
#include <vector>

#include <core/system.h>

namespace emp {
    class InputSystem;
    enum MushKey {
        UP, 
        DOWN,
        LEFT,
        RIGHT,
        SPACE,
        ENTER,
        CTRL_L,
        ESCAPE,
        S
    };
   
    static InputSystem* instance_input;
    class InputSystem : public System
    {
    public:
        InputSystem();
        InputSystem(Engine& engine, string name);

    	~InputSystem()
    	{
    		
            instance_input = this;
    	}

        static InputSystem* GetInstance();

        bool IsPress(MushKey key) {
            bool isPress = false;
            switch (key)
            {
            case MushKey::UP:
                if (this->isPress_up)
                    isPress = true;
                break;
            case MushKey::DOWN:
                if (isPress_down)
                    isPress = true;
                break;
            case MushKey::LEFT:
                if (isPress_left)
                    isPress = true;
                break;
            case MushKey::RIGHT:
                if (isPress_right)
                    isPress = true;
                break;
            case MushKey::SPACE:
                if (isPress_space)
                    isPress = true;
                break;
            case MushKey::ENTER:
                if (isPress_enter)
                    isPress = true;
                break;
            case MushKey::CTRL_L:
                if (isPress_ctrl_l)
                    isPress = true;
                break;
            case MushKey::ESCAPE:
                if (isPress_escape)
                    isPress = true;
                break;
            case MushKey::S:
                if (isPress_s)
                    isPress = true;
                break;
            default:
                break;
            }
            return isPress;
        }

    	void Init() override;
        void Destroy() override;
        void Update(float) override;
    private:
        bool isPress_up = false;
        bool isPress_down = false;
        bool isPress_left = false;
        bool isPress_right = false;
        bool isPress_space = false;
        bool isPress_enter = false;
        bool isPress_ctrl_l = false;
        bool isPress_escape = false;
        bool isPress_s = false;

        int GetCounterKeyPress() {
            int count = 0;
            if (isPress_up)
                count++;
            if(isPress_down)
                count++;
            if(isPress_left)
                count++;
            if(isPress_right)
                count++;
            if(isPress_space)
                count++;
            if(isPress_enter)
                count++;
            if(isPress_ctrl_l)
                count++;
            if(isPress_escape)
                count++;
            if(isPress_s)
                count++;
            return count;
        }
    	
    };

    namespace Input
    {
        static bool IsKeyDown(MushKey key)
        {
            return InputSystem::GetInstance()->IsPress(key);
        }
    }

}