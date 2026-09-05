#pragma once

#include <array>
#include <string>

namespace excelsior {
	class Window;
	class PenInput;

	class AppUi {
	public:
		AppUi(Window& window, PenInput& penInput);

		void draw(std::array<float, 4>& clearColor, const std::string& topLeftTextMessage);

		[[nodiscard]]
		bool shouldQuit() const noexcept;

	private:
		// update window layout related values
		// always call before draw()
		void updateLayout();

		// Custom window chrome
		void drawWindowChrome();

		void drawTopLeftOverlay(std::array<float, 4>& clearColor, const std::string& topLeftTextMessage);

		Window& m_window;
		PenInput& m_penInput;

		float m_chromeHeight = 0.0f;
		float m_resizeBorderThickness = 5.0f;

		bool m_showDemoWindow = false;
		bool m_shouldQuit = false;
	};
}