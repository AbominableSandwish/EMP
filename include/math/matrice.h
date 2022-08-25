#include <math/vector.h>
#define PI 3.14159265
namespace emp
{
	
	struct Matrice4
	{
		std::vector<Vector4> matrice4 = std::vector<Vector4>(4);



	public:
		Matrice4() {
			matrice4 = std::vector<Vector4>(4);
			matrice4[0].r = 1;
			matrice4[1].g = 1;
			matrice4[2].b = 1;
			matrice4[3].a = 1;
		}
		Matrice4(float x, float y, float z) {
			SetPosition(Vector3(x, y, z));
		}
		Matrice4(Vector3 position);
		Matrice4(std::vector<Vector4> matrice) : matrice4(matrice) {};

		void Zero() {
			matrice4 = std::vector<Vector4>(4);
		}

		Vector3 GetPosition()
		{
			return Vector3(matrice4[3].r, matrice4[3].g, matrice4[3].b);
		}
		void SetPosition(Vector3 position)
		{
			this->matrice4[3].r = position.x;
			this->matrice4[3].g = position.y;
			this->matrice4[3].b = position.z;
		}

		Vector3 GetScale()
		{
			return Vector3(this->matrice4[0].r, this->matrice4[1].g, this->matrice4[2].b);
		}
		void SetScale(Vector3 scale)
		{
			//this->matrice4[0].r = scale.x;
			//this->matrice4[1].g = scale.y;
			//this->matrice4[2].b = scale.z;
		}


		void operator=(Matrice4 m)
		{
			this->matrice4[0] = m.matrice4[0];
			this->matrice4[1] = m.matrice4[1];
			this->matrice4[2] = m.matrice4[2];
			this->matrice4[3] = m.matrice4[3];
		}

		void SetMatrice(Matrice4 m)
		{
			this->matrice4[0] = m.matrice4[0];
			this->matrice4[1] = m.matrice4[1];
			this->matrice4[2] = m.matrice4[2];
			this->matrice4[3] = m.matrice4[3];
		}

		Matrice4 operator*(Matrice4& m)
		{
			this->matrice4[0].r = this->matrice4[0].r * m.matrice4[0].r + this->matrice4[0].r * m.matrice4[1].r + this->matrice4[0].r * m.matrice4[2].r + this->matrice4[0].r * m.matrice4[3].r;
			this->matrice4[0].g = this->matrice4[0].g * m.matrice4[0].r + this->matrice4[0].g * m.matrice4[1].r + this->matrice4[0].g * m.matrice4[2].r + this->matrice4[0].g * m.matrice4[3].r;
			this->matrice4[0].b = this->matrice4[0].b * m.matrice4[0].r + this->matrice4[0].b * m.matrice4[1].r + this->matrice4[0].b * m.matrice4[2].r + this->matrice4[0].b * m.matrice4[3].r;
			this->matrice4[0].a = this->matrice4[0].a * m.matrice4[0].r + this->matrice4[0].a * m.matrice4[1].r + this->matrice4[0].a * m.matrice4[2].r + this->matrice4[0].a * m.matrice4[3].r;

			this->matrice4[1].r = this->matrice4[1].r * m.matrice4[0].g + this->matrice4[1].r * m.matrice4[1].g + this->matrice4[1].r * m.matrice4[2].g + this->matrice4[1].r * m.matrice4[3].g;
			this->matrice4[1].g = this->matrice4[1].g * m.matrice4[0].g + this->matrice4[1].g * m.matrice4[1].g + this->matrice4[1].g * m.matrice4[2].g + this->matrice4[1].g * m.matrice4[3].g;
			this->matrice4[1].b = this->matrice4[1].b * m.matrice4[0].g + this->matrice4[1].b * m.matrice4[1].g + this->matrice4[1].b * m.matrice4[2].g + this->matrice4[1].b * m.matrice4[3].g;
			this->matrice4[1].a = this->matrice4[1].a * m.matrice4[0].g + this->matrice4[1].a * m.matrice4[1].g + this->matrice4[1].a * m.matrice4[2].g + this->matrice4[1].a * m.matrice4[3].g;

			this->matrice4[2].r = this->matrice4[2].r * m.matrice4[0].b + this->matrice4[2].r * m.matrice4[1].b + this->matrice4[2].r * m.matrice4[2].b + this->matrice4[2].r * m.matrice4[3].b;
			this->matrice4[2].g = this->matrice4[2].g * m.matrice4[0].b + this->matrice4[2].g * m.matrice4[1].b + this->matrice4[2].g * m.matrice4[2].b + this->matrice4[2].g * m.matrice4[3].b;
			this->matrice4[2].b = this->matrice4[2].b * m.matrice4[0].b + this->matrice4[2].b * m.matrice4[1].b + this->matrice4[2].b * m.matrice4[2].b + this->matrice4[2].b * m.matrice4[3].b;
			this->matrice4[2].a = this->matrice4[2].a * m.matrice4[0].b + this->matrice4[2].a * m.matrice4[1].b + this->matrice4[2].a * m.matrice4[2].b + this->matrice4[2].a * m.matrice4[3].b;
		
			this->matrice4[3].r = this->matrice4[3].r * m.matrice4[0].a + this->matrice4[3].r * m.matrice4[1].a + this->matrice4[3].r * m.matrice4[2].a + this->matrice4[3].r * m.matrice4[3].a;
			this->matrice4[3].g = this->matrice4[3].g * m.matrice4[0].a + this->matrice4[3].g * m.matrice4[1].a + this->matrice4[3].g * m.matrice4[2].a + this->matrice4[3].g * m.matrice4[3].a;
			this->matrice4[3].b = this->matrice4[3].b * m.matrice4[0].a + this->matrice4[3].b * m.matrice4[1].a + this->matrice4[3].b * m.matrice4[2].a + this->matrice4[3].b * m.matrice4[3].a;
			this->matrice4[3].a = this->matrice4[3].a * m.matrice4[0].a + this->matrice4[3].a * m.matrice4[1].a + this->matrice4[3].a * m.matrice4[2].a + this->matrice4[3].a * m.matrice4[3].a;
			return *this;

			/*for (int i = 0; i < 4; i++) {
					this->matrice4[i].r = this->matrice4[i].r * m.matrice4[0].r + this->matrice4[i].r * m.matrice4[1].r + this->matrice4[i].r * m.matrice4[2].r + this->matrice4[i].r * m.matrice4[3].r;
				}
			}*/
		}

		Vector4 operator*(Vector4& v) {
			Vector4 result = Vector4(
				v.r * this->matrice4[0].r + v.g * this->matrice4[0].g + v.b * this->matrice4[0].b + v.a * this->matrice4[0].a,
				v.r * this->matrice4[1].r + v.g * this->matrice4[1].g + v.b * this->matrice4[1].b + v.a * this->matrice4[1].a,
				v.r * this->matrice4[2].r + v.g * this->matrice4[2].g + v.b * this->matrice4[2].b + v.a * this->matrice4[2].a,
				v.r * this->matrice4[3].r + v.g * this->matrice4[3].g + v.b * this->matrice4[3].b + v.a * this->matrice4[3].a
			);
			return result;
		}

		Matrice4 operator+(Matrice4& m)
		{
			Matrice4 result = Matrice4();
			for (int i = 0; i < 4; i++) {
				result.matrice4[i] = this->matrice4[i] + m.matrice4[i];
			}
			return result;
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

		
		Matrice3() {
			this->matrice3_ = std::vector<Vector3>(3);
			for (size_t i = 0; i < 3; i++)
			{
				this->matrice3_[i].x = 0.0f; 
				this->matrice3_[i].y = 0.0f;
				this->matrice3_[i].z = 0.0f;
			}
		}

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

			this->matrice3_[0].x = w;
			this->matrice3_[1].y = l;
		}
		
		Matrice3(float x, float y, float axis_x, float axis_y, float w, float l)
		{
			this->matrice3_ = std::vector<Vector3>(3);
			this->matrice3_[0].x = x;
			this->matrice3_[1].y = y;
			//
			//this->matrice[0][2] = x;
			//this->matrice[1][2] = y;

			this->matrice3_[0].x = w;
			this->matrice3_[1].y = l;
		}

		Matrice3(Vector3 position, Vector3 scale)
		{
			this->matrice3_ = std::vector<Vector3>(3);
			this->matrice3_[0].x = position.x;
			this->matrice3_[1].y = position.y;

			this->matrice3_[0].x = scale.x;
			this->matrice3_[1].y = scale.y;
			this->matrice3_[2].z = scale.z;
		}

		Vector3 GetPosition()
		{
			return Vector3(this->matrice3_[2].x , this->matrice3_[2].y , 0);
		}

		void SetPosition(Vector3 position)
		{

			this->matrice3_[2].x = position.x;
			this->matrice3_[2].y = position.y;
			this->matrice3_[2].z = position.z;

			
		}

		//void SetRotation(Vector2 rotation)
		//{
		//	/*matrice[0][2] = cos(90) * matrice[0][2] - sin(90) * matrice[1][2];
		//	matrice[1][2] = sin(90) * matrice[0][2] + cos(90) * matrice[1][2];*/
		//}

		
		Vector3 GetScale()
		{
			return Vector3(this->matrice3_[0].x, this->matrice3_[1].y , this->matrice3_[2].z);
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


		void SetScale(Vector3 scale)
		{
			this->matrice3_[0].x = scale.x;
			this->matrice3_[1].y = scale.y;
			this->matrice3_[2].z = scale.z;
		}
	};
}
