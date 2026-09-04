#include "AppWorkArea.hpp"

#include <algorithm>

namespace excelsior {
	AppWorkArea getAppWorkArea(float chromeHeight) {
		const ImGuiViewport* viewport = ImGui::GetMainViewport();

		AppWorkArea workArea{
			.pos = viewport->WorkPos,
			.size = viewport->WorkSize
		};

		workArea.pos.y += chromeHeight;
		workArea.size.y = std::max(0.0f, workArea.size.y - chromeHeight);

		return workArea;
	}
}