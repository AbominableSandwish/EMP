#include <math/vector.h>

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
	
	struct Matrice3
	{
		float matrice[3][3];

		float x = 0;
		float y = 0;
		int dimension = 3;
	public:


		
		Matrice3() = default;
		
		Matrice3(float x, float y)
		{
			this->x = x;
			this->y = y;

			this->matrice[0][2] = x;
			this->matrice[1][2] = y;

			this->matrice[2][0] = 1;
			this->matrice[2][1] = 1;
		}
		
		Matrice3(Vector2 position)
		{	

			this->x = position.x;
			this->y = position.y;

			this->matrice[0][2] = position.x;
			this->matrice[1][2] = position.y;

			this->matrice[2][0] = 1;
			this->matrice[2][1] = 1;
		}

		Matrice3(float x, float y, float w, float l)
		{
			this->x = x;
			this->y = y;

			this->matrice[0][2] = x;
			this->matrice[1][2] = y;

			this->matrice[2][0] = w;
			this->matrice[2][1] = l;
		}

		float& Posx()
		{
			return this->matrice[0][2];
		}

		float& Posy()
		{
			return this->matrice[1][2];
		}
		
		Matrice3(float x, float y, float axis_x, float axis_y, float w, float l)
		{

			this->x = x;
			this->y = y;

			this->matrice[0][0] = axis_x;
			this->matrice[1][1] = axis_y;
			
			this->matrice[0][2] = x;
			this->matrice[1][2] = y;

			this->matrice[2][0] = w;
			this->matrice[2][1] = l;
		}

		Matrice3(Vector2 position, Vector2 scale)
		{
			this->x = position.x;
			this->y = position.y;

			this->matrice[0][2] = position.x;
			this->matrice[1][2] = position.y;

			this->matrice[2][0] = scale.x;
			this->matrice[2][1] = scale.y;
		}

		Vector2 GetPosition()
		{
			return Vector2(x, y);
		}

		Vector2 GetMatriceRotation()
		{
			return Vector2(matrice[0][0], matrice[1][1]);
		}

		void SetPosition(Vector2 position)
		{
			matrice[0][2] = position.x;
			matrice[1][2] = position.y;

			this->x = position.x;
			this->y = position.y;
		}

		void SetRotation(Vector2 rotation)
		{
			matrice[0][0] = rotation.x;
			matrice[1][1] = rotation.y;
		}
		
		Vector2 GetScale()
		{
			return Vector2(matrice[2][0], matrice[2][1]);
		}

		void operator=(Matrice3& m)
		{
			this->matrice[0][0] = m.matrice[0][0];
			this->matrice[0][1] = m.matrice[0][1];
			this->matrice[0][2] = m.matrice[0][2];
			
			this->matrice[1][0] = m.matrice[1][0];
			this->matrice[1][1] = m.matrice[1][1];
			this->matrice[1][2] = m.matrice[1][2];
			
			this->matrice[2][0] = m.matrice[2][0];
			this->matrice[2][1] = m.matrice[2][1];
			this->matrice[2][2] = m.matrice[2][2];
		}

		Matrice3 operator+(Matrice3 & m)
		{
			this->matrice[0][0] = this->matrice[0][0] + m.matrice[0][0];
			this->matrice[0][1] = this->matrice[0][1] + m.matrice[0][1];
			this->matrice[0][2] = this->matrice[0][2] + m.matrice[0][2];
			
			this->matrice[1][0] = this->matrice[1][0] + m.matrice[1][0];
			this->matrice[1][1] = this->matrice[1][1] + m.matrice[1][1];
			this->matrice[1][2] = this->matrice[1][2] + m.matrice[1][2];
			
			this->matrice[2][0] = this->matrice[2][0] + m.matrice[2][0];
			this->matrice[2][1] = this->matrice[2][1] + m.matrice[2][1];
			this->matrice[2][2] = this->matrice[2][2] + m.matrice[2][2];
			return *this;
		}

		Matrice3 operator-(Matrice3 & m)
		{
			this->matrice[0][0] = this->matrice[0][0] - m.matrice[0][0];
			this->matrice[0][1] = this->matrice[0][1] - m.matrice[0][1];
			this->matrice[0][2] = this->matrice[0][2] - m.matrice[0][2];
			
			this->matrice[1][0] = this->matrice[1][0] - m.matrice[1][0];
			this->matrice[1][1] = this->matrice[1][1] - m.matrice[1][1];
			this->matrice[1][2] = this->matrice[1][2] - m.matrice[1][2];
			
			this->matrice[2][0] = this->matrice[2][0] - m.matrice[2][0];
			this->matrice[2][1] = this->matrice[2][1] - m.matrice[2][1];
			this->matrice[2][2] = this->matrice[2][2] - m.matrice[2][2];
			return *this;
		}
		 
		Matrice3 operator*(Matrice3& m)
		{ 
			this->matrice[0][0] = this->matrice[0][0] * m.matrice[0][0] + this->matrice[0][1] * m.matrice[1][0] + this->matrice[0][2] * m.matrice[2][0];
			this->matrice[0][1] = this->matrice[0][0] * m.matrice[0][1] + this->matrice[0][1] * m.matrice[1][1] + this->matrice[0][2] * m.matrice[2][1];
			this->matrice[0][2] = this->matrice[0][0] * m.matrice[0][2] + this->matrice[0][1] * m.matrice[1][2] + this->matrice[0][2] * m.matrice[2][2];

			this->matrice[1][0] = this->matrice[1][1] * m.matrice[0][0] + this->matrice[1][1] * m.matrice[1][0] + this->matrice[1][2] * m.matrice[2][0];
			this->matrice[1][1] = this->matrice[1][1] * m.matrice[0][1] + this->matrice[1][1] * m.matrice[1][1] + this->matrice[1][2] * m.matrice[2][1];
			this->matrice[1][2] = this->matrice[1][1] * m.matrice[0][2] + this->matrice[1][1] * m.matrice[1][2] + this->matrice[1][2] * m.matrice[2][2];
			
			this->matrice[2][0] = this->matrice[2][1] * m.matrice[0][0] + this->matrice[2][1] * m.matrice[1][0] + this->matrice[2][2] * m.matrice[2][0];
			this->matrice[2][1] = this->matrice[2][1] * m.matrice[0][1] + this->matrice[2][1] * m.matrice[1][1] + this->matrice[2][2] * m.matrice[2][1];
			this->matrice[2][2] = this->matrice[2][1] * m.matrice[0][2] + this->matrice[2][1] * m.matrice[1][2] + this->matrice[2][2] * m.matrice[2][2];
			return *this;
		}

		Matrice3 operator*(Vector2 &v)
		{
			this->matrice[0][0] = this->matrice[0][0] * v.x + this->matrice[0][1] * v.y + this->matrice[0][2] * 1;
			this->matrice[0][1] = this->matrice[1][0] * v.x + this->matrice[1][1] * v.y + this->matrice[1][2] * 1;
			this->matrice[0][2] = this->matrice[2][0] * v.x + this->matrice[2][1] * v.y + this->matrice[2][2] * 1;
			return *this;
		}

		Vector2 Translate(Vector2 &direction)
		{
			Vector2 new_position;
			new_position = (*this * direction).GetPosition();
			return  new_position;
		}

		Vector2 Rotate();

		void SetScale(Vector2 scale)
		{
			this->matrice[2][0] = scale.x;
			this->matrice[2][1] = scale.y;
		}
	};
}
