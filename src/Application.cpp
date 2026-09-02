#include "Application.hpp"

Application::Application() {
	SDL_SetAppMetadata("Excelsior", "0.1", "com.github.laughingclouds");

	// print which render drivers are available
	SDL_Log("Available renderer drivers:");
	for (int i = 0; i < SDL_GetNumRenderDrivers(); i++) {
		SDL_Log("%d. %s", i + 1, SDL_GetRenderDriver(i));
	}

	if (!SDL_InitSubSystem(SDL_INIT_VIDEO)) {
		throw std::runtime_error(std::format("Couldn't initialize SDL: {}", SDL_GetError()));
	}

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	if (!SDL_CreateWindowAndRenderer("Excelsior", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer)) {
		throw std::runtime_error(std::format("Couldn't create window/renderer: {}", SDL_GetError()));
	}

	m_window.reset(window);
	m_renderer.reset(renderer);

	int w = 0, h = 0;
	SDL_GetRenderOutputSize(m_renderer.get(), &w, &h);

	// make render target match output size so drawing matches pen's position on tablet displays
	m_renderTarget.reset(SDL_CreateTexture(
		m_renderer.get(),
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET,
		w, h
	));

	if (!m_renderTarget) {
		throw std::runtime_error(std::format("Couldn't create render target: {}", SDL_GetError()));
	}

	// blank the render target to gray to start
	SDL_SetRenderTarget(m_renderer.get(), m_renderTarget.get());
	SDL_SetRenderDrawColor(m_renderer.get(), 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(m_renderer.get());
	SDL_SetRenderTarget(m_renderer.get(), nullptr);
	SDL_SetRenderDrawBlendMode(m_renderer.get(), SDL_BLENDMODE_BLEND);

	SDL_SetLogPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_DEBUG);

	m_topLeftTextMessage = getMsg();
}

SDL_AppResult Application::handleEvent(const SDL_Event& event) {
	if (event.type == SDL_EVENT_QUIT) {
		return SDL_APP_SUCCESS;
	}

	if (event.type == SDL_EVENT_PEN_MOTION) {
		if (m_pressure > 0.0f) {
			if (m_previous_touchX >= 0.0f) { // only draw if we're moving while touching
				m_topLeftTextMessage = "Writing";
				SDL_SetRenderTarget(m_renderer.get(), m_renderTarget.get());
				SDL_SetRenderDrawColorFloat(m_renderer.get(), 1.0f, 1.0f, 1.0f, m_pressure);
				SDL_RenderLine(m_renderer.get(), m_previous_touchX, m_previous_touchY, event.pmotion.x, event.pmotion.y);
			}

			m_previous_touchX = event.pmotion.x;
			m_previous_touchY = event.pmotion.y;
		}
		else {
			m_previous_touchX = m_previous_touchY = -1.0f;
		}
	}
	else if (event.type == SDL_EVENT_PEN_AXIS) {
		if (event.paxis.axis == SDL_PEN_AXIS_PRESSURE) {
			const float rawPressure = event.paxis.value;

			m_pressure = (rawPressure == 0.0f) ? 0.0f : std::min(rawPressure + PRESSURE_OFFSET, 1.0f);

			m_topLeftTextMessage = std::format(
				"Actual pressure: {}, offset: {}, New Pressure: {}",
				rawPressure, PRESSURE_OFFSET, m_pressure
			);
		}
		else if (event.paxis.axis == SDL_PEN_AXIS_XTILT) {
			m_tiltX = event.paxis.value;
		}
		else if (event.paxis.axis == SDL_PEN_AXIS_YTILT) {
			m_tiltY = event.paxis.value;
		}
	}

	return SDL_APP_CONTINUE;
}

SDL_AppResult Application::update() {
	// make sure we're drawing to window and not render target
	SDL_SetRenderTarget(m_renderer.get(), nullptr);
	// non-drawing surface to be gray to distinguish
	SDL_SetRenderDrawColor(m_renderer.get(), 100, 100, 100, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(m_renderer.get());

	SDL_RenderTexture(m_renderer.get(), m_renderTarget.get(), nullptr, nullptr);
	SDL_RenderDebugText(m_renderer.get(), 0, 8, m_topLeftTextMessage.c_str());
	SDL_RenderPresent(m_renderer.get());

	return SDL_APP_CONTINUE;
}