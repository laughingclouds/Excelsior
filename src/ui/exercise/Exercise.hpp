#pragma once

namespace excelsior {
	struct AppWorkArea;

	namespace exercise {
		class Exercise {
		public:
			// draw complete ui
			void draw(const AppWorkArea workArea);
		private:
			// draw a render selection window in mid-left
			void drawExerciseSelectWindow(const AppWorkArea workArea);
		};

		extern Exercise ex;
	}
}