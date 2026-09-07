#include "Exercise.hpp"
#include "shader.hpp"
#include "ui/AppWorkArea.hpp"

#include <glad/gl.h>

// vertex data
float vertices[] = {
	// DEFAULT_TRIANGLE / Rectangle
	0.0f, 0.5f, 0.0f,	// 00 - center top
	-0.5f, -0.5f, 0.0f,	// 01 - bottom left
	0.5f, -0.5f, 0.0f,	// 02 - bottom right
	//
	-0.5f, 0.5f, 0.0f,	// 03 - top left
	0.5f, 0.5f, 0.0f,	// 04 - top right
	0.0f, -0.5f, 0.0f,	// 05 - center bottom
	// Triangle T1
	-0.5f, 0.0f, 0.0f,	// 06: T1 - Left
	-0.25f, 0.5f, 0.0f,	// 07: T1 - Top
	0.0f, 0.0f, 0.0f,	// 08: T1 - Right
	// Triangle T2
	0.0f, 0.0f, 0.0f,	// 09: T2 - Left
	0.25f, 0.5f, 0.0f,	// 10: T2 - Top
	0.5f, 0.0f, 0.0f,	// 11: T2 - Right
};

// referencing vertices[]
unsigned int indices[] = {
	// default triangle
	0, 1, 2,
	// rectangle
	3, 1, 2,	// first triangle
	4, 3, 2,	// second triangle
	// Triangle T1
	6, 7, 8,
	// Triangle T2
	9, 10, 11,	// T2
};

namespace excelsior::exercise {
	Exercise ex;

	Exercise::Exercise() :
		m_primaryFragColor(orangeColVec),
		m_secondaryFragColor(yellowColVec)
	{
	}

	void Exercise::initGL() {
		assert(m_GLstate != GLState::INITIALIZED);

		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);
		glGenBuffers(1, &m_ebo);

		glBindVertexArray(m_vao);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(glsl::aPos, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), reinterpret_cast<void*>(0));
		glEnableVertexAttribArray(glsl::aPos);

		m_GLstate = GLState::INITIALIZED;
	}

	void Exercise::destroyGL() {
		assert(m_GLstate != GLState::DESTROYED);

		glDeleteProgram(m_shaderProgram);
		glDeleteVertexArrays(1, &m_vao);
		glDeleteBuffers(1, &m_vbo);
		glDeleteBuffers(1, &m_ebo);

		m_GLstate = GLState::DESTROYED;
	}

	void Exercise::render() {
		assert(m_GLstate == GLState::INITIALIZED);

		glUseProgram(m_shaderProgram);
		glBindVertexArray(m_vao);
		glUniform4fv(glsl::uColor, 1, m_primaryFragColor);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	void Exercise::draw(const AppWorkArea workArea) {
		drawExerciseSelectWindow(workArea);
	}

	void Exercise::setShaderProgram(unsigned int shaderProgram) {
		m_shaderProgram = shaderProgram;
	}

	void Exercise::setWireframe() const {
		glPolygonMode(GL_FRONT_AND_BACK, m_isPolygonMode ? GL_LINE : GL_FILL);
	}
}