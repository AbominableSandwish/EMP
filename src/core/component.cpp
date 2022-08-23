#include <core/component.h>
#include <math/matrice.h>

namespace emp {
}
	Transform::Transform() {
		matrice = Matrice3();

		this->scale_x = 1;
		this->scale_y = 1;
	}

	Transform::Transform(float x, float y)
	{
		matrice = Matrice3(x, y);
		this->scale_x = 1;
		this->scale_y = 1;
	}

	Transform::Transform(float x, float y, float w, float l)
	{
		matrice = Matrice3(x, y, w, l);

		this->scale_x = w;
		this->scale_y = l;
	}

	void Transform::Init() {

	}

	Vector2 Transform::GetPosition()
	{
		return matrice.GetPosition();
	}

	void Transform::SetPosition(Vector2 position)
	{
		this->matrice.SetPosition(position);
	}

	void Transform::SetPosition(float x, float y)
	{
		this->matrice.SetPosition(Vector2(x, y));
	}

	Vector2 Transform::GetRotation()
	{
		return this->matrice.GetMatriceRotation();
	}

	void Transform::SetRotation(float axis_x, float axis_y)
	{
		this->matrice.SetRotation(Vector2(axis_x, axis_y));
	}

	Vector2 Transform::GetScale()
	{
		return matrice.GetScale();
	}

	void Transform::SetScale(float w, float l)
	{
		this->matrice.SetScale(Vector2(w, l));

	}
