// Excelsior.cpp : Defines the entry point for the application.
/* 
* Moving or resizing window is stuttery
* https://github.com/libsdl-org/SDL/issues/12528
*/

#include <algorithm>
#include <format>
#include <iostream>
#include <string>

#include "Stroke.hpp"

#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#define WINDOW_HEIGHT 480
#define WINDOW_WIDTH 640
constexpr float PRESSURE_OFFSET = 0.5;

static float previous_touch_x = -1.0f;
static float previous_touch_y = -1.0f;
static float tilt_x = 0.0f;
static float tilt_y = 0.0f;

static std::string topLeftTextMessage;

static int height_offset;
static int width_offset;

// example taken from https://github.com/libsdl-org/SDL/blob/main/docs/hello.c
// https://examples.libsdl.org/SDL3/pen/01-drawing-lines/

typedef struct {
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* render_target;
} AppState;

/* Runs once at startup */
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {

	int w, h;

	SDL_SetAppMetadata("Excelsior", "0.1", "com.github.laughingclouds");

	SDL_Log("Available renderer drivers:");
	for (int i = 0; i < SDL_GetNumRenderDrivers(); i++) {
		SDL_Log("%d. %s", i + 1, SDL_GetRenderDriver(i));
	}

	if (!SDL_InitSubSystem(SDL_INIT_VIDEO)) {
		SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	AppState* as = (AppState*)SDL_calloc(1, sizeof(AppState));
	if (!as) {
		return SDL_APP_FAILURE;
	}

	*appstate = as;

	/* Create the window */
	if (!SDL_CreateWindowAndRenderer("Excelsior", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &as->window, &as->renderer)) {
		SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	/* Draw lines on render target (single texture) instead of recording and
	redrawing pen strokes in each frame */

	// make render target match output size so drawing matches pen's position on tablet displays
	SDL_GetRenderOutputSize(as->renderer, &w, &h);
	width_offset = WINDOW_WIDTH - w;
	height_offset = WINDOW_HEIGHT - h;

	as->render_target = SDL_CreateTexture(as->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
	if (!as->render_target) {
		SDL_Log("Couldn't create render target: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	/* blank the render target to gray to start */
	SDL_SetRenderTarget(as->renderer, as->render_target);
	SDL_SetRenderDrawColor(as->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(as->renderer);
	SDL_SetRenderTarget(as->renderer, nullptr);
	SDL_SetRenderDrawBlendMode(as->renderer, SDL_BLENDMODE_BLEND);


	SDL_SetLogPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_DEBUG);

	topLeftTextMessage = getMsg();

	return SDL_APP_CONTINUE;
}

/* Runs when a new event (mouse input, keypresses, etc) occurs */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
	AppState* as = (AppState*)appstate;
	static float pressure = 0.0f;

	if (event->type == SDL_EVENT_QUIT) {
		return SDL_APP_SUCCESS; /* end program, reporting success to OS */
	}

	if (event->type == SDL_EVENT_PEN_MOTION) {
		if (pressure > 0.0f) {
			if (previous_touch_x >= 0.0f) { // only draw if we're moving while touching
				topLeftTextMessage = "Writing";
				SDL_SetRenderTarget(as->renderer, as->render_target);
				SDL_SetRenderDrawColorFloat(as->renderer, 255, 255, 255, pressure); // white ink
				SDL_RenderLine(as->renderer, previous_touch_x, previous_touch_y, event->pmotion.x, event->pmotion.y);
			}
			previous_touch_x = event->pmotion.x;
			previous_touch_y = event->pmotion.y;
			//previous_touch_x = event->pmotion.x + width_offset;
			//previous_touch_y = event->pmotion.y + height_offset;
		}
		else {
			previous_touch_x = previous_touch_y = -1.0f;
		}
	}

	if (event->type == SDL_EVENT_WINDOW_RESIZED) {
		int w, h;
		SDL_GetRenderOutputSize(as->renderer, &w, &h);

		width_offset = WINDOW_WIDTH - w;
		height_offset = WINDOW_HEIGHT - h;

		topLeftTextMessage = std::format(
			"Window Resized: wOffset: {}, hOffset: {}",
			width_offset,
			height_offset
		);
	}
	else if (event->type == SDL_EVENT_PEN_AXIS) {
		if (event->paxis.axis == SDL_PEN_AXIS_PRESSURE) {
			//SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "axis = %d, value = %f", event->paxis.axis, event->paxis.value);
			const float rawPressure = event->paxis.value;
			
			pressure = (rawPressure == 0.0f) ? 0.0f : std::min(rawPressure + PRESSURE_OFFSET, 1.0f);

			topLeftTextMessage = std::format(
				"Actual pressure: {}, Offset: {}, New Pressure: {}",
				rawPressure,
				PRESSURE_OFFSET,
				pressure
			);
		}
		else if (event->paxis.axis == SDL_PEN_AXIS_XTILT) {
			tilt_x = event->paxis.value;
		}
		else if (event->paxis.axis == SDL_PEN_AXIS_YTILT) {
			tilt_y = event->paxis.value;
		}
	}

	return SDL_APP_CONTINUE;
}

/* Runs once per frame, and is the heart of the program */
SDL_AppResult SDL_AppIterate(void* appstate) {

	AppState *as = (AppState*)appstate;

	/* make sure we're drawing to window and not render target */
	SDL_SetRenderTarget(as->renderer, nullptr);
	// non-drawing surface to be gray to distinguish
	SDL_SetRenderDrawColor(as->renderer, 100, 100, 100, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(as->renderer);
	SDL_RenderTexture(as->renderer, as->render_target, nullptr, nullptr);
	//SDL_snprintf(debug_text, sizeof(debug_text), "Tilt: %f %f", tilt_x, tilt_y);
	SDL_RenderDebugText(as->renderer, 0, 8, topLeftTextMessage.c_str());
	SDL_RenderPresent(as->renderer);

	return SDL_APP_CONTINUE;
}

/* Runs once at shutdown */
void SDL_AppQuit(void *appstate, SDL_AppResult result){
	if (appstate != NULL) {
		AppState* as = (AppState*)appstate;

		SDL_DestroyTexture(as->render_target);
		SDL_DestroyRenderer(as->renderer);
		SDL_DestroyWindow(as->window);

		SDL_free(as);
	}
}