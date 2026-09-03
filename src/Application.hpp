#pragma once

#include <functional>
#include <memory>
#include <string>

// SDL forward declarations
struct SDL_Window;
struct SDL_GLContextState;
typedef SDL_GLContextState* SDL_GLContext;
union SDL_Event;
enum SDL_AppResult;

using UniqueGLContext = std::unique_ptr<SDL_GLContextState, std::function<void(SDL_GLContext)>>;

class Application {
public:
	static constexpr int WINDOW_WIDTH = 640;
	static constexpr int WINDOW_HEIGHT = 480;
	static constexpr float PRESSURE_OFFSET = 0.5f;
	
	/* Initialize Application + (window, renderer, texture)
	To be called in SDL_AppInit*/
	Application();

	// Call within SDL_AppEvent
	SDL_AppResult handleEvent(const SDL_Event& event);

	// Call within SDL_AppIterate
	SDL_AppResult update();

private:
	std::shared_ptr<SDL_Window> m_window;
	UniqueGLContext m_glContext;

	float m_previous_touchX = -1.0f;
	float m_previous_touchY = -1.0f;
	float m_pressure = 0.0f;
	float m_tiltX = 0.0f;
	float m_tiltY = 0.0f;

	std::string m_topLeftTextMessage;
};