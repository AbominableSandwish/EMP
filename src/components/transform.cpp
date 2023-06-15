#include <components/transform.h>
#include <math/matrice.h>
#include <glm/glm.hpp>
#include "imgui.h"
#include "imgui_internal.h"

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

	glm::vec3 Transform::Position() {
		return glm::vec3(this->position->r, position->g, this->position->b);
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

	void Transform::SetPosition(glm::vec3 position)
	{
		this->position = new Vector4(position.x, position.y, position.z, 0);
		this->matrice->SetPosition(Vector3(position.x, position.y, position.z));
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

	void Transform::SetRotation(float degree, Vector3 axis)
	{
		if (axis.x == 1)
			this->angle_x = degree;
		if (axis.y == 1)
			this->angle_y = degree;
		if (axis.z == 1)
			this->angle_z = degree;
		Matrice4 rotation = RotationMatrixFrom(degree * (3.14 / 180), axis);
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

	void Transform::Inspect()
	{

		emp::Vector3 position = GetPosition();
		emp::Vector3 scale = GetScale();
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
		ImGui::Text(" Transform: ");
		ImGui::PopStyleColor();
		int input_position[3] = { position.x, position.y, position.z };
		int input_rotation[3] = { angle_x , angle_y, angle_z };
		float input_scale[3] = { scale.x, scale.y, scale.z };


		bool value_changed = false;

		const char* format[3] = { "X:%1d",  "Y:%1d",  "Z:%1d" };
		static const char* fmt_table_float[3] = { "%0.1f",   "%0.1f",   "%0.1f" }; // Short display 
		ImGui::BeginGroup();
		ImGui::Text("   Position: ");
		ImGui::SameLine();


		ImGuiContext& g = *GImGui;
		ImGui::PushID("##Position");
		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		for (int i = 0; i < 3; i++)
		{
			ImGui::PushID(i);
			if (i > 0)
				ImGui::SameLine(0, g.Style.ItemInnerSpacing.x);
			value_changed |= ImGui::DragInt("", &input_position[i], 1.0f, -1000, 1000, format[i]);
			ImGui::PopID();
			ImGui::PopItemWidth();
		}
		ImGui::PopID();
		const char* label_end = ImGui::FindRenderedTextEnd("##Position");
		if ("##Position" != label_end)
		{
			ImGui::SameLine(0.0f, g.Style.ItemInnerSpacing.x);
			ImGui::TextEx("##Position", label_end);
		}
		ImGui::EndGroup();


		ImGui::BeginGroup();
		ImGui::Text("   Rotation: ");
		ImGui::SameLine();


		ImGui::PushID("##Rotation");
		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		for (int i = 0; i < 3; i++)
		{
			ImGui::PushID(i);
			if (i > 0)
				ImGui::SameLine(0, g.Style.ItemInnerSpacing.x);
			value_changed |= ImGui::DragInt("", &input_rotation[i], 1.0f, 0, 360, format[i]);
			ImGui::PopID();
			ImGui::PopItemWidth();
		}
		ImGui::PopID();
		label_end = ImGui::FindRenderedTextEnd("##Rotation");
		if ("##Rotation" != label_end)
		{
			ImGui::SameLine(0.0f, g.Style.ItemInnerSpacing.x);
			ImGui::TextEx("##Rotation", label_end);
		}

		ImGui::EndGroup();

		if (value_changed) {
			Reset();
			SetPosition(emp::Vector3(input_position[0], input_position[1], input_position[2]));
			SetRotation(input_rotation[0], emp::Vector3(1, 0, 0));
			SetRotation(input_rotation[1], emp::Vector3(0, 1, 0));
			SetRotation(input_rotation[2], emp::Vector3(0, 0, 1));
			value_changed = false;
		}

		ImGui::BeginGroup();
		ImGui::Text("   Scale:    ");
		ImGui::SameLine();


		ImGui::PushID("##Scale");
		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		for (int i = 0; i < 3; i++)
		{
			ImGui::PushID(i);
			if (i > 0)
				ImGui::SameLine(0, g.Style.ItemInnerSpacing.x);
			value_changed |= ImGui::DragFloat("", &input_scale[i], 0.1f, -10.0f, 10.0f, fmt_table_float[i]);
			ImGui::PopID();
			ImGui::PopItemWidth();
		}
		ImGui::PopID();
		label_end = ImGui::FindRenderedTextEnd("##Scale");
		if ("##Scale" != label_end)
		{
			ImGui::SameLine(0.0f, g.Style.ItemInnerSpacing.x);
			ImGui::TextEx("##Scale", label_end);
		}

		ImGui::EndGroup();

		if (value_changed) {
			Reset();
			SetPosition(emp::Vector3(input_position[0], input_position[1], input_position[2]));
			SetRotation(input_rotation[0], emp::Vector3(1, 0, 0));
			SetRotation(input_rotation[1], emp::Vector3(0, 1, 0));
			SetRotation(input_rotation[2], emp::Vector3(0, 0, 1));
			SetScale(input_scale[0], input_scale[1], input_scale[2]);
		}

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
		float c = cos(angle);
		float s = sin(angle);
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