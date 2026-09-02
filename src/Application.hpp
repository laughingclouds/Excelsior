#pragma once

#include <memory>

#include <SDL3/SDL.h>

struct SDLWindowDeleter { void operator()(SDL_Window* w) const { SDL_DestroyWindow(w); } };
struct SDLRendererDeleter { void operator()(SDL_Renderer* r) const { SDL_DestroyRenderer(r); } };
struct SDLTextureDeleter { void operator()(SDL_Texture* t) const { SDL_DestroyTexture(t); } };

using UniqueWindow = std::unique_ptr<SDL_Window, SDLWindowDeleter>;
using UniqueRenderer = std::unique_ptr<SDL_Renderer, SDLRendererDeleter>;
using UniqueWindow = std::unique_ptr<SDL_Texture, SDLTextureDeleter>;

class Application {};