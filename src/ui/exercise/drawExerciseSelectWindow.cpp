#include "Exercise.hpp"

#include "ui/AppWorkArea.hpp"
#include <imgui.h>

namespace excelsior {
	namespace exercise {
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
				ImGui::TextUnformatted("Well LMAO");
				//ImGui::BeginListBox("List");
				//ImGui::ListBox("List");
				//ImGui::EndListBox();
			}
			ImGui::End();
		}
	}
}