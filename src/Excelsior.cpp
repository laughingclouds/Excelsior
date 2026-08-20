// Excelsior.cpp : Defines the entry point for the application.
//

#include "Excelsior.h"


#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static SDL_Texture* render_target = NULL;
static float pressure = 0.0f;
static float previous_touch_x = -1.0f;
static float previous_touch_y = -1.0f;
static float tilt_x = 0.0f;
static float tilt_y = 0.0f;

// example taken from https://github.com/libsdl-org/SDL/blob/main/docs/hello.c
// https://examples.libsdl.org/SDL3/pen/01-drawing-lines/


/* Runs once at startup */
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {

	int w, h;

	SDL_SetAppMetadata("Excelsior", "0.1", "com.github.laughingclouds");

	if (!SDL_InitSubSystem(SDL_INIT_VIDEO)) {
		SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	/* Create the window */
	if (!SDL_CreateWindowAndRenderer("Excelsior", 640, 480, 0, &window, &renderer)) {
		SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	/* Draw lines on render target (single texture) instead of recording and
	redrawing pen strokes in each frame */

	// make render target match output size so drawing matches pen's position on tablet displays
	SDL_GetRenderOutputSize(renderer, &w, &h);
	render_target = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
	if (!render_target) {
		SDL_Log("Couldn't create render target: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	/* blank the render target to gray to start */
	SDL_SetRenderTarget(renderer, render_target);
	SDL_SetRenderDrawColor(renderer, 100, 100, 100, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
	SDL_SetRenderTarget(renderer, NULL);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);


	SDL_SetLogPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_DEBUG);

	return SDL_APP_CONTINUE;
}

/* Runs when a new event (mouse input, keypresses, etc) occurs */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
	if (event->type == SDL_EVENT_QUIT) {
		return SDL_APP_SUCCESS; /* end program, reporting success to OS */
	}

	// for some reason pressure isn't working
	if (event->type == SDL_EVENT_PEN_DOWN) {
		pressure = 1.0f;
	}

	if (event->type == SDL_EVENT_PEN_UP) {
		pressure = 0.0f;
	}

	if (event->type == SDL_EVENT_PEN_MOTION) {
		if (pressure > 0.0f) {
			if (previous_touch_x >= 0.0f) { // only draw if we're moving while touching
				SDL_SetRenderTarget(renderer, render_target);
				SDL_SetRenderDrawColorFloat(renderer, 0, 0, 0, pressure);
				SDL_RenderLine(renderer, previous_touch_x, previous_touch_y, event->pmotion.x, event->pmotion.y);
			}
			previous_touch_x = event->pmotion.x;
			previous_touch_y = event->pmotion.y;
		}
		else {
			previous_touch_x = previous_touch_y = -1.0f;
		}
	}

	return SDL_APP_CONTINUE;
}

/* Runs once per frame, and is the heart of the program */
SDL_AppResult SDL_AppIterate(void* appstate) {

	char debug_text[1024];

	/* make sure we're drawing to window and not render target */
	SDL_SetRenderTarget(renderer, NULL);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
	SDL_RenderTexture(renderer, render_target, NULL, NULL);
	SDL_snprintf(debug_text, sizeof(debug_text), "Tilt: %f %f", tilt_x, tilt_y);
	SDL_RenderDebugText(renderer, 0, 8, debug_text);
	SDL_RenderPresent(renderer);

	return SDL_APP_CONTINUE;
}

/* Runs once at shutdown */
void SDL_AppQuit(void *appstate, SDL_AppResult result){
	SDL_DestroyTexture(render_target);
	/* SDL cleans window/renderer for us */
}