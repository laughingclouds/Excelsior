#include "PenInput.hpp"

#include <algorithm>

namespace excelsior {
	void PenInput::processEvent(const SDL_Event& event) {
		switch (event.type) {
		case SDL_EVENT_PEN_MOTION:
			processMotion(event); break;
		case SDL_EVENT_PEN_AXIS:
			processAxis(event); break;
		}
	}

	void PenInput::processMotion(const SDL_Event& event) {
		if (m_state.pressure <= 0.0f) return;

		m_state.x = event.pmotion.x;
		m_state.y = event.pmotion.y;
	}

	void PenInput::processAxis(const SDL_Event& event) {
		switch (event.paxis.axis) {
		case SDL_PEN_AXIS_PRESSURE:
			m_state.rawPressure = event.paxis.value;

			m_state.pressure = (m_state.rawPressure == 0.0f) ? 0.0f : std::min(m_state.rawPressure + PRESSURE_OFFSET, 1.0f);

			if (m_state.pressure == 0.0f) {
				m_state.x = -1.0f;
				m_state.y = -1.0f;
			}
			break;
		case SDL_PEN_AXIS_XTILT:
			m_state.tiltX = event.paxis.value; break;
		case SDL_PEN_AXIS_YTILT:
			m_state.tiltY = event.paxis.value; break;
		}
	}

	const PenState& PenInput::state() const noexcept {
		return m_state;
	}

	bool PenInput::isDrawing() const noexcept {
		return (m_state.pressure > 0.0f &&
			m_state.x >= 0.0f &&
			m_state.y >= 0.0f);
	}
}