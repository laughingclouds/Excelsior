#pragma once

union SDL_Event;

namespace excelsior {
	struct PenState {
		float x = -1.0f;
		float y = -1.0f;

		float rawPressure = 0.0f;
		float pressure = 0.0f;

		float tiltX = 0.0f;
		float tiltY = 0.0f;
	};

	class PenInput {
	public:
		static constexpr float PRESSURE_OFFSET = 0.5f;

		void processEvent(const SDL_Event& event);

		[[nodiscard]]
		const PenState& state() const noexcept;

		[[nodiscard]]
		bool isDrawing() const noexcept;

	private:
		void processMotion(const SDL_Event& event);
		void processAxis(const SDL_Event& event);

		PenState m_state;
	};
}