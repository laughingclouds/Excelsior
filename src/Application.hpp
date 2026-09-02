#pragma once

#include <memory>
#include <string>

// SDL forward declarations
struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;
union SDL_Event;
enum SDL_AppResult;

struct SDLWindowDeleter { void operator()(SDL_Window* w) const; };
struct SDLRendererDeleter { void operator()(SDL_Renderer* r) const; };
struct SDLTextureDeleter { void operator()(SDL_Texture* t) const; };

using UniqueWindow = std::unique_ptr<SDL_Window, SDLWindowDeleter>;
using UniqueRenderer = std::unique_ptr<SDL_Renderer, SDLRendererDeleter>;
using UniqueTexture = std::unique_ptr<SDL_Texture, SDLTextureDeleter>;

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
	UniqueWindow m_window;
	UniqueRenderer m_renderer;
	UniqueTexture m_renderTarget;

	float m_previous_touchX = -1.0f;
	float m_previous_touchY = -1.0f;
	float m_pressure = 0.0f;
	float m_tiltX = 0.0f;
	float m_tiltY = 0.0f;

	std::string m_topLeftTextMessage;
};