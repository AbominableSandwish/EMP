#include <vector>

namespace emp {

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
        float x =0;
        float y =0;
        float z =0;

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

        Vector3 operator/(float f) {
            float xp = x / f;
            float yp = y / f;
            float zp = z / f;
            return Vector3(xp, yp, zp);
        }

        static Vector3 Normalize()
        {
         
        }

        float Magnitude()
        {
            return sqrt(x * x + y * y);
        }


        Vector3 Normalized()
        {
            float magnitude = (*this).Magnitude();
            this->x = x / magnitude;
            this->y = y / magnitude;
            this->z = z / magnitude;
            return *this;
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