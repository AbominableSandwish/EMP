#include <tool/hierarchy.h>
#include <core/engine.h>
#include <core/entity.h>
#include <core/log.h>l
#include <tool/inspector.h>

	void Hierarchy::Init()
	{
		m_manager = this->m_engine->GetEntityManager();
	}

	Hierarchy::Hierarchy(emp::Engine& engine, std::string name) : Tool(engine, name)
	{
	}

	void Hierarchy::Update(float)
	{
	}

	void Hierarchy::Draw()
	{
		if (ImGui::BeginMenuBar())
		{
			ImGui::EndMenuBar();
		}

		if (!is_select) {
			from = -1;
			to_move = -1;
		}
		is_select = false;
		auto entities = m_manager->GetEntities();
		if (entities.size() != 0) {
			int i = 0;
			for (auto entity : entities)
			{
				i++;
				std::string name = "##" + std::to_string(entity->id);
				ImGui::SetCursorPos(ImVec2(10, i * 18 + 24));
				if (ImGui::TreeNode(name.c_str()))
				{
					ImGui::SameLine();
					if (ImGui::Selectable(entity->GetName().c_str()))
					{
						emp::LOG::Debug("Click");
					}
					ImGuiDragDropFlags src_flags = 0;
					src_flags |= ImGuiDragDropFlags_SourceNoDisableHover;     // Keep the source displayed as hovered
					src_flags |= ImGuiDragDropFlags_SourceNoHoldToOpenOthers; // Because our dragging is local, we disable the feature of opening foreign treenodes/tabs while dragging
					//src_flags |= ImGuiDragDropFlags_SourceNoPreviewTooltip; // Hide the tooltip
					if (ImGui::BeginDragDropSource(src_flags))
					{
						ImGui::SetDragDropPayload("spore", entity->GetName().c_str(), sizeof(const char*));
						ImGui::TextUnformatted(entity->GetName().c_str());
						ImGui::EndDragDropSource();

					}
					if (ImGui::BeginDragDropTarget())
					{
						ImGuiDragDropFlags target_flags = 0;
						target_flags |= ImGuiDragDropFlags_AcceptBeforeDelivery;    // Don't wait until the delivery (release mouse button on a target) to do something
						target_flags |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect; // Don't display the yellow rectangle
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("spore", target_flags))
						{
							//move_from = *(const int*)payload->Data;
							//move_to = n;

						}
						ImGui::EndDragDropTarget();
					}
					else
					{
						emp::LOG::Debug("DROP");
					}

					//ImGui::Indent();
					//ImGui::Selectable("Spore_A");
					//ImGui::Selectable("Spore_B");
					//ImGui::Selectable("Spore_C");
					//ImGui::Unindent();

					ImGui::TreePop();
				}
				else
				{
					ImGui::SameLine();
					if (modif == i && rename) {
						std::string name = entity->GetName();

						int bufSize = 32;
						char buffer[32] = {};
						int c = 0;
						for (auto character : name)
						{
							buffer[c] = character;
							c++;
						}


						if (ImGui::InputText("##text1", buffer, bufSize, ImGuiInputTextFlags_EnterReturnsTrue))
						{
							emp::Entity* entity = entities[i - 1];
							entity->SetName(buffer);
							rename = false;
							modif = -1;
						}
						ImGui::SetKeyboardFocusHere(0);
					}
					else
					{
						if (ImGui::Selectable(entity->GetName().c_str()))
						{
							emp::LOG::Debug("Click");
							int id = entity->id;
							if (Inspector::GetInstance() != nullptr) {
								Inspector::GetInstance()->SetTarget(id);
							}
						}
					}
					if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(1))
						ImGui::OpenPopup(("context_menu" + entity->GetName()).c_str());

					if (ImGui::BeginPopup(("context_menu" + entity->GetName()).c_str())) {
						if (ImGui::Selectable("Rename"))
						{
							rename = true;
							modif = i;

						}
						if (ImGui::Selectable("Remove"))
						{
							this->m_manager->RemoveEntity(i - 1);
						}
						ImGui::EndPopup();
					}


					if (ImGui::BeginDragDropSource())
					{
						is_select = true;
						ImGui::SetDragDropPayload("spore", &i, sizeof(int));
						from = i;
						ImGui::TextUnformatted(entity->GetName().c_str());
						ImGui::EndDragDropSource();
						ImVec2 canvas = ImGui::GetWindowSize();
						if (ImGui::GetMousePos().y - ImGui::GetWindowPos().y - 1 > 40 && ImGui::GetMousePos().y - ImGui::GetWindowPos().y - 1 < 22 + (entities.size() + 1) * 18)
						{
							ImGui::SetCursorPos(ImVec2(10, ((int)((ImGui::GetMousePos().y - ImGui::GetWindowPos().y) / 18)) * 18 + 4));
							ImGui::Button("btn", ImVec2(canvas.x, 2));
							emp::LOG::Debug(std::to_string((int)((ImGui::GetMousePos().y - ImGui::GetWindowPos().y - 40) + 9) / 18));
						}
					}

					if (ImGui::BeginDragDropTarget())
					{
						ImGuiDragDropFlags target_flags = 0;
						target_flags |= ImGuiDragDropFlags_AcceptBeforeDelivery;    // Don't wait until the delivery (release mouse button on a target) to do something
						target_flags |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect; // Don't display the yellow rectangle
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("spore", target_flags))
						{
							from = *(const int*)payload->Data;
							to_move = i;
						}

						ImGui::EndDragDropTarget();
					}
				}
			}

			//emp::LOG::Debug("from Id: " + std::to_string(from));
			//emp::LOG::Debug("move Id : " + std::to_string(to_move));
			if (from != -1 && to_move != -1 && is_select == false)
			{
				emp::LOG::Debug("Action Move Spore: From to: " + std::to_string(from) + ", Move to: " + std::to_string(to_move));
				//memmove(&names[copy_dst], &names[copy_src], (size_t)copy_count * sizeof(const char*));
				//names[to_move] = tmp;
				m_manager->MoveEntity(from - 1, to_move - 1);
				//ImGui::SetDragDropPayload("spore", &to_move, sizeof(int)); // Update payload immediately so on the next frame if we move the mouse to an earlier item our index payload will be correct. This is odd and showcase how the DnD api isn't best presented in this example.
			}
		}
	}

	void Hierarchy::Destroy()
	{
	}

