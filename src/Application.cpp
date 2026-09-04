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

	m_glContext.reset(SDL_GL_CreateContext(m_window.get()));
	if (!m_glContext) {
		throw std::runtime_error(std::format("Couldn't create GL Context for window: {}", SDL_GetError()));
	}

	SDL_GL_MakeCurrent(m_window.get(), m_glContext.get());
	SDL_GL_SetSwapInterval(1); // Enable vsync
	SDL_SetWindowPosition(m_window.get(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	SDL_ShowWindow(m_window.get());

	// setup imgui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	
	// enable keyboard, gamepad controls
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	// setup imgui style
	ImGui::StyleColorsDark();

	// setup scaling
	ImGuiStyle& style = ImGui::GetStyle();
	style.ScaleAllSizes(main_scale);
	style.FontScaleDpi = main_scale;

	// setup platform/renderer backends
	ImGui_ImplSDL3_InitForOpenGL(m_window.get(), m_glContext.get());
	ImGui_ImplOpenGL3_Init(glsl_version);

	SDL_SetLogPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_DEBUG);

	int version = gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress);
	if (!version) {
		throw std::runtime_error(std::format("Couldn't initialize OpenGL Context"));
	}

SDL_AppResult Application::handleEvent(const SDL_Event& event) {
		m_imgui.processEvent(event);
		m_penInput.processEvent(event);

	switch (event.type) {
	case SDL_EVENT_QUIT:
		return SDL_APP_SUCCESS;

	case SDL_EVENT_WINDOW_EXPOSED:
			return update();

	case SDL_EVENT_PEN_MOTION:
			if (m_penInput.isDrawing()) m_topLeftTextMessage = "Writing";
			break;

	case SDL_EVENT_PEN_AXIS:
			if (event.paxis.axis == SDL_PEN_AXIS_PRESSURE) {
				const PenState& pen = m_penInput.state();

		m_topLeftTextMessage = std::format(
			"Actual pressure: {}, offset: {}, New Pressure: {}",
					pen.rawPressure,
					PenInput::PRESSURE_OFFSET,
					pen.pressure
		);
			}
		break;
	}

		return SDL_APP_CONTINUE;
	case SDL_PEN_AXIS_YTILT:
		m_tiltY = event.paxis.value; break;
	}
}

SDL_AppResult Application::update() {
	if (SDL_GetWindowFlags(m_window.get()) & SDL_WINDOW_MINIMIZED) {
		SDL_Delay(10); // Throttle loop
		return SDL_APP_CONTINUE;
	}

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();

	if (m_show_demo_window)
		ImGui::ShowDemoWindow(&m_show_demo_window);

	this->imguiHelloWorld();

	if (m_show_another_window)
		this->anotherWindow();

	this->topLeftOverlay();

	ImGui::Render();
	ImGuiIO& io = ImGui::GetIO();
	glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
	if (clear_color_changed) {
		clear_color_changed = false;
		glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
	}
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	SDL_GL_SwapWindow(m_window.get());

	return SDL_APP_CONTINUE;
}

void Application::imguiHelloWorld() {
	ImGuiIO& io = ImGui::GetIO();
	static float f = 0.0f;
	static int counter = 0;

	// Begin/End pair creates a named window
	ImGui::Begin("Hello World!");

	ImGui::Text(m_topLeftTextMessage.c_str());
	ImGui::Checkbox("Demo Window", &m_show_demo_window);
	ImGui::Checkbox("Another Window", &m_show_another_window);

	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
	if (ImGui::ColorEdit3("Clear color", (float*)&clear_color)) {
		clear_color_changed = true;
	}

	if (ImGui::Button("Button"))
		counter++;

	ImGui::SameLine();
	ImGui::Text("counter = %d", counter);

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	ImGui::End();
}

void Application::anotherWindow() {
	ImGui::Begin("Another Window", &m_show_another_window);
	ImGui::Text("Hello from another window!");
	if (ImGui::Button("Close me"))
		m_show_another_window = false;
	ImGui::End();
}

void Application::topLeftOverlay(bool* p_open) {
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
		ImGui::Text(m_topLeftTextMessage.c_str());
		ImGui::Separator();
		if (ImGui::IsMousePosValid())
			ImGui::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
		else
			ImGui::Text("Mouse Position: <invalid>");
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