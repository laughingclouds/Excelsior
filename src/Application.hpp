#pragma once

#include <memory>
#include <string>
#include <format>
#include <stdexcept>

#include <SDL3/SDL.h>

#include "Stroke.hpp"

struct SDLWindowDeleter { void operator()(SDL_Window* w) const { SDL_DestroyWindow(w); } };
struct SDLRendererDeleter { void operator()(SDL_Renderer* r) const { SDL_DestroyRenderer(r); } };
struct SDLTextureDeleter { void operator()(SDL_Texture* t) const { SDL_DestroyTexture(t); } };

using UniqueWindow = std::unique_ptr<SDL_Window, SDLWindowDeleter>;
using UniqueRenderer = std::unique_ptr<SDL_Renderer, SDLRendererDeleter>;
using UniqueTexture = std::unique_ptr<SDL_Texture, SDLTextureDeleter>;

class Application {
public:
	static constexpr int WINDOW_WIDTH = 640;
	static constexpr int WINDOW_HEIGHT = 480;
	static constexpr float PRESSURE_OFFSET = 0.5f;

	Application();

	SDL_AppResult handleEvent(const SDL_Event& event);
	SDL_AppResult update();

private:
	UniqueWindow m_window;
	UniqueRenderer m_renderer;
	UniqueTexture m_renderTarget;

	float m_previous_touch_x = -1.0f;
	float m_previous_touch_y = -1.0f;
	float m_tilt_x = 0.0f;
	float m_tilt_y = 0.0f;

	std::string m_topLeftTextMessage;
};