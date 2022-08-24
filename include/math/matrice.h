#include <math/vector.h>
#define PI 3.14159265
namespace emp
{
	
	struct Matrice4
	{
		float matrice[4][4];

	public:
		Matrice4() = default;
		Matrice4(float x, float y, float z);
		Matrice4(Vector3 position);

		Vector3 GetPosition()
		{
			return Vector3(matrice[3][0], matrice[3][1], matrice[3][2]);
		}

		Vector3 GetScale();

		void operator=(Matrice4& m)
		{
			this->matrice[0][0] = m.matrice[0][0];
			this->matrice[0][1] = m.matrice[0][1];
			this->matrice[0][2] = m.matrice[0][2];
			this->matrice[0][3] = m.matrice[0][3];
			
			this->matrice[1][0] = m.matrice[1][0];
			this->matrice[1][1] = m.matrice[1][1];
			this->matrice[1][2] = m.matrice[1][2];
			this->matrice[1][3] = m.matrice[1][3];
			
			this->matrice[2][0] = m.matrice[2][0];
			this->matrice[2][1] = m.matrice[2][1];
			this->matrice[2][2] = m.matrice[2][2];
			this->matrice[2][3] = m.matrice[2][3];
			
			this->matrice[3][0] = m.matrice[3][0];
			this->matrice[3][1] = m.matrice[3][1];
			this->matrice[3][2] = m.matrice[3][2];
			this->matrice[3][3] = m.matrice[3][3];
		}

		Matrice4 operator+(Matrice4& m)
		{
			this->matrice[0][0] = this->matrice[0][0] + m.matrice[0][0];
			this->matrice[0][1] = this->matrice[0][1] + m.matrice[0][1];
			this->matrice[0][2] = this->matrice[0][2] + m.matrice[0][2];
			this->matrice[0][3] = this->matrice[0][3] + m.matrice[0][3];
			
			this->matrice[1][0] = this->matrice[1][0] + m.matrice[1][0];
			this->matrice[1][1] = this->matrice[1][1] + m.matrice[1][1];
			this->matrice[1][2] = this->matrice[1][2] + m.matrice[1][2];
			this->matrice[1][3] = this->matrice[1][3] + m.matrice[1][3];
			
			this->matrice[2][0] = this->matrice[2][0] + m.matrice[2][0];
			this->matrice[2][1] = this->matrice[2][1] + m.matrice[2][1];
			this->matrice[2][2] = this->matrice[2][2] + m.matrice[2][2];
			this->matrice[2][3] = this->matrice[2][3] + m.matrice[2][3];
			
			this->matrice[3][0] = this->matrice[3][0] + m.matrice[3][0];
			this->matrice[3][1] = this->matrice[3][1] + m.matrice[3][1];
			this->matrice[3][2] = this->matrice[3][2] + m.matrice[3][2];
			this->matrice[3][3] = this->matrice[3][3] + m.matrice[3][3];
			
			return *this;
		}

		Matrice4 operator-(Matrice4& m)
		{
			this->matrice[0][0] = this->matrice[0][0] - m.matrice[0][0];
			this->matrice[0][1] = this->matrice[0][1] - m.matrice[0][1];
			this->matrice[0][2] = this->matrice[0][2] - m.matrice[0][2];
			this->matrice[0][3] = this->matrice[0][3] - m.matrice[0][3];
			
			this->matrice[1][0] = this->matrice[1][0] - m.matrice[1][0];
			this->matrice[1][1] = this->matrice[1][1] - m.matrice[1][1];
			this->matrice[1][2] = this->matrice[1][2] - m.matrice[1][2];
			this->matrice[1][3] = this->matrice[1][3] - m.matrice[1][3];
			
			this->matrice[2][0] = this->matrice[2][0] - m.matrice[2][0];
			this->matrice[2][1] = this->matrice[2][1] - m.matrice[2][1];
			this->matrice[2][2] = this->matrice[2][2] - m.matrice[2][2];
			this->matrice[2][3] = this->matrice[2][3] - m.matrice[2][3];
			
			this->matrice[3][0] = this->matrice[3][0] - m.matrice[3][0];
			this->matrice[3][1] = this->matrice[3][1] - m.matrice[3][1];
			this->matrice[3][2] = this->matrice[3][2] - m.matrice[3][2];
			this->matrice[3][3] = this->matrice[3][3] - m.matrice[3][3];
			return *this;
		}

		Matrice4 operator*(Matrice4& m)
		{
			//todo
			return *this;
		}


		void Translate(Vector3 direction)
		{
			//todos
		}

		//this creates my translation matrice which causes the cube to disappear
		const Matrice4 translateMatrice(float x, float y, float z)
		{
			Matrice4 tranMatrix = Matrice4(x,y,z);
			return Matrice4(tranMatrix);
		}
	};
	
	class Matrice3
	{
		
		int dimension = 2;
	public:
		std::vector<Vector3> matrice3_;

		
		Matrice3() = default;

		Matrice3(std::vector<Vector3> matrice) {
			this->matrice3_ = matrice;
		}
		
		Matrice3(float x, float y)
		{
			this->matrice3_ = std::vector<Vector3>(3);
			this->matrice3_[0].x = x;
			this->matrice3_[1].y = y;

			this->matrice3_[2].x = 1;
			this->matrice3_[2].y = 1;
		}
		
		Matrice3(Vector3 position)
		{	
			this->matrice3_ = std::vector<Vector3>(3);
		}

		Matrice3(float x, float y, float w, float l)
		{
			this->matrice3_ = std::vector<Vector3>(3);
			this->matrice3_[0].x = x;
			this->matrice3_[1].y = y;

			this->matrice3_[2].x = w;
			this->matrice3_[2].y = l;
		}
		
		Matrice3(float x, float y, float axis_x, float axis_y, float w, float l)
		{
			this->matrice3_ = std::vector<Vector3>(3);
			this->matrice3_[0].x = x;
			this->matrice3_[1].y = y;
			//
			//this->matrice[0][2] = x;
			//this->matrice[1][2] = y;

			this->matrice3_[2].x = w;
			this->matrice3_[2].y = l;
		}

		Matrice3(Vector3 position, Vector3 scale)
		{
			this->matrice3_ = std::vector<Vector3>(3);
			this->matrice3_[0].x = position.x;
			this->matrice3_[1].y = position.y;

			this->matrice3_[2].x = scale.x;
			this->matrice3_[2].y = scale.y;
		}

		Vector3 GetPosition()
		{
			return Vector3(0, 0, 0);
		}

		void SetPosition(Vector3 position)
		{
			//this->matrice3_[0].x = position.x;
			//this->matrice3_[1].y = position.y;
		}

		//void SetRotation(Vector2 rotation)
		//{
		//	/*matrice[0][2] = cos(90) * matrice[0][2] - sin(90) * matrice[1][2];
		//	matrice[1][2] = sin(90) * matrice[0][2] + cos(90) * matrice[1][2];*/
		//}

		Matrice3 RotationMatrixFrom(const float angle, Vector3 axis)
		{
			const Vector3 normalizedAxis = axis.Normalized();

			const float x = normalizedAxis.x;
			const float y = normalizedAxis.y;
			const float c = cos(angle);
			const float s = sin(angle);
			const float t = 1.0f - c;
			const float txx = t * x * x;
			const float tyy = t * y * y;
			const float txy = t * x * y;
			const float sx = s * x;
			const float sy = s * y;


			return Matrice3(
				std::vector<Vector3>
			{
					Vector3(txx + c, txy, 0),
					Vector3(txy, tyy + c, 0),
					Vector3(sy, sx, 1)});
		}
		
		Vector3 GetScale()
		{
			return this->matrice3_[2];
		}

		//void operator=(Matrice3& m)
		//{
		//	this->matrice[0][0] = m.matrice[0][0];
		//	this->matrice[0][1] = m.matrice[0][1];
		//	this->matrice[0][2] = m.matrice[0][2];
		//	
		//	this->matrice[1][0] = m.matrice[1][0];
		//	this->matrice[1][1] = m.matrice[1][1];
		//	this->matrice[1][2] = m.matrice[1][2];
		//	
		//	this->matrice[2][0] = m.matrice[2][0];
		//	this->matrice[2][1] = m.matrice[2][1];
		//	this->matrice[2][2] = m.matrice[2][2];
		//}

		//Matrice3 operator+(Matrice3 & m)
		//{
		//	this->matrice[0][0] = this->matrice[0][0] + m.matrice[0][0];
		//	this->matrice[0][1] = this->matrice[0][1] + m.matrice[0][1];
		//	this->matrice[0][2] = this->matrice[0][2] + m.matrice[0][2];
		//	
		//	this->matrice[1][0] = this->matrice[1][0] + m.matrice[1][0];
		//	this->matrice[1][1] = this->matrice[1][1] + m.matrice[1][1];
		//	this->matrice[1][2] = this->matrice[1][2] + m.matrice[1][2];
		//	
		//	this->matrice[2][0] = this->matrice[2][0] + m.matrice[2][0];
		//	this->matrice[2][1] = this->matrice[2][1] + m.matrice[2][1];
		//	this->matrice[2][2] = this->matrice[2][2] + m.matrice[2][2];
		//	return *this;
		//}

		//Matrice3 operator-(Matrice3 & m)
		//{
		//	this->matrice[0][0] = this->matrice[0][0] - m.matrice[0][0];
		//	this->matrice[0][1] = this->matrice[0][1] - m.matrice[0][1];
		//	this->matrice[0][2] = this->matrice[0][2] - m.matrice[0][2];
		//	
		//	this->matrice[1][0] = this->matrice[1][0] - m.matrice[1][0];
		//	this->matrice[1][1] = this->matrice[1][1] - m.matrice[1][1];
		//	this->matrice[1][2] = this->matrice[1][2] - m.matrice[1][2];
		//	
		//	this->matrice[2][0] = this->matrice[2][0] - m.matrice[2][0];
		//	this->matrice[2][1] = this->matrice[2][1] - m.matrice[2][1];
		//	this->matrice[2][2] = this->matrice[2][2] - m.matrice[2][2];
		//	return *this;
		//}
		// 

		Matrice3 operator=(Matrice3& m)
		{
			this->matrice3_[0].x = m.matrice3_[0].x;
			this->matrice3_[0].y = m.matrice3_[0].y;
			this->matrice3_[0].z = m.matrice3_[0].z;

				this->matrice3_[1].x = m.matrice3_[1].x;
				this->matrice3_[1].y = m.matrice3_[1].y;
				this->matrice3_[1].z = m.matrice3_[1].z;

				this->matrice3_[2].x = m.matrice3_[2].x;
				this->matrice3_[2].y = m.matrice3_[2].y;
				this->matrice3_[2].z = m.matrice3_[2].z;
		}

		void SetMatrice(Matrice3 m)
		{
			this->matrice3_[0].x = m.matrice3_[0].x;
			this->matrice3_[0].y = m.matrice3_[0].y;
			this->matrice3_[0].z = m.matrice3_[0].z;

			this->matrice3_[1].x = m.matrice3_[1].x;
			this->matrice3_[1].y = m.matrice3_[1].y;
			this->matrice3_[1].z = m.matrice3_[1].z;

			this->matrice3_[2].x = m.matrice3_[2].x;
			this->matrice3_[2].y = m.matrice3_[2].y;
			this->matrice3_[2].z = m.matrice3_[2].z;
		}

		Matrice3 operator*(Matrice3& m)
		{ 
			this->matrice3_[0].x = this->matrice3_[0].x * m.matrice3_[0].x + this->matrice3_[0].y * m.matrice3_[1].x + this->matrice3_[0].z * m.matrice3_[2].x;
			this->matrice3_[0].y = this->matrice3_[0].x * m.matrice3_[0].y + this->matrice3_[0].y * m.matrice3_[1].y + this->matrice3_[0].z * m.matrice3_[2].y;
			this->matrice3_[0].z = this->matrice3_[0].x * m.matrice3_[0].z + this->matrice3_[0].y * m.matrice3_[1].z + this->matrice3_[0].z * m.matrice3_[2].z;

			this->matrice3_[1].x = this->matrice3_[1].y * m.matrice3_[0].x + this->matrice3_[1].y * m.matrice3_[1].x + this->matrice3_[1].z * m.matrice3_[2].x;
			this->matrice3_[1].y = this->matrice3_[1].y * m.matrice3_[0].y + this->matrice3_[1].y * m.matrice3_[1].y + this->matrice3_[1].z * m.matrice3_[2].y;
			this->matrice3_[1].z = this->matrice3_[1].y * m.matrice3_[0].z + this->matrice3_[1].y * m.matrice3_[1].z + this->matrice3_[1].z * m.matrice3_[2].z;
			
			this->matrice3_[2].x = this->matrice3_[2].y * m.matrice3_[0].x + this->matrice3_[2].y * m.matrice3_[1].x + this->matrice3_[2].z * m.matrice3_[2].x;
			this->matrice3_[2].y = this->matrice3_[2].y * m.matrice3_[0].y + this->matrice3_[2].y * m.matrice3_[1].y + this->matrice3_[2].z * m.matrice3_[2].y;
			this->matrice3_[2].z = this->matrice3_[2].y * m.matrice3_[0].z + this->matrice3_[2].y * m.matrice3_[1].z + this->matrice3_[2].z * m.matrice3_[2].z;
			return *this;
		}

		//Matrice3 operator*(Vector2 &v)
		//{
		//	this->matrice[0][0] = this->matrice[0][0] * v.x + this->matrice[0][1] * v.y + this->matrice[0][2] * 1;
		//	this->matrice[0][1] = this->matrice[1][0] * v.x + this->matrice[1][1] * v.y + this->matrice[1][2] * 1;
		//	this->matrice[0][2] = this->matrice[2][0] * v.x + this->matrice[2][1] * v.y + this->matrice[2][2] * 1;
		//	return *this;
		//}

		//Vector2 Translate(Vector2 &direction)
		//{
		//	Vector2 new_position;
		//	new_position = (*this * direction).GetPosition();
		//	return  new_position;
		//}

		Vector2 Rotate();

		void SetScale(Vector2 scale)
		{
			//this->matrice3_[2].x = scale.x;
			//this->matrice3_[2].y = scale.y;
		}
	};
}
