#include "AppUi.hpp"
#include "AppWorkArea.hpp"

#include "Window.hpp"
#include "PenInput.hpp"

#ifdef ENABLE_EXERCISE
#include "exercise/Exercise.hpp"
#endif

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
		const AppWorkArea workArea = getAppWorkArea(m_chromeHeight);

		drawDebugOverlay(workArea);
		
		#ifdef ENABLE_EXERCISE
		if (m_showExercise)
			exercise::ex.draw(workArea);
		#endif

		if (m_showDemoWindow)
			ImGui::ShowDemoWindow(&m_showDemoWindow);

	}

	void AppUi::updateLayout() {
		m_chromeHeight = ImGui::GetFrameHeight();
	}
}