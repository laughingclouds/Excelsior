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