#pragma once

#include <memory>
#include <utility>

#include <SDL3/SDL_video.h>

namespace excelsior {
class Window {
public:
	Window(const char* title, int width, int height);
	~Window();

	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	Window(Window&&) = delete;
	Window& operator=(Window&&) = delete;

	bool minimize();
	bool maximize();
	bool restore();
	
	[[nodiscard]]
	SDL_Window* nativeHandle() const noexcept;

	[[nodiscard]]
	SDL_GLContext glContext() const noexcept;

	[[nodiscard]]
	float contentScale() const noexcept;

	[[nodiscard]]
	bool isMinimized() const noexcept;

	[[nodiscard]]
	bool isMaximized() const noexcept;

	// Return { width, height }
	[[nodiscard]]
	std::pair<int, int> framebufferSize() const noexcept;

	void swapBuffers() const;

	// call either maximize or restore
	bool toggleMaximize();

	void setTitleBarHitTest(float titleBarHeight, float captionButtonWidth);

private:
	static SDL_HitTestResult SDLCALL hitTest(SDL_Window* window, const SDL_Point* point, void* data);

	struct SDLWindowDeleter { void operator()(SDL_Window* w) const noexcept; };
	struct GLContextDeleter { void operator()(SDL_GLContextState* c) const noexcept; };

	using UniqueWindow = std::unique_ptr<SDL_Window, SDLWindowDeleter>;
	using UniqueGLContext = std::unique_ptr<SDL_GLContextState, GLContextDeleter>;

	UniqueWindow m_window;
	UniqueGLContext m_glContext;

	float m_contentScale = 1.0f;

	float m_titleBarHeight = 0.0f;
	float m_captionButtonsWidth = 0.0f;
	int m_windowWidth = 0;

	bool m_hitTestRegistered = false;
};
}