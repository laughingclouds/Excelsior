#include "Exercise.hpp"
#include "ui/AppWorkArea.hpp"

namespace excelsior::exercise {
	Exercise ex;

	Exercise::Exercise() :
		m_primaryFragColor(orangeColVec),
		m_secondaryFragColor(yellowColVec)
	{
	}

	void Exercise::draw(const AppWorkArea workArea) {
		drawExerciseSelectWindow(workArea);
	}

}