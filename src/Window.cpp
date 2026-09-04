#include "Window.hpp"

#include <format>
#include <stdexcept>

#include <glad/gl.h>
#include <SDL3/SDL.h>

namespace excelsior {

	void Window::SDLWindowDeleter::operator()(SDL_Window* w) const noexcept { if (w) SDL_DestroyWindow(w); }
	void Window::GLContextDeleter::operator()(SDL_GLContextState* c) const noexcept { if (c) SDL_GL_DestroyContext(c); }

	Window::Window(const char* title, int width, int height) {
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		// following https://github.com/ocornut/imgui/blob/master/examples/example_sdl3_opengl3/main.cpp
		// create window with graphics context
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

		m_contentScale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());

		if (m_contentScale <= 0.0f)
			m_contentScale = 1.0f;

		// Known issue with borderless
		// https://github.com/libsdl-org/SDL/issues/12791
		// SDL3: OpenGL Borderless Window Becomes Exclusive Fullscreen on SDL_GL_SwapWindow (Windows 11)
		const SDL_WindowFlags flags =
			SDL_WINDOW_OPENGL |
			SDL_WINDOW_RESIZABLE |
			SDL_WINDOW_HIDDEN |
			//SDL_WINDOW_BORDERLESS |
			SDL_WINDOW_HIGH_PIXEL_DENSITY;

		m_window.reset(
			SDL_CreateWindow(
				title,
				static_cast<int>(width * m_contentScale),
				static_cast<int>(height * m_contentScale),
				flags
			)
		);
		if (!m_window) {
			throw std::runtime_error(std::format("Couldn't create window: {}", SDL_GetError()));
		}

		m_glContext.reset(SDL_GL_CreateContext(m_window.get()));
		if (!m_glContext) {
			throw std::runtime_error(std::format("Couldn't create OpenGL context: {}", SDL_GetError()));
		}

		if (!SDL_GL_MakeCurrent(m_window.get(), m_glContext.get())) {
			throw std::runtime_error(std::format("Couldn't make OpenGL context current: {}", SDL_GetError()));
		}

		// can initialize glad after GL context created
		const int version = gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress);
		if (!version) {
			throw std::runtime_error(std::format("Couldn't initialize GLAD"));
		}
		SDL_Log("OpenGL v%d.%d Initialized", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

		SDL_GL_SetSwapInterval(1); // enable vsync
		SDL_SetWindowPosition(m_window.get(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
		SDL_ShowWindow(m_window.get()); // since we initialized in hidden state
	}

	Window::~Window() {
		// context should stop being current before it is destroyed
		if (m_window && m_glContext)
			SDL_GL_MakeCurrent(m_window.get(), nullptr);

		// members will be destroyed in reverse declaration order
		// m_glContext
		// m_window
	}

	bool Window::minimize() {
		return SDL_MinimizeWindow(m_window.get());
	}

	bool Window::maximize() {
		return SDL_MaximizeWindow(m_window.get());
	}

	bool Window::restore() {
		return SDL_RestoreWindow(m_window.get());
	}

	SDL_Window* Window::nativeHandle() const noexcept {
		return m_window.get();
	}

	SDL_GLContext Window::glContext() const noexcept {
		return m_glContext.get();
	}

	float Window::contentScale() const noexcept {
		return m_contentScale;
	}

	bool Window::isMinimized() const noexcept {
		return SDL_GetWindowFlags(m_window.get()) & SDL_WINDOW_MINIMIZED;
	}

	bool Window::isMaximized() const noexcept {
		return SDL_GetWindowFlags(m_window.get()) & SDL_WINDOW_MAXIMIZED;
	}

	std::pair<int, int> Window::framebufferSize() const noexcept {
		int width = 0, height = 0;

		SDL_GetWindowSizeInPixels(m_window.get(), &width, &height);

		return { width, height };
	}

	void Window::swapBuffers() const {
		SDL_GL_SwapWindow(m_window.get());
	}

	bool Window::toggleMaximize() {
		if (isMaximized())
			return restore();
		return maximize();
	}
}