#include <glad/gl.h>

#include "Application.hpp"
#include "Stroke.hpp"

#include <algorithm>
#include <format>
#include <stdexcept>

#include <SDL3/SDL.h>

void SDLWindowDeleter::operator()(SDL_Window* w) const { if (w) SDL_DestroyWindow(w); }
void GLContextDeleter::operator()(SDL_GLContext c) const {
	if (c) {
		SDL_GL_MakeCurrent(SDL_GL_GetCurrentWindow(), nullptr);
		SDL_GL_DestroyContext(c);
	}
}

Application::Application() {
	SDL_SetAppMetadata("Excelsior", "0.1", "com.github.laughingclouds");

	m_topLeftTextMessage = getMsg();

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

	// following https://github.com/ocornut/imgui/blob/master/examples/example_sdl3_opengl3/main.cpp
	// create window with graphics context
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	
	float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
	
	SDL_WindowFlags window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY;

	m_window.reset(SDL_CreateWindow("Excelsior", (int) (WINDOW_WIDTH * main_scale), (int) (WINDOW_HEIGHT * main_scale), window_flags));
	if (!m_window) {
		throw std::runtime_error(std::format("Couldn't create window/renderer: {}", SDL_GetError()));
	}

	m_glContext.reset(SDL_GL_CreateContext(m_window.get()));
	if (!m_glContext) {
		throw std::runtime_error(std::format("Couldn't create GL Context for window: {}", SDL_GetError()));
	}

	SDL_GL_MakeCurrent(m_window.get(), m_glContext.get());
	SDL_GL_SetSwapInterval(1); // Enable vsync
	SDL_SetWindowPosition(m_window.get(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	SDL_ShowWindow(m_window.get());

	int version = gladLoadGL((GLADloadfunc) SDL_GL_GetProcAddress);
	if (!version) {
		throw std::runtime_error(std::format("Couldn't initialize OpenGL Context"));
	}

	SDL_Log("GL v%d.%d Initialized", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));


	SDL_SetLogPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_DEBUG);

	// gray background
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
}

SDL_AppResult Application::handleEvent(const SDL_Event& event) {
	switch (event.type) {
	case SDL_EVENT_QUIT:
		return SDL_APP_SUCCESS;

	case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED: {
		m_fbWidth = event.window.data1;
		m_fbHeight = event.window.data2;
		glViewport(0, 0, m_fbWidth, m_fbHeight);
		break;
	}
	case SDL_EVENT_WINDOW_EXPOSED:
		this->update(); break;
	case SDL_EVENT_PEN_MOTION:
		this->procesPenMotion(event); break;
	case SDL_EVENT_PEN_AXIS:
		this->processPenAxis(event); break;
	}

	return SDL_APP_CONTINUE;
}

SDL_AppResult Application::update() {
	Uint32 flags = SDL_GetWindowFlags(m_window.get());
	if (flags & SDL_WINDOW_MINIMIZED) {
		SDL_Delay(10); // Throttle loop
		return SDL_APP_CONTINUE;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	SDL_GL_SwapWindow(m_window.get());

	return SDL_APP_CONTINUE;
}

void Application::procesPenMotion(const SDL_Event& event) {
	if (m_pressure > 0.0f) {
		if (m_previous_touchX >= 0.0f) { // only draw if we're moving while touching
			m_topLeftTextMessage = "Writing";
			SDL_Log(m_topLeftTextMessage.c_str());
		}
		m_previous_touchX = event.pmotion.x;
		m_previous_touchY = event.pmotion.y;
	}
	else {
		m_previous_touchX = m_previous_touchY = -1.0f;
	}
}

void Application::processPenAxis(const SDL_Event& event) {
	switch (event.paxis.axis) {
	case SDL_PEN_AXIS_PRESSURE: {
		const float rawPressure = event.paxis.value;

		m_pressure = (rawPressure == 0.0f) ? 0.0f : std::min(rawPressure + PRESSURE_OFFSET, 1.0f);

		m_topLeftTextMessage = std::format(
			"Actual pressure: {}, offset: {}, New Pressure: {}",
			rawPressure, PRESSURE_OFFSET, m_pressure
		);
		break;
	}
	case SDL_PEN_AXIS_XTILT:
		m_tiltX = event.paxis.value; break;
	case SDL_PEN_AXIS_YTILT:
		m_tiltY = event.paxis.value; break;
	}
}