#include <components/transform.h>
#include <math/matrice.h>

namespace emp {
	Transform::Transform() {
		matrice = new Matrice3();

		this->scale_x = 1;
		this->scale_y = 1;
	}

	Transform::Transform(float x, float y)
	{
		matrice = new Matrice3(x, y);
		this->scale_x = 1;
		this->scale_y = 1;
	}

	Transform::Transform(float x, float y, float w, float l)
	{
		matrice = new Matrice3(x, y, w, l);

		this->scale_x = w;
		this->scale_y = l;
	}

	void Transform::Init() {

	}

	Vector3 Transform::GetPosition()
	{
		return matrice->GetPosition();
	}

	void Transform::SetPosition(Vector3 position)
	{
		this->matrice->SetPosition(position);
	}

	void Transform::SetPosition(float x, float y)
	{
		this->matrice->SetPosition(Vector3(x, y, 0));
	}

	Vector3 Transform::GetRotation()
	{
		return Vector3(0,0,0);
	}

	void Transform::SetRotation(float radiant, Vector3 axis)
	{
		if (axis.x == 1)
			this->angle_x = radiant;
		Matrice3 rotation = this->matrice->RotationMatrixFrom(radiant, axis);
		Matrice3 matrice = this->matrice->matrice3_;
		this->matrice->SetMatrice(matrice * rotation);
	}

	Vector3 Transform::GetScale()
	{
		return matrice->GetScale();
	}

	void Transform::SetScale(float w, float l)
	{
		///this->matrice->SetScale(Vector2(w, l));

	}
}