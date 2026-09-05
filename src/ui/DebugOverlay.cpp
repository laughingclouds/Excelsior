#include "AppUi.hpp"
#include "AppWorkArea.hpp"

#include "PenInput.hpp"

#include <imgui.h>

namespace excelsior {
	void AppUi::drawDebugOverlay(const AppWorkArea workArea) {
		static int location = 0;
		ImGuiIO& io = ImGui::GetIO();
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
		if (location >= 0)
		{
			const float PAD = 10.0f;
			ImVec2 work_pos = workArea.pos; // Use work area to avoid menu-bar/task-bar, if any!
			ImVec2 work_size = workArea.size;
			ImVec2 window_pos, window_pos_pivot;
			window_pos.x = (location & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
			window_pos.y = (location & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
			window_pos_pivot.x = (location & 1) ? 1.0f : 0.0f;
			window_pos_pivot.y = (location & 2) ? 1.0f : 0.0f;

			ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
			window_flags |= ImGuiWindowFlags_NoMove;
		}
		else if (location == -2)
		{
			// Center window
			ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
			window_flags |= ImGuiWindowFlags_NoMove;
		}
		ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
		if (ImGui::Begin("Top Left Overlay", nullptr, window_flags))
		{
			ImGui::ColorEdit3("Clear color", m_clearColor.data());

			ImGui::TextUnformatted(m_overlayText.c_str());
			ImGui::SameLine();
			ImGui::Checkbox("Demo Window", &m_showDemoWindow);
			ImGui::Separator();

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

			if (ImGui::IsMousePosValid())
				ImGui::Text("Mouse Position: (%.1f, %.1f)", io.MousePos.x, io.MousePos.y);
			else
				ImGui::TextUnformatted("Mouse Position: <invalid>");

			const PenState& pen = m_penInput.state();
			if (pen.x != -1.0f)
				ImGui::Text("Pen Position: (%.1f, %.1f)", pen.x, pen.y);

			if (pen.pressure > 0.0f)
				ImGui::Text("Pressure: %.3f", pen.pressure);

			if (pen.tiltX > 0.0f || pen.tiltY > 0.0f)
				ImGui::Text("TiltX: %.1f, TiltY: %.1f", pen.tiltX, pen.tiltY);

			if (ImGui::BeginPopupContextWindow())
			{
				if (ImGui::MenuItem("Custom", NULL, location == -1)) location = -1;
				if (ImGui::MenuItem("Center", NULL, location == -2)) location = -2;
				if (ImGui::MenuItem("Top-left", NULL, location == 0)) location = 0;
				if (ImGui::MenuItem("Top-right", NULL, location == 1)) location = 1;
				if (ImGui::MenuItem("Bottom-left", NULL, location == 2)) location = 2;
				if (ImGui::MenuItem("Bottom-right", NULL, location == 3)) location = 3;
				ImGui::EndPopup();
			}
		}
		ImGui::End();
	}
}