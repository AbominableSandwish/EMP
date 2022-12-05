#include <components/transform.h>
#include <math/matrice.h>

namespace emp {
	Transform::Transform() {

		matrice = new Matrice4();
	}

	Transform::Transform(float x, float y, float z)
	{
		matrice = new Matrice4();
		SetPosition(Vector3(x, y, z));
		SetScale(1.0f, 1.0f, 1.0f);
	}

	Transform::Transform(float x, float y, float z, float w, float l)
	{
		matrice = new Matrice4();
		position = new Vector4(x, y, z, 1);
		this->SetPosition(Vector3(x, y, z));
		scale_x = w;
		scale_y = l;
		this->SetScale(w, l);
	}
	Transform::Transform(float x, float y, float z, float angle_x, float angle_y, float angle_z,float w, float h, float p )
	{
		matrice = new Matrice4();
		position = new Vector4(x, y, z, 1);
		this->SetPosition(Vector3(x, y, z));
		SetRotation(angle_x, Vector3(1, 0, 0));
		SetRotation(angle_y, Vector3(0, 1, 0));
		SetRotation(angle_z, Vector3(0, 0, 1));
		this->SetScale(w, h, p);
	}

	void Transform::Init() {

	}

	void Transform::Reset()
	{
		matrice = new Matrice4();
		SetPosition(Vector3(0, 0, 0));
		SetScale(1.0f, 1.0f, 1.0f);
	}

	Vector3 Transform::GetPosition()
	{
		return matrice->GetPosition();
	}

	Vector4 Transform::GetPositionPosition()
	{
		return *this->position;
	}

	void Transform::SetPosition(Vector3 position)
	{
		this->position = new Vector4(position.x, position.y, position.z, 0);
		this->matrice->SetPosition(position);
	}

	void Transform::SetPosition(float x, float y)
	{
		this->position = new Vector4(x, y, 0, 0);
		this->matrice->SetPosition(Vector3(x, y, 0));
	}

	Vector3 Transform::GetRotation()
	{
		return Vector3(0,0,0);
	}

	void Transform::SetRotation(float radiant, Vector3 axis)
	{
		if (axis.x == 1)
			this->angle_x = radiant / (3.14 * 5.5f);
		if (axis.y == 1)
			this->angle_y = radiant / (3.14 * 5.5f);
		if (axis.z == 1)
			this->angle_z = radiant / (3.14 * 5.5f);
		Matrice4 rotation = RotationMatrixFrom(radiant, axis);
		Matrice4 matrice = this->matrice->matrice4;
		this->matrice->SetMatrice(rotation);
	}

	Vector3 Transform::GetScale()
	{
		return Vector3(scale_x, scale_y, scale_z);
	}

	void Transform::SetScale(float w, float l)
	{
		scale_x = w;
		scale_y = l;
		scale_z = 1;
		Matrice4 scale = ScalingMatrix(Vector3(w, l, 1));
		Matrice4 matrice = this->matrice->matrice4;
		this->matrice->SetMatrice(matrice * scale);
	}
	void Transform::SetScale(float w, float h, float p)
	{
		scale_x = w;
		scale_y = h;
		scale_z = p;
		Matrice4 scale = ScalingMatrix(Vector3(w, h, p));
		Matrice4 matrice = this->matrice->matrice4;
		this->matrice->SetMatrice(matrice * scale);
	}

	Matrice4 Transform::ScalingMatrix(Vector3 scale)
	{
		Matrice4 matrixScaling = Matrice4();
		matrixScaling.matrice4[0].r = scale.x;
		matrixScaling.matrice4[1].g = scale.y;
		matrixScaling.matrice4[2].b = scale.z;
		matrixScaling.matrice4[3].a = 1;
		return matrixScaling;
	}

	Matrice4 Transform::TranslationMatrix(Vector3 position)
	{
		Matrice4 tranMatrix = Matrice4();

		tranMatrix.matrice4[0].a = position.x;
		tranMatrix.matrice4[1].a = position.y;
		tranMatrix.matrice4[2].a = position.z;
		tranMatrix.matrice4[3].a = 1;
		Matrice4 matrice = this->matrice->matrice4;
		this->matrice->SetMatrice(matrice + tranMatrix);
		return tranMatrix;
	}


	Matrice4  Transform::RotationMatrixFrom(const float angle, Vector3 axis)
	{
		float c = cos(angle / 1000);
		float s = sin(angle / 1000);
		const Vector3 normalizedAxis = axis.Normalized();
	
		Vector3 temp((1.0f - c) * axis.x, (1.0f - c) * axis.y, (1.0f - c) * axis.z);
		
		Matrice4 Rotate = Matrice4();
		Rotate.Zero();
		Rotate.matrice4[0].r = c + temp.x * axis.x;
		Rotate.matrice4[0].g = temp.x * axis.y + s * axis.z;
		Rotate.matrice4[0].b = temp.x * axis.z - s * axis.y;

		Rotate.matrice4[1].r = temp.y * axis.x - s * axis.z;
		Rotate.matrice4[1].g = c + temp.y * axis.y;
		Rotate.matrice4[1].b = temp.y * axis.z + s * axis.x;

		Rotate.matrice4[2].r = temp.z * axis.x + s * axis.y;
		Rotate.matrice4[2].g = temp.z * axis.y - s * axis.x;
		Rotate.matrice4[2].b = c + temp.z * axis.z;
		
		Matrice4 result;
		result.Zero();

		result.matrice4[0] = matrice->matrice4[0] * Rotate.matrice4[0].r + matrice->matrice4[1] * Rotate.matrice4[0].g + matrice->matrice4[2] * Rotate.matrice4[0].b;
		result.matrice4[1] = matrice->matrice4[0] * Rotate.matrice4[1].r + matrice->matrice4[1] * Rotate.matrice4[1].g + matrice->matrice4[2] * Rotate.matrice4[1].b;
		result.matrice4[2] = matrice->matrice4[0] * Rotate.matrice4[2].r + matrice->matrice4[1] * Rotate.matrice4[2].g + matrice->matrice4[2] * Rotate.matrice4[2].b;
		result.matrice4[3] = matrice->matrice4[3];
		return result;
	}
}