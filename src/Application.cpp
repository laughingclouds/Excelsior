#include <glad/gl.h>

#include "Application.hpp"
#include "Stroke.hpp"

#include <format>

#include <imgui.h>


namespace excelsior {

	Application::Application()
		: 
		m_window("Excelsior",
			WINDOW_WIDTH,
			WINDOW_HEIGHT
		),
		m_imgui(m_window),
		m_topLeftTextMessage(getMsg())
	{
	}

	SDL_AppResult Application::handleEvent(const SDL_Event& event) {
		m_imgui.processEvent(event);
		m_penInput.processEvent(event);

		switch (event.type) {
		case SDL_EVENT_QUIT:
		case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
			return SDL_APP_SUCCESS;

		case SDL_EVENT_WINDOW_EXPOSED:
			return update();

		case SDL_EVENT_PEN_MOTION:
			if (m_penInput.isDrawing()) m_topLeftTextMessage = "Writing";
			else m_topLeftTextMessage = "Idle";
			break;
		}

		return SDL_APP_CONTINUE;
	}

	SDL_AppResult Application::update() {
		if (m_window.isMinimized()) {
			SDL_Delay(10); // Throttle loop
			return SDL_APP_CONTINUE;
		}

		m_imgui.beginFrame();

		drawUi();

		m_imgui.render(m_clearColor);
		m_window.swapBuffers();

		if (m_shouldQuit) {
			return SDL_APP_SUCCESS;
		}

		return SDL_APP_CONTINUE;
	}

	void Application::drawUi() {
		drawWindowChrome();

		if (m_showDemoWindow)
			ImGui::ShowDemoWindow(&m_showDemoWindow);

		showHelloWorldWindow();

		if (m_showAnotherWindow)
			showAnotherWindow();

		showTopLeftOverlay();
	}

	void Application::drawWindowChrome() {
		const ImGuiViewport* viewport = ImGui::GetMainViewport();

		const float chromeHeight = ImGui::GetFrameHeight();

		const float btnWidth = chromeHeight * 2.5;
		const float btnHeight = chromeHeight;
		const float captionButtonsWidth = 3.0f * btnWidth;

		// native window behavior using chrome dimensions
		m_window.configureChromeHitTest(chromeHeight, captionButtonsWidth, m_resizeBorderThickness);
		
		// strip padding, border, minimum restriction
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(0.0f, 0.0f));

		ImGui::SetNextWindowPos(viewport->Pos, ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, chromeHeight), ImGuiCond_Always);

		ImGui::Begin("Excelsior###MainWindowChrome",
			nullptr,
			ImGuiWindowFlags_NoDecoration |
			ImGuiWindowFlags_NoMove |
			//ImGuiWindowFlags_NoBackground |
			ImGuiWindowFlags_NoSavedSettings
			);

		// Title bar
		const float textLineHeight = ImGui::GetTextLineHeight();
		const float textY = (chromeHeight - textLineHeight) * 0.5f;
		ImGui::SetCursorPos(ImVec2(
			8.0f, // space on left side of text
			textY // Y coordinate at middle of imgui window
		));
		ImGui::TextUnformatted("Excelsior");

		// Window buttons

		// right most edge of current window (window chrome)
		const float right = ImGui::GetWindowWidth();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f)); // invisible bg

		ImGui::SetCursorPos(ImVec2(right - 3.0f * btnWidth, 0.0f));
		if (ImGui::Button("_###MinimizeApp", ImVec2(btnWidth, btnHeight))) {
			m_window.minimize();
		}

		ImGui::SetCursorPos(ImVec2(right - 2.0f * btnWidth, 0.0f));
		if (ImGui::Button("[]###MaximizeApp", ImVec2(btnWidth, btnHeight))) {
			m_window.toggleMaximize();
		}

		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.0f, 0.0f, 0.70f)); // red on hover
		ImGui::SetCursorPos(ImVec2(right - btnWidth, 0.0f));
		if (ImGui::Button("X###QuitApp", ImVec2(btnWidth, btnHeight))) {
			m_shouldQuit = true;
		}

		ImGui::PopStyleColor(2);

		ImGui::End();
		ImGui::PopStyleVar(3);
	}

	void Application::showHelloWorldWindow() {
		ImGuiIO& io = ImGui::GetIO();
		static float f = 0.0f;
		static int counter = 0;

		// Begin/End pair creates a named window
		if (ImGui::Begin("Hello World!")) {
			ImGui::Text(m_topLeftTextMessage.c_str());
			ImGui::Checkbox("Demo Window", &m_showDemoWindow);
			ImGui::Checkbox("Another Window", &m_showAnotherWindow);

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
			ImGui::ColorEdit3("Clear color", m_clearColor.data());

			if (ImGui::Button("Button"))
				counter++;

			ImGui::SameLine();

			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		}

		ImGui::End();
	}

	void Application::showAnotherWindow() {
		if (ImGui::Begin("Another Window", &m_showAnotherWindow)) {
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close me"))
				m_showAnotherWindow = false;
		}
		ImGui::End();
	}

	void Application::showTopLeftOverlay(bool* p_open) {
		static int location = 0;
		ImGuiIO& io = ImGui::GetIO();
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
		if (location >= 0)
		{
			const float PAD = 10.0f;
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
			ImVec2 work_size = viewport->WorkSize;
			ImVec2 window_pos, window_pos_pivot;
			window_pos.x = (location & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
			window_pos.y = (location & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
			window_pos_pivot.x = (location & 1) ? 1.0f : 0.0f;
			window_pos_pivot.y = (location & 2) ? 1.0f : 0.0f;
			ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
			window_flags |= ImGuiWindowFlags_NoMove;
		}
		else if (location == -2)
		{
			// Center window
			ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
			window_flags |= ImGuiWindowFlags_NoMove;
		}
		ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
		if (ImGui::Begin("Top Left Overlay", p_open, window_flags))
		{
			const PenState& pen = m_penInput.state();

			ImGui::Text(m_topLeftTextMessage.c_str());
			ImGui::Separator();
			if (ImGui::IsMousePosValid())
				ImGui::Text("Mouse Position: (%.1f, %.1f)", io.MousePos.x, io.MousePos.y);
			else
				ImGui::Text("Mouse Position: <invalid>");

			if (pen.x != -1.0f)
				ImGui::Text("Pen Position: (%.1f, %.1f)", pen.x, pen.y);

			if (pen.pressure > 0.0f)
				ImGui::Text("Pressure: %.3f", pen.pressure);

			if (pen.tiltX > 0.0f || pen.tiltY > 0.0f)
				ImGui::Text("TiltX: %.1f, TiltY: %.1f", pen.tiltX, pen.tiltY);

			if (ImGui::BeginPopupContextWindow())
			{
				if (ImGui::MenuItem("Custom", NULL, location == -1)) location = -1;
				if (ImGui::MenuItem("Center", NULL, location == -2)) location = -2;
				if (ImGui::MenuItem("Top-left", NULL, location == 0)) location = 0;
				if (ImGui::MenuItem("Top-right", NULL, location == 1)) location = 1;
				if (ImGui::MenuItem("Bottom-left", NULL, location == 2)) location = 2;
				if (ImGui::MenuItem("Bottom-right", NULL, location == 3)) location = 3;
				ImGui::EndPopup();
			}
		}
		ImGui::End();
	}
}