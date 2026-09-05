#include "AppUi.hpp"
#include "AppWorkArea.hpp"

#include "Window.hpp"
#include "PenInput.hpp"

namespace excelsior {

	AppUi::AppUi(
		Window& window,
		PenInput& penInput,
		std::array<float, 4>& clearColor,
		bool& shouldQuit,
		std::string& overlayText
	)
		:
		m_window(window),
		m_penInput(penInput),
		m_clearColor(clearColor),
		m_shouldQuit(shouldQuit),
		m_overlayText(overlayText)
	{
	}

	void AppUi::draw() {
		updateLayout();
		drawWindowChrome();

		if (m_showDemoWindow)
			ImGui::ShowDemoWindow(&m_showDemoWindow);

		const AppWorkArea workArea = getAppWorkArea(m_chromeHeight);

		drawDebugOverlay(workArea);
	}

	void AppUi::updateLayout() {
		m_chromeHeight = ImGui::GetFrameHeight();
	}
}