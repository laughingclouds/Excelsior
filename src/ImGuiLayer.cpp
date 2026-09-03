#include "ImGuiLayer.hpp"

#include "Window.hpp"

#include <stdexcept>

#include <glad/gl.h>

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl3.h>

namespace excelsior {
	ImGuiLayer::ImGuiLayer(Window& window) : m_window(window) {
		// setup imgui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();

		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();

		style.ScaleAllSizes(m_window.contentScale());
		style.FontScaleDpi = m_window.contentScale();

		// setup platform/renderer backends
		if (!ImGui_ImplSDL3_InitForOpenGL(
			m_window.nativeHandle(),
			m_window.glContext()
		)) {
			ImGui::DestroyContext();
			throw std::runtime_error("Couldn't initialize ImGui SDL3 backend");
		}

		if (!ImGui_ImplOpenGL3_Init("#version 430")) {
			ImGui_ImplSDL3_Shutdown();
			ImGui::DestroyContext();

			throw std::runtime_error("Couldn't initialize ImGui OpenGL backend");
		}
	}

	ImGuiLayer::~ImGuiLayer() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL3_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::processEvent(const SDL_Event& event) {
		ImGui_ImplSDL3_ProcessEvent(&event);
	}

	void ImGuiLayer::beginFrame() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL3_NewFrame();

		ImGui::NewFrame();
	}

	void ImGuiLayer::render(const std::array<float, 4>& clearColor) {
		ImGui::Render();

		const auto [width, height] = m_window.framebufferSize();
		glViewport(0, 0, width, height);
		glClearColor(
			clearColor[0] * clearColor[3],
			clearColor[1] * clearColor[3],
			clearColor[2] * clearColor[3],
			clearColor[3]
		);

		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}