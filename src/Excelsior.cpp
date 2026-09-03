// Excelsior.cpp : Defines the entry point for the application.
/* 
* Moving or resizing window is stuttery
* https://github.com/libsdl-org/SDL/issues/12528
*/
#include "Application.hpp"

#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>



/* Runs once at startup */
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
	try {
		*appstate = new Application();
		return SDL_APP_CONTINUE;
	}
	catch (const std::exception& e) {
		SDL_Log("AppInit Error: %s", e.what());
		return SDL_APP_FAILURE;
	}
}

/* Runs when a new event (mouse input, keypresses, etc) occurs */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
	auto* app = static_cast<Application*>(appstate);
	return app->handleEvent(*event);
}

/* Runs once per frame, and is the heart of the program */
SDL_AppResult SDL_AppIterate(void* appstate) {
	auto* app = static_cast<Application*>(appstate);
	return app->update();
}

/* Runs once at shutdown */
void SDL_AppQuit(void *appstate, SDL_AppResult result){
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();

	delete static_cast<Application*>(appstate);
}