#include <vector>

namespace Math {

    struct Vector2
    {
    public:
        float x;
        float y;

        Vector2() = default;
        Vector2(float x, float y)
        {
            this->x = x;
            this->y = y;
        }

        void operator=(Vector2& v)
        {
            this->x = v.x;
            this->y = v.y;
        }

    	Vector2 operator+(Vector2& v)
        {
            return Vector2(this->x + v.x, this->y + v.y);
        }

        Vector2 operator-(Vector2& v)
        {
            return Vector2(this->x - v.x, this->y - v.y);
        }

    	static Vector2 Normalize()
        {
	        
        }

    	static Vector2 Reflect()
        {
	        
        }
    	
        static float Dot()
        {

        }

    	//scalaire
    };

    struct iVector2
    {
        int x;
        int y;
    };
}