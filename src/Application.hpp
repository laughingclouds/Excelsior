#pragma once

#include "ImGuiLayer.hpp"
#include "PenInput.hpp"
#include "Window.hpp"

#include <array>
#include <string>

#include <SDL3/SDL.h>

namespace excelsior {

	class Application {
	public:
		static constexpr int WINDOW_WIDTH = 1280;
		static constexpr int WINDOW_HEIGHT = 800;
	
		/* Initialize Application + (window, renderer, texture)
		To be called in SDL_AppInit*/
		Application();

		// Call within SDL_AppEvent
		SDL_AppResult handleEvent(const SDL_Event& event);

		// Call within SDL_AppIterate
		SDL_AppResult update();

	private:
		// update window layout related values
		// always call before drawUi()
		void updateLayout();

		void drawUi();

		// Custom window chrome
		void drawWindowChrome();

		void showTopLeftOverlay(bool* p_open = nullptr);
		
		Window m_window;
		ImGuiLayer m_imgui;
		PenInput m_penInput;

		std::array<float, 4> m_clearColor{
			0.45f,	// r
			0.55f,	// g
			0.60f,	// b
			1.00f	// a
		};

		bool m_showDemoWindow = true;

		bool m_shouldQuit = false;

		std::string m_topLeftTextMessage;

		float m_chromeHeight = 0.0f;
		const float m_resizeBorderThickness = 6.0f;
	};

}