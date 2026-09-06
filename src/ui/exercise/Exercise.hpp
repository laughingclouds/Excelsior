#pragma once

namespace excelsior {
	struct AppWorkArea;
}

namespace excelsior::exercise {

	inline constexpr float orangeColVec[] = { 1.0f, 0.5f, 0.2f, 1.0f };
	inline constexpr float yellowColVec[] = { 1.0f, 1.0f, 0.0f, 1.0f };

	enum Type {
		DEFAULT_TRIANGLE,
		RECTANGLE,
		TWO_TRIANGLES, // next to each other
		TWO_TRIANGLES_DIFF_COL, // next to ech other, different color
	};

	class Exercise {
	public:
		Exercise();

		// draw complete ui
		void draw(const AppWorkArea workArea);
	private:
		// draw a render selection window in mid-left
		void drawExerciseSelectWindow(const AppWorkArea workArea);

		int m_type = DEFAULT_TRIANGLE;
		bool m_isPolygonMode = false;

		const float* m_primaryFragColor;
		const float* m_secondaryFragColor;
	};

	extern Exercise ex;

}