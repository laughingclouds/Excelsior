#pragma once

#include <array>

union SDL_Event;

namespace excelsior {
	class Window;

	class ImGuiLayer {
	public:
		explicit ImGuiLayer(Window& window);
		~ImGuiLayer();

		ImGuiLayer(const ImGuiLayer&) = delete;
		ImGuiLayer& operator=(const ImGuiLayer&) = delete;
		ImGuiLayer(ImGuiLayer&&) = delete;
		ImGuiLayer& operator=(ImGuiLayer&&) = delete;

		void processEvent(const SDL_Event& event);
		
		/* Start a new ImGui Frame by calling ImGui::NewFrame(); */
		void beginFrame() const;
		
		void render() const;

	private:
		Window& m_window;
	};
}