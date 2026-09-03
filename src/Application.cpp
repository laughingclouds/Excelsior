#include <glad/gl.h>

#include "Application.hpp"
#include "Stroke.hpp"

#include <algorithm>
#include <format>
#include <stdexcept>

#include <SDL3/SDL.h>

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

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	m_window = std::shared_ptr<SDL_Window>(
		SDL_CreateWindow("Excelsior", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL),
		SDL_DestroyWindow
	);
	if (!m_window) {
		throw std::runtime_error(std::format("Couldn't create window/renderer: {}", SDL_GetError()));
	}

	std::shared_ptr<SDL_Window> windowRef = m_window;

	m_glContext = std::unique_ptr<SDL_GLContextState, std::function<void(SDL_GLContext)>>(
		SDL_GL_CreateContext(windowRef.get()),
		[windowRef](SDL_GLContext c) {
			if (c) {
				// windowRef keeps SDL_Window alive
				SDL_GL_MakeCurrent(windowRef.get(), nullptr);
				SDL_GL_DestroyContext(c);
			}
		}
	);
	//m_glContext.reset(SDL_GL_CreateContext(m_window.get()));
	if (!m_glContext) {
		throw std::runtime_error(std::format("Couldn't create GL Context for window: {}", SDL_GetError()));
	}

	int version = gladLoadGL((GLADloadfunc) SDL_GL_GetProcAddress);
	if (!version) {
		throw std::runtime_error(std::format("Couldn't initialize OpenGL Context"));
	}

	// set framebuffer width, height
	int w = 0, h = 0;
	SDL_GetWindowSizeInPixels(m_window.get(), &w, &h);
	glViewport(0, 0, w, h);

	SDL_Log("GL v%d.%d Initialized", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

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
				//SDL_SetRenderTarget(m_renderer.get(), m_renderTarget.get());
				//SDL_SetRenderDrawColorFloat(m_renderer.get(), 1.0f, 1.0f, 1.0f, m_pressure);
				//SDL_RenderLine(m_renderer.get(), m_previous_touchX, m_previous_touchY, event.pmotion.x, event.pmotion.y);
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
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	SDL_GL_SwapWindow(m_window.get());

	return SDL_APP_CONTINUE;
}