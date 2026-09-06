#include "utils.hpp"
#include "imgui_raii.hpp"

#include <imgui.h>

namespace excelsior::ui {
	void spacerY(float height) {
		ImGui::Dummy(ImVec2(0.0f, height));
	}

	void spacerX(float width) {
		ImGui::Dummy(ImVec2(width, 0.0f));
	}

	void coloredDummy(const char* text, const ImVec2& size, ImVec4 col) {
		if (text) {
			ImGui::TextUnformatted(text);
		}

		ImGui::Dummy(size);

		ImGui::GetWindowDrawList()->AddRectFilled(
			ImGui::GetItemRectMin(),
			ImGui::GetItemRectMax(),
			ImGui::ColorConvertFloat4ToU32(col),
			2.0f, ImDrawFlags_RoundCornersAll);
	}

	// scoped under group
	void coloredDummyGroup(const char* text, const ImVec2& size, ImVec4 col) {
		UI_GROUP();
		coloredDummy(text, size, col);
	}

	void undecoratedLink(const char* label) {
		ImGui::PushStyleColor(ImGuiCol_TextLink, ImGui::GetStyleColorVec4(ImGuiCol_Text));
		ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0, 0, 0, 0));

		ImGui::TextLink(label);

		if (ImGui::IsItemHovered())
			ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);

		ImGui::PopStyleColor(3);
	}
}