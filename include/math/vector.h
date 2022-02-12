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

    struct Vector3
    {
    public:
        float x;
        float y;
        float z;

        Vector3() = default;
        Vector3(float x, float y, float z)
        {
            this->x = x;
            this->y = y;
            this->z = z;
        }

        void operator=(Vector3& v)
        {
            this->x = v.x;
            this->y = v.y;
            this->z = v.z;
        }

        Vector3 operator+(Vector3& v)
        {
            return Vector3(this->x + v.x, this->y + v.y, this->z + v.z);
        }

        Vector3 operator-(Vector3& v)
        {
            return Vector3(this->x - v.x, this->y - v.y, this->z + v.z);
        }

        static Vector3 Normalize()
        {

        }

        static Vector3 Reflect()
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