#include "AppUi.hpp"
#include "Window.hpp"

#include <imgui.h>

namespace excelsior {
	void AppUi::drawWindowChrome() {
		const ImGuiViewport* viewport = ImGui::GetMainViewport();

		const float btnWidth = m_chromeHeight * 2.5;
		const float btnHeight = m_chromeHeight;
		const float captionButtonsWidth = 3.0f * btnWidth;

		// native window behavior using chrome dimensions
		m_window.configureChromeHitTest(m_chromeHeight, captionButtonsWidth, m_resizeBorderThickness);

		// strip padding, border, minimum restriction
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(0.0f, 0.0f));

		ImGui::SetNextWindowPos(viewport->Pos, ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, m_chromeHeight), ImGuiCond_Always);

		ImGui::Begin("Excelsior###MainWindowChrome",
			nullptr,
			ImGuiWindowFlags_NoDecoration |
			ImGuiWindowFlags_NoMove |
			//ImGuiWindowFlags_NoBackground |
			ImGuiWindowFlags_NoSavedSettings
		);

		// Title bar
		const float textLineHeight = ImGui::GetTextLineHeight();
		const float textY = (m_chromeHeight - textLineHeight) * 0.5f;
		ImGui::SetCursorPos(ImVec2(
			8.0f, // space on left side of text
			textY // Y coordinate at middle of imgui window
		));
		ImGui::TextUnformatted("Excelsior");

		// Window buttons

		// right most edge of current window (window chrome)
		const float right = ImGui::GetWindowWidth();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f)); // invisible bg

		ImGui::SetCursorPos(ImVec2(right - 3.0f * btnWidth, 0.0f));
		if (ImGui::Button("_###MinimizeApp", ImVec2(btnWidth, btnHeight))) {
			m_window.minimize();
		}

		ImGui::SetCursorPos(ImVec2(right - 2.0f * btnWidth, 0.0f));
		if (ImGui::Button("[]###MaximizeApp", ImVec2(btnWidth, btnHeight))) {
			m_window.toggleMaximize();
		}

		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.0f, 0.0f, 0.70f)); // red on hover
		ImGui::SetCursorPos(ImVec2(right - btnWidth, 0.0f));
		if (ImGui::Button("X###QuitApp", ImVec2(btnWidth, btnHeight))) {
			m_shouldQuit = true;
		}

		ImGui::PopStyleColor(2);

		ImGui::End();
		ImGui::PopStyleVar(3);
	}
}