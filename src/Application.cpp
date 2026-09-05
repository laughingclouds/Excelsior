#include <glad/gl.h>

#include "Application.hpp"
#include "Stroke.hpp"

#include "ui/AppWorkArea.hpp"

#include <algorithm>
#include <format>

#include <imgui.h>


namespace excelsior {

	Application::Application()
		: 
		m_window("Excelsior",
			WINDOW_WIDTH,
			WINDOW_HEIGHT
		),
		m_imgui(m_window),
		m_penInput(),
		m_ui(m_window, m_penInput),
		m_overlayTextMessage(getMsg())
	{
	}

	SDL_AppResult Application::handleEvent(const SDL_Event& event) {
		m_imgui.processEvent(event);
		m_penInput.processEvent(event);

		switch (event.type) {
		case SDL_EVENT_QUIT:
		case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
			return SDL_APP_SUCCESS;

		case SDL_EVENT_WINDOW_EXPOSED:
			return update();

		case SDL_EVENT_PEN_MOTION:
			if (m_penInput.isDrawing()) m_overlayTextMessage = "Writing";
			else m_overlayTextMessage = "Idle";
			break;
		}

		return SDL_APP_CONTINUE;
	}

	SDL_AppResult Application::update() {
		if (m_window.isMinimized()) {
			SDL_Delay(10); // Throttle loop
			return SDL_APP_CONTINUE;
		}

		m_imgui.beginFrame();
		
		m_ui.draw(
			m_clearColor,
			m_overlayTextMessage
		);

		m_imgui.render(m_clearColor);
		m_window.swapBuffers();

		if (m_ui.shouldQuit()) {
			return SDL_APP_SUCCESS;
		}

		return SDL_APP_CONTINUE;
	}
}