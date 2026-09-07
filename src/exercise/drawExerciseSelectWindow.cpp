#include "Exercise.hpp"

#include "ui/AppWorkArea.hpp"
#include "ui/imgui_raii.hpp"
#include "ui/utils.hpp"

#include <imgui.h>

namespace excelsior::exercise {

	void Exercise::drawExerciseSelectWindow(const AppWorkArea workArea) {
		const float LEFT_PAD = 10.0f;
		ImVec2 work_pos = workArea.pos;
		ImVec2 work_size = workArea.size;

		ImVec2 window_pos, window_pos_pivot;

		// left most edge
		window_pos.x = work_pos.x + LEFT_PAD;
		window_pos.y = work_pos.y + work_size.y * 0.5f;

		// pivot around left center edge of window
		ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, ImVec2(0.0f, 0.5f));

		ImGuiWindowFlags flags = ImGuiWindowFlags_None;
		flags |= ImGuiWindowFlags_NoMove;
		flags |= ImGuiWindowFlags_NoCollapse;
		flags |= ImGuiWindowFlags_NoSavedSettings;
		//flags |= ImGuiWindowFlags_NoTitleBar;
		flags |= ImGuiWindowFlags_NoResize;
		flags |= ImGuiWindowFlags_AlwaysAutoResize;

		if (ImGui::Begin("Rendering", nullptr, flags)) {
			ui::spacerY();
			ImGui::TextUnformatted("Exercise");
			ImGui::RadioButton("Triangle (Default)", (int*)&m_type, (int)Type::DEFAULT_TRIANGLE);
			ImGui::RadioButton("Rectangle", (int*)&m_type, (int)Type::RECTANGLE);
			ImGui::RadioButton("Two Triangles", (int*)&m_type, (int)Type::TWO_TRIANGLES);
			ImGui::RadioButton("Two Triangles (Different Color)", (int*)&m_type, (int)Type::TWO_TRIANGLES_DIFF_COL);
			ui::spacerY();
			if (ImGui::Checkbox("Wireframe", &m_isPolygonMode)) {
				setWireframe();
			}
			ui::spacerY();

			{
				UI_GROUP();

				ImGui::TextUnformatted("Shader");
				
				if (ImGui::Button("Orange")) {
					m_primaryFragColor = orangeColVec;
					m_secondaryFragColor = yellowColVec;
				}

				ImGui::SameLine();

				if (ImGui::Button("Yellow")) {
					m_primaryFragColor = yellowColVec;
					m_secondaryFragColor = orangeColVec;
				}
			}

			ImGui::SameLine(0.0f, 20.0f); // horizontal gap of 20px

			assert(m_primaryFragColor != nullptr);
			assert(m_secondaryFragColor != nullptr);

			{
				UI_GROUP();
				ui::undecoratedLink("Fragment shader");
				ui::coloredDummyGroup("Primary", ImVec2(20.0f, 20.0f), *(const ImVec4*)m_primaryFragColor);
				ImGui::SameLine(0.0f, 20.0f);
				ui::coloredDummyGroup("Secondary", ImVec2(20.0f, 20.0f), *(const ImVec4*)m_secondaryFragColor);
			}
		}
		ImGui::End();
	}
}