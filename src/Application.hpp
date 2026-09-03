#pragma once

#include <memory>
#include <string>

// SDL forward declarations
struct SDL_Window;
struct SDL_GLContextState;
typedef SDL_GLContextState* SDL_GLContext;
union SDL_Event;
enum SDL_AppResult;

struct SDLWindowDeleter { void operator()(SDL_Window* w) const; };
struct GLContextDeleter { void operator()(SDL_GLContext c) const; };

using UniqueWindow = std::unique_ptr<SDL_Window, SDLWindowDeleter>;
using UniqueGLContext = std::unique_ptr<SDL_GLContextState, GLContextDeleter>;

class Application {
public:
	static constexpr int WINDOW_WIDTH = 1280;
	static constexpr int WINDOW_HEIGHT = 800;
	static constexpr float PRESSURE_OFFSET = 0.5f;
	
	/* Initialize Application + (window, renderer, texture)
	To be called in SDL_AppInit*/
	Application();

	// Call within SDL_AppEvent
	SDL_AppResult handleEvent(const SDL_Event& event);

	// Call within SDL_AppIterate
	SDL_AppResult update();

private:
	UniqueWindow m_window;
	UniqueGLContext m_glContext;

	// viewport pixels dimension
	int m_fbWidth = WINDOW_WIDTH;
	int m_fbHeight = WINDOW_HEIGHT;

	float m_previous_touchX = -1.0f;
	float m_previous_touchY = -1.0f;
	float m_pressure = 0.0f;
	float m_tiltX = 0.0f;
	float m_tiltY = 0.0f;

	std::string m_topLeftTextMessage;
};