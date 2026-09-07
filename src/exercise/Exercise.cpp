#include "Exercise.hpp"
#include "shader.hpp"
#include "ui/AppWorkArea.hpp"

#include <glad/gl.h>

namespace excelsior::exercise {
	Exercise ex;

	Exercise::Exercise() :
		m_primaryFragColor(orangeColVec),
		m_secondaryFragColor(yellowColVec),
		m_shaderProgram()
	{
	}

	Exercise::~Exercise() {
		glDeleteProgram(m_shaderProgram);
	}

	void Exercise::draw(const AppWorkArea workArea) {
		drawExerciseSelectWindow(workArea);
	}

	void Exercise::setShaderProgram(unsigned int shaderProgram) {
		m_shaderProgram = shaderProgram;
	}
}