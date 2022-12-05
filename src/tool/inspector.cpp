#include <tool/inspector.h>
#include "core/log.h"
#include <core/engine.h>
#include <core/entity.h>
#include "graphic/graphic.h"
#include <core/component.h>
#include <math/matrice.h>
#include <components/transform.h>
#include <components/light.h>
#include "imgui.h"
#include "imgui_internal.h"

namespace emp {
	class Transform;
}


	void Inspector::Destroy()
	{
		
	}
	
	void Inspector::SetTarget(int id)
	{
		Target = id;
	}

	Inspector* Inspector::GetInstance()
	{
		if (instance == nullptr)
		{
			return nullptr;
		}
		return instance;
	}


    void Inspector::Init()
    {
        this->manager = m_engine->GetEntityManager();
    }

    Inspector::Inspector(emp::Engine& engine, std::string name) : Tool(engine, name)
    {
        instance = this;
    }

    void Inspector::Update(float)
    {
    }

    float col2[4] = { 0, 0, 0.0f, 0.0f };

    void Inspector::Draw()
    {
        if (Target != -1) {
            int bufSize = 32;
            char buffer[32] = {};
            int c = 0;
            emp::Entity* entity = manager->GetEntity(Target);
            for (auto character : entity->GetName())
            {
                buffer[c] = character;
                c++;
            }
            ImGui::Text("Entity Name: ");
            ImGui::SameLine();
            if (ImGui::InputText("##name", buffer, bufSize, ImGuiInputTextFlags_EnterReturnsTrue))
            {
                emp::LOG::Debug("Setting attributes Spore");
                entity->SetName(buffer);
            }

            ImGui::Separator();
            emp::Transform& tranform = m_engine->GetComponentManager()->GetComponent<emp::Transform>(Target);

            float angle_x = tranform.angle_x;
            float angle_y = tranform.angle_y;
            float angle_z = tranform.angle_z;

            emp::Vector3 position = tranform.GetPosition();
            emp::Vector3 scale = tranform.GetScale();
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
            ImGui::Text(" Transform: ");
            ImGui::PopStyleColor();
            int input_position[3] = { position.x, position.y, position.z};
            int input_rotation[3] = { angle_x , angle_y, angle_z };
            float input_scale[3] = { scale.x, scale.y, scale.z };
          

            bool value_changed = false;
         
            const char* format[3] = { "X:%1d",  "Y:%1d",  "Z:%1d" };
            static const char* fmt_table_float[3]= { "%0.1f",   "%0.1f",   "%0.1f" }; // Short display 
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
                tranform.Reset();
                tranform.SetPosition(emp::Vector3(input_position[0], input_position[1], input_position[2]));
                tranform.SetRotation(input_rotation[0] * 3.14f * 5.5f, emp::Vector3(1, 0, 0));
                tranform.SetRotation(input_rotation[1] * 3.14f * 5.5f, emp::Vector3(0, 1, 0));
                tranform.SetRotation(input_rotation[2] * 3.14f * 5.5f, emp::Vector3(0, 0, 1));
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
                tranform.Reset();
                tranform.SetPosition(emp::Vector3(input_position[0], input_position[1], input_position[2]));
                tranform.SetRotation(input_rotation[0] * 3.14f * 5.5f, emp::Vector3(1, 0, 0));
                tranform.SetRotation(input_rotation[1] * 3.14f * 5.5f, emp::Vector3(0, 1, 0));
                tranform.SetRotation(input_rotation[2] * 3.14f * 5.5f, emp::Vector3(0, 0, 1));
                tranform.SetScale(input_scale[0], input_scale[1], input_scale[2]);
            }


            ImGui::Separator();

            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
            ImGui::Text(" Material: ");
            ImGui::PopStyleColor();

            emp::PointLight& light = m_engine->GetComponentManager()->GetComponent<emp::PointLight>(Target);
            value_changed = false;

            float red = light.specular.r;
            float green = light.specular.g;
            float blue = light.specular.b;

            float input_red = red;
            float input_green = green;
            float input_blue = blue;
            int shininess= 32;
            ImGui::Text("   Shininess:");
            ImGui::SameLine();
            ImGui::DragInt("", &shininess, 1.0f, 0, 100);
            static float col1[3] = { 1.0f, 0.0f, 0.2f };
            ImGui::Text("   Ambiant:  ");
            ImGui::SameLine();
            ImGui::ColorEdit3("", col1);

            ImGui::Text("   Diffuse:  ");
            ImGui::SameLine();
            ImGui::ColorEdit3("", col1);
            
            ImGui::Text("   Specular: ");
            ImGui::SameLine();
            ImGui::ColorEdit3("", col1);



           
        }
    }


    //// Helper for ColorPicker4()
    //static void RenderArrowsForVerticalBar(ImDrawList* draw_list, ImVec2 pos, ImVec2 half_sz, float bar_w, float alpha)
    //{
    //    ImU32 alpha8 = IM_F32_TO_INT8_SAT(alpha);
    //    ImGui::RenderArrowPointingAt(draw_list, ImVec2(pos.x + half_sz.x + 1, pos.y), ImVec2(half_sz.x + 2, half_sz.y + 1), ImGuiDir_Right, IM_COL32(0, 0, 0, alpha8));
    //    ImGui::RenderArrowPointingAt(draw_list, ImVec2(pos.x + half_sz.x, pos.y), half_sz, ImGuiDir_Right, IM_COL32(255, 255, 255, alpha8));
    //    ImGui::RenderArrowPointingAt(draw_list, ImVec2(pos.x + bar_w - half_sz.x - 1, pos.y), ImVec2(half_sz.x + 2, half_sz.y + 1), ImGuiDir_Left, IM_COL32(0, 0, 0, alpha8));
    //    ImGui::RenderArrowPointingAt(draw_list, ImVec2(pos.x + bar_w - half_sz.x, pos.y), half_sz, ImGuiDir_Left, IM_COL32(255, 255, 255, alpha8));
    //}



    //bool Picker4(const char* label, float col[4], ImGuiColorEditFlags flags, const float* ref_col)
    //{
    //    ImGuiContext& g = *GImGui;
    //    ImGuiWindow* window = ImGui::GetCurrentWindow();
    //    if (window->SkipItems)
    //        return false;

    //    ImDrawList* draw_list = window->DrawList;
    //    ImGuiStyle& style = g.Style;
    //    ImGuiIO& io = g.IO;

    //    const float width = ImGui::CalcItemWidth();
    //    g.NextItemData.ClearFlags();

    //    ImGui::PushID(label);
    //    ImGui::BeginGroup();

    //    if (!(flags & ImGuiColorEditFlags_NoSidePreview))
    //        flags |= ImGuiColorEditFlags_NoSmallPreview;

    //    // Context menu: display and store options.
    //    if (!(flags & ImGuiColorEditFlags_NoOptions))
    //        ImGui::ColorPickerOptionsPopup(col, flags);

    //    // Read stored options
    //    if (!(flags & ImGuiColorEditFlags_PickerMask_))
    //        flags |= ((g.ColorEditOptions & ImGuiColorEditFlags_PickerMask_) ? g.ColorEditOptions : ImGuiColorEditFlags_DefaultOptions_) & ImGuiColorEditFlags_PickerMask_;
    //    if (!(flags & ImGuiColorEditFlags_InputMask_))
    //        flags |= ((g.ColorEditOptions & ImGuiColorEditFlags_InputMask_) ? g.ColorEditOptions : ImGuiColorEditFlags_DefaultOptions_) & ImGuiColorEditFlags_InputMask_;
    //    IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiColorEditFlags_PickerMask_)); // Check that only 1 is selected
    //    IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiColorEditFlags_InputMask_));  // Check that only 1 is selected
    //    if (!(flags & ImGuiColorEditFlags_NoOptions))
    //        flags |= (g.ColorEditOptions & ImGuiColorEditFlags_AlphaBar);

    //    // Setup
    //    int components = (flags & ImGuiColorEditFlags_NoAlpha) ? 3 : 4;
    //    bool alpha_bar = (flags & ImGuiColorEditFlags_AlphaBar) && !(flags & ImGuiColorEditFlags_NoAlpha);
    //    ImVec2 picker_pos = window->DC.CursorPos;
    //    float square_sz = ImGui::GetFrameHeight();
    //    float bars_width = square_sz; // Arbitrary smallish width of Hue/Alpha picking bars
    //    float sv_picker_size = ImMax(bars_width * 1, width - (alpha_bar ? 2 : 1) * (bars_width + style.ItemInnerSpacing.x)); // Saturation/Value picking box
    //    float bar0_pos_x = picker_pos.x + sv_picker_size + style.ItemInnerSpacing.x;
    //    float bar1_pos_x = bar0_pos_x + bars_width + style.ItemInnerSpacing.x;
    //    float bars_triangles_half_sz = IM_FLOOR(bars_width * 0.20f);

    //    float backup_initial_col[4];
    //    memcpy(backup_initial_col, col, components * sizeof(float));

    //    float wheel_thickness = sv_picker_size * 0.08f;
    //    float wheel_r_outer = sv_picker_size * 0.50f;
    //    float wheel_r_inner = wheel_r_outer - wheel_thickness;
    //    ImVec2 wheel_center(picker_pos.x + (sv_picker_size + bars_width) * 0.5f, picker_pos.y + sv_picker_size * 0.5f);

    //    // Note: the triangle is displayed rotated with triangle_pa pointing to Hue, but most coordinates stays unrotated for logic.
    //    float triangle_r = wheel_r_inner - (int)(sv_picker_size * 0.027f);
    //    ImVec2 triangle_pa = ImVec2(triangle_r, 0.0f); // Hue point.
    //    ImVec2 triangle_pb = ImVec2(triangle_r * -0.5f, triangle_r * -0.866025f); // Black point.
    //    ImVec2 triangle_pc = ImVec2(triangle_r * -0.5f, triangle_r * +0.866025f); // White point.

    //    float H = col[0], S = col[1], V = col[2];
    //    float R = col[0], G = col[1], B = col[2];
    //    if (flags & ImGuiColorEditFlags_InputRGB)
    //    {
    //        // Hue is lost when converting from greyscale rgb (saturation=0). Restore it.
    //        ImGui::ColorConvertRGBtoHSV(R, G, B, H, S, V);
    //        //ColorEditRestoreHS(col, &H, &S, &V);
    //    }
    //    else if (flags & ImGuiColorEditFlags_InputHSV)
    //    {
    //        ImGui::ColorConvertHSVtoRGB(H, S, V, R, G, B);
    //    }

    //    bool value_changed = false, value_changed_h = false, value_changed_sv = false;

    //    ImGui::PushItemFlag(ImGuiItemFlags_NoNav, true);
    //    if (flags & ImGuiColorEditFlags_PickerHueWheel)
    //    {
    //        // Hue wheel + SV triangle logic
    //        ImGui::InvisibleButton("hsv", ImVec2(sv_picker_size + style.ItemInnerSpacing.x + bars_width, sv_picker_size));
    //        if (ImGui::IsItemActive())
    //        {
    //            ImVec2 initial_off = ImVec2(g.IO.MouseClickedPos[0].x - wheel_center.x, g.IO.MouseClickedPos[0].y - wheel_center.y);
    //            ImVec2 current_off = ImVec2(g.IO.MousePos.x - wheel_center.x, g.IO.MousePos.y - wheel_center.y);
    //            float initial_dist2 = ImLengthSqr(initial_off);
    //            if (initial_dist2 >= (wheel_r_inner - 1) * (wheel_r_inner - 1) && initial_dist2 <= (wheel_r_outer + 1) * (wheel_r_outer + 1))
    //            {
    //                // Interactive with Hue wheel
    //                H = ImAtan2(current_off.y, current_off.x) / IM_PI * 0.5f;
    //                if (H < 0.0f)
    //                    H += 1.0f;
    //                value_changed = value_changed_h = true;
    //            }
    //            float cos_hue_angle = ImCos(-H * 2.0f * IM_PI);
    //            float sin_hue_angle = ImSin(-H * 2.0f * IM_PI);
    //            if (ImTriangleContainsPoint(triangle_pa, triangle_pb, triangle_pc, ImRotate(initial_off, cos_hue_angle, sin_hue_angle)))
    //            {
    //                // Interacting with SV triangle
    //                ImVec2 current_off_unrotated = ImRotate(current_off, cos_hue_angle, sin_hue_angle);
    //                if (!ImTriangleContainsPoint(triangle_pa, triangle_pb, triangle_pc, current_off_unrotated))
    //                    current_off_unrotated = ImTriangleClosestPoint(triangle_pa, triangle_pb, triangle_pc, current_off_unrotated);
    //                float uu, vv, ww;
    //                ImTriangleBarycentricCoords(triangle_pa, triangle_pb, triangle_pc, current_off_unrotated, uu, vv, ww);
    //                V = ImClamp(1.0f - vv, 0.0001f, 1.0f);
    //                S = ImClamp(uu / V, 0.0001f, 1.0f);
    //                value_changed = value_changed_sv = true;
    //            }
    //        }
    //        if (!(flags & ImGuiColorEditFlags_NoOptions))
    //            ImGui::OpenPopupOnItemClick("context");
    //    }
    //    else if (flags & ImGuiColorEditFlags_PickerHueBar)
    //    {
    //        // SV rectangle logic
    //        ImGui::InvisibleButton("sv", ImVec2(sv_picker_size, sv_picker_size));
    //        if (ImGui::IsItemActive())
    //        {
    //            S = ImSaturate((io.MousePos.x - picker_pos.x) / (sv_picker_size - 1));
    //            V = 1.0f - ImSaturate((io.MousePos.y - picker_pos.y) / (sv_picker_size - 1));

    //            // Greatly reduces hue jitter and reset to 0 when hue == 255 and color is rapidly modified using SV square.
    //            if (g.ColorEditLastColor == ImGui::ColorConvertFloat4ToU32(ImVec4(col[0], col[1], col[2], 0)))
    //                H = g.ColorEditLastHue;
    //            value_changed = value_changed_sv = true;
    //        }
    //        if (!(flags & ImGuiColorEditFlags_NoOptions))
    //            ImGui::OpenPopupOnItemClick("context");

    //        // Hue bar logic
    //        ImGui::SetCursorScreenPos(ImVec2(bar0_pos_x, picker_pos.y));
    //        ImGui::InvisibleButton("hue", ImVec2(bars_width, sv_picker_size));
    //        if (ImGui::IsItemActive())
    //        {
    //            H = ImSaturate((io.MousePos.y - picker_pos.y) / (sv_picker_size - 1));
    //            value_changed = value_changed_h = true;
    //        }
    //    }

    //    // Alpha bar logic
    //    if (alpha_bar)
    //    {
    //        ImGui::SetCursorScreenPos(ImVec2(bar1_pos_x, picker_pos.y));
    //        ImGui::InvisibleButton("alpha", ImVec2(bars_width, sv_picker_size));
    //        if (ImGui::IsItemActive())
    //        {
    //            col[3] = 1.0f - ImSaturate((io.MousePos.y - picker_pos.y) / (sv_picker_size - 1));
    //            value_changed = true;
    //        }
    //    }
    //    ImGui::PopItemFlag(); // ImGuiItemFlags_NoNav

    //    if (!(flags & ImGuiColorEditFlags_NoSidePreview))
    //    {
    //        ImGui::SameLine(0, style.ItemInnerSpacing.x);
    //        ImGui::BeginGroup();
    //    }

    //    if (!(flags & ImGuiColorEditFlags_NoLabel))
    //    {
    //        const char* label_display_end = ImGui::FindRenderedTextEnd(label);
    //        if (label != label_display_end)
    //        {
    //            if ((flags & ImGuiColorEditFlags_NoSidePreview))
    //                ImGui::SameLine(0, style.ItemInnerSpacing.x);
    //            ImGui::TextEx(label, label_display_end);
    //        }
    //    }

    //    if (!(flags & ImGuiColorEditFlags_NoSidePreview))
    //    {
    //        ImGui::PushItemFlag(ImGuiItemFlags_NoNavDefaultFocus, true);
    //        ImVec4 col_v4(col[0], col[1], col[2], (flags & ImGuiColorEditFlags_NoAlpha) ? 1.0f : col[3]);
    //        if ((flags & ImGuiColorEditFlags_NoLabel))
    //            ImGui::Text("Current");

    //        ImGuiColorEditFlags sub_flags_to_forward = ImGuiColorEditFlags_InputMask_ | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_AlphaPreviewHalf | ImGuiColorEditFlags_NoTooltip;
    //        ImGui::ColorButton("##current", col_v4, (flags & sub_flags_to_forward), ImVec2(square_sz * 3, square_sz * 2));
    //        if (ref_col != NULL)
    //        {
    //            ImGui::Text("Original");
    //            ImVec4 ref_col_v4(ref_col[0], ref_col[1], ref_col[2], (flags & ImGuiColorEditFlags_NoAlpha) ? 1.0f : ref_col[3]);
    //            if (ImGui::ColorButton("##original", ref_col_v4, (flags & sub_flags_to_forward), ImVec2(square_sz * 3, square_sz * 2)))
    //            {
    //                memcpy(col, ref_col, components * sizeof(float));
    //                value_changed = true;
    //            }
    //        }
    //        ImGui::PopItemFlag();
    //        ImGui::EndGroup();
    //    }

    //    // Convert back color to RGB
    //    if (value_changed_h || value_changed_sv)
    //    {
    //        if (flags & ImGuiColorEditFlags_InputRGB)
    //        {
    //            ImGui::ColorConvertHSVtoRGB(H, S, V, col[0], col[1], col[2]);
    //            g.ColorEditLastHue = H;
    //            g.ColorEditLastSat = S;
    //            g.ColorEditLastColor = ImGui::ColorConvertFloat4ToU32(ImVec4(col[0], col[1], col[2], 0));
    //        }
    //        else if (flags & ImGuiColorEditFlags_InputHSV)
    //        {
    //            col[0] = H;
    //            col[1] = S;
    //            col[2] = V;
    //        }
    //    }

    //    // R,G,B and H,S,V slider color editor
    //    bool value_changed_fix_hue_wrap = false;
    //    if ((flags & ImGuiColorEditFlags_NoInputs) == 0)
    //    {
    //        ImGui::PushItemWidth((alpha_bar ? bar1_pos_x : bar0_pos_x) + bars_width - picker_pos.x);
    //        ImGuiColorEditFlags sub_flags_to_forward = ImGuiColorEditFlags_DataTypeMask_ | ImGuiColorEditFlags_InputMask_ | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_AlphaPreviewHalf;
    //        ImGuiColorEditFlags sub_flags = (flags & sub_flags_to_forward) | ImGuiColorEditFlags_NoPicker;
    //        if (flags & ImGuiColorEditFlags_DisplayRGB || (flags & ImGuiColorEditFlags_DisplayMask_) == 0)
    //            if (ImGui::ColorEdit4("##rgb", col, sub_flags | ImGuiColorEditFlags_DisplayRGB))
    //            {
    //                // FIXME: Hackily differentiating using the DragInt (ActiveId != 0 && !ActiveIdAllowOverlap) vs. using the InputText or DropTarget.
    //                // For the later we don't want to run the hue-wrap canceling code. If you are well versed in HSV picker please provide your input! (See #2050)
    //                value_changed_fix_hue_wrap = (g.ActiveId != 0 && !g.ActiveIdAllowOverlap);
    //                value_changed = true;
    //            }
    //        if (flags & ImGuiColorEditFlags_DisplayHSV || (flags & ImGuiColorEditFlags_DisplayMask_) == 0)
    //            value_changed |= ImGui::ColorEdit4("##hsv", col, sub_flags | ImGuiColorEditFlags_DisplayHSV);
    //        if (flags & ImGuiColorEditFlags_DisplayHex || (flags & ImGuiColorEditFlags_DisplayMask_) == 0)
    //            value_changed |= ImGui::ColorEdit4("##hex", col, sub_flags | ImGuiColorEditFlags_DisplayHex);
    //        ImGui::PopItemWidth();
    //    }

    //    // Try to cancel hue wrap (after ColorEdit4 call), if any
    //    if (value_changed_fix_hue_wrap && (flags & ImGuiColorEditFlags_InputRGB))
    //    {
    //        float new_H, new_S, new_V;
    //        ImGui::ColorConvertRGBtoHSV(col[0], col[1], col[2], new_H, new_S, new_V);
    //        if (new_H <= 0 && H > 0)
    //        {
    //            if (new_V <= 0 && V != new_V)
    //                ImGui::ColorConvertHSVtoRGB(H, S, new_V <= 0 ? V * 0.5f : new_V, col[0], col[1], col[2]);
    //            else if (new_S <= 0)
    //                ImGui::ColorConvertHSVtoRGB(H, new_S <= 0 ? S * 0.5f : new_S, new_V, col[0], col[1], col[2]);
    //        }
    //    }

    //    if (value_changed)
    //    {
    //        if (flags & ImGuiColorEditFlags_InputRGB)
    //        {
    //            R = col[0];
    //            G = col[1];
    //            B = col[2];
    //            ImGui::ColorConvertRGBtoHSV(R, G, B, H, S, V);
    //            //ColorEditRestoreHS(col, &H, &S, &V);   // Fix local Hue as display below will use it immediately.
    //        }
    //        else if (flags & ImGuiColorEditFlags_InputHSV)
    //        {
    //            H = col[0];
    //            S = col[1];
    //            V = col[2];
    //            ImGui::ColorConvertHSVtoRGB(H, S, V, R, G, B);
    //        }
    //    }

    //    const int style_alpha8 = IM_F32_TO_INT8_SAT(style.Alpha);
    //    const ImU32 col_black = IM_COL32(0, 0, 0, style_alpha8);
    //    const ImU32 col_white = IM_COL32(255, 255, 255, style_alpha8);
    //    const ImU32 col_midgrey = IM_COL32(128, 128, 128, style_alpha8);
    //    const ImU32 col_hues[6 + 1] = { IM_COL32(255,0,0,style_alpha8), IM_COL32(255,255,0,style_alpha8), IM_COL32(0,255,0,style_alpha8), IM_COL32(0,255,255,style_alpha8), IM_COL32(0,0,255,style_alpha8), IM_COL32(255,0,255,style_alpha8), IM_COL32(255,0,0,style_alpha8) };

    //    ImVec4 hue_color_f(1, 1, 1, style.Alpha); ImGui::ColorConvertHSVtoRGB(H, 1, 1, hue_color_f.x, hue_color_f.y, hue_color_f.z);
    //    ImU32 hue_color32 = ImGui::ColorConvertFloat4ToU32(hue_color_f);
    //    ImU32 user_col32_striped_of_alpha = ImGui::ColorConvertFloat4ToU32(ImVec4(R, G, B, style.Alpha)); // Important: this is still including the main rendering/style alpha!!

    //    ImVec2 sv_cursor_pos;

    //    if (flags & ImGuiColorEditFlags_PickerHueWheel)
    //    {
    //        // Render Hue Wheel
    //        const float aeps = 0.5f / wheel_r_outer; // Half a pixel arc length in radians (2pi cancels out).
    //        const int segment_per_arc = ImMax(4, (int)wheel_r_outer / 12);
    //        for (int n = 0; n < 6; n++)
    //        {
    //            const float a0 = (n) / 6.0f * 2.0f * IM_PI - aeps;
    //            const float a1 = (n + 1.0f) / 6.0f * 2.0f * IM_PI + aeps;
    //            const int vert_start_idx = draw_list->VtxBuffer.Size;
    //            draw_list->PathArcTo(wheel_center, (wheel_r_inner + wheel_r_outer) * 0.5f, a0, a1, segment_per_arc);
    //            draw_list->PathStroke(col_white, 0, wheel_thickness);
    //            const int vert_end_idx = draw_list->VtxBuffer.Size;

    //            // Paint colors over existing vertices
    //            ImVec2 gradient_p0(wheel_center.x + ImCos(a0) * wheel_r_inner, wheel_center.y + ImSin(a0) * wheel_r_inner);
    //            ImVec2 gradient_p1(wheel_center.x + ImCos(a1) * wheel_r_inner, wheel_center.y + ImSin(a1) * wheel_r_inner);
    //            ImGui::ShadeVertsLinearColorGradientKeepAlpha(draw_list, vert_start_idx, vert_end_idx, gradient_p0, gradient_p1, col_hues[n], col_hues[n + 1]);
    //        }

    //        // Render Cursor + preview on Hue Wheel
    //        float cos_hue_angle = ImCos(H * 2.0f * IM_PI);
    //        float sin_hue_angle = ImSin(H * 2.0f * IM_PI);
    //        ImVec2 hue_cursor_pos(wheel_center.x + cos_hue_angle * (wheel_r_inner + wheel_r_outer) * 0.5f, wheel_center.y + sin_hue_angle * (wheel_r_inner + wheel_r_outer) * 0.5f);
    //        float hue_cursor_rad = value_changed_h ? wheel_thickness * 0.65f : wheel_thickness * 0.55f;
    //        int hue_cursor_segments = ImClamp((int)(hue_cursor_rad / 1.4f), 9, 32);
    //        draw_list->AddCircleFilled(hue_cursor_pos, hue_cursor_rad, hue_color32, hue_cursor_segments);
    //        draw_list->AddCircle(hue_cursor_pos, hue_cursor_rad + 1, col_midgrey, hue_cursor_segments);
    //        draw_list->AddCircle(hue_cursor_pos, hue_cursor_rad, col_white, hue_cursor_segments);

    //        // Render SV triangle (rotated according to hue)
    //        ImVec2 rot = ImRotate(triangle_pa, cos_hue_angle, sin_hue_angle);
    //        ImVec2 tra = ImVec2(rot.x + wheel_center.x, rot.y + wheel_center.y);
    //        rot = ImRotate(triangle_pb, cos_hue_angle, sin_hue_angle);
    //        ImVec2 trb = ImVec2(wheel_center.x + rot.x, wheel_center.y + rot.y);
    //        rot = ImRotate(triangle_pc, cos_hue_angle, sin_hue_angle);
    //        ImVec2 trc = ImVec2(wheel_center.x + rot.x, wheel_center.y + rot.y);
    //        ImVec2 uv_white = ImGui::GetFontTexUvWhitePixel();
    //        draw_list->PrimReserve(6, 6);
    //        draw_list->PrimVtx(tra, uv_white, hue_color32);
    //        draw_list->PrimVtx(trb, uv_white, hue_color32);
    //        draw_list->PrimVtx(trc, uv_white, col_white);
    //        draw_list->PrimVtx(tra, uv_white, 0);
    //        draw_list->PrimVtx(trb, uv_white, col_black);
    //        draw_list->PrimVtx(trc, uv_white, 0);
    //        draw_list->AddTriangle(tra, trb, trc, col_midgrey, 1.5f);
    //        sv_cursor_pos = ImLerp(ImLerp(trc, tra, ImSaturate(S)), trb, ImSaturate(1 - V));
    //    }
    //    else if (flags & ImGuiColorEditFlags_PickerHueBar)
    //    {
    //        // Render SV Square
    //        draw_list->AddRectFilledMultiColor(picker_pos, ImVec2(sv_picker_size + picker_pos.x, sv_picker_size + picker_pos.y), col_white, hue_color32, hue_color32, col_white);
    //        draw_list->AddRectFilledMultiColor(picker_pos, ImVec2(sv_picker_size + picker_pos.x, sv_picker_size + picker_pos.y), 0, 0, col_black, col_black);
    //        ImGui::RenderFrameBorder(picker_pos, ImVec2(sv_picker_size + picker_pos.x, sv_picker_size + picker_pos.y), 0.0f);
    //        sv_cursor_pos.x = ImClamp(IM_ROUND(picker_pos.x + ImSaturate(S) * sv_picker_size), picker_pos.x + 2, picker_pos.x + sv_picker_size - 2); // Sneakily prevent the circle to stick out too much
    //        sv_cursor_pos.y = ImClamp(IM_ROUND(picker_pos.y + ImSaturate(1 - V) * sv_picker_size), picker_pos.y + 2, picker_pos.y + sv_picker_size - 2);

    //        // Render Hue Bar
    //        for (int i = 0; i < 6; ++i)
    //            draw_list->AddRectFilledMultiColor(ImVec2(bar0_pos_x, picker_pos.y + i * (sv_picker_size / 6)), ImVec2(bar0_pos_x + bars_width, picker_pos.y + (i + 1) * (sv_picker_size / 6)), col_hues[i], col_hues[i], col_hues[i + 1], col_hues[i + 1]);
    //        float bar0_line_y = IM_ROUND(picker_pos.y + H * sv_picker_size);
    //        ImGui::RenderFrameBorder(ImVec2(bar0_pos_x, picker_pos.y), ImVec2(bar0_pos_x + bars_width, picker_pos.y + sv_picker_size), 0.0f);
    //        RenderArrowsForVerticalBar(draw_list, ImVec2(bar0_pos_x - 1, bar0_line_y), ImVec2(bars_triangles_half_sz + 1, bars_triangles_half_sz), bars_width + 2.0f, style.Alpha);
    //    }

    //    // Render cursor/preview circle (clamp S/V within 0..1 range because floating points colors may lead HSV values to be out of range)
    //    float sv_cursor_rad = value_changed_sv ? 10.0f : 6.0f;
    //    draw_list->AddCircleFilled(sv_cursor_pos, sv_cursor_rad, user_col32_striped_of_alpha, 12);
    //    draw_list->AddCircle(sv_cursor_pos, sv_cursor_rad + 1, col_midgrey, 12);
    //    draw_list->AddCircle(sv_cursor_pos, sv_cursor_rad, col_white, 12);

    //    // Render alpha bar
    //    if (alpha_bar)
    //    {
    //        float alpha = ImSaturate(col[3]);
    //        ImRect bar1_bb(bar1_pos_x, picker_pos.y, bar1_pos_x + bars_width, picker_pos.y + sv_picker_size);
    //        ImGui::RenderColorRectWithAlphaCheckerboard(draw_list, bar1_bb.Min, bar1_bb.Max, 0, bar1_bb.GetWidth() / 2.0f, ImVec2(0.0f, 0.0f));
    //        draw_list->AddRectFilledMultiColor(bar1_bb.Min, bar1_bb.Max, user_col32_striped_of_alpha, user_col32_striped_of_alpha, user_col32_striped_of_alpha & ~IM_COL32_A_MASK, user_col32_striped_of_alpha & ~IM_COL32_A_MASK);
    //        float bar1_line_y = IM_ROUND(picker_pos.y + (1.0f - alpha) * sv_picker_size);
    //        ImGui::RenderFrameBorder(bar1_bb.Min, bar1_bb.Max, 0.0f);
    //        RenderArrowsForVerticalBar(draw_list, ImVec2(bar1_pos_x - 1, bar1_line_y), ImVec2(bars_triangles_half_sz + 1, bars_triangles_half_sz), bars_width + 2.0f, style.Alpha);
    //    }

    //    ImGui::EndGroup();

    //    if (value_changed && memcmp(backup_initial_col, col, components * sizeof(float)) == 0)
    //        value_changed = false;
    //    if (value_changed)
    //        ImGui::MarkItemEdited(g.LastItemData.ID);

    //    ImGui::PopID();

    //    return value_changed;

