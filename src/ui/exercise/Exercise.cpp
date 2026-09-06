#include "Exercise.hpp"
#include "ui/AppWorkArea.hpp"

namespace excelsior {
	namespace exercise {
		Exercise ex;

		void Exercise::draw(const AppWorkArea workArea) {
			drawExerciseSelectWindow(workArea);
		}
	}
}