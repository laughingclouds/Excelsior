#pragma once

#include <imgui.h>

namespace excelsior {
	struct AppWorkArea {
		ImVec2 pos;
		ImVec2 size;
	};

	AppWorkArea getAppWorkArea(float chromeHeight);
}