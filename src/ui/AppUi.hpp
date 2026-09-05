#pragma once

#include <array>
#include <string>

namespace excelsior {
	class Window;
	class PenInput;

	class AppUi {
	public:
		AppUi(
			Window& window,
			PenInput& penInput,
			std::array<float, 4>& clearColor,
			bool& shouldQuit,
			std::string& overlayText
		);

		void draw();

		[[nodiscard]]
		bool shouldQuit() const noexcept;

	private:
		// update window layout related values
		// always call before draw()
		void updateLayout();

		// Custom window chrome
		void drawWindowChrome();

		void drawTopLeftOverlay();

		Window& m_window;
		PenInput& m_penInput;

		std::array<float, 4>& m_clearColor;
		bool& m_shouldQuit;
		std::string& m_overlayText;

		float m_chromeHeight = 0.0f;
		float m_resizeBorderThickness = 5.0f;

		bool m_showDemoWindow = false;
	};
}