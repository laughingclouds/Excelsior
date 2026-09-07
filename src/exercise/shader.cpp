// keeping the raw strings in global because indenting is making the
// the code look weirder.
#include "shader.hpp"
#include "Exercise.hpp"

#include <format>
#include <stdexcept>

#include <glad/gl.h>

const char* vertexShaderSource = R"(
#version 430 core
layout (location = 0) in vec3 aPos;

void main() {
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
)";

const char* fragmentShaderSource = R"(
#version 430 core
out vec4 FragColor;

layout (location = 20) uniform vec4 uColor;

void main() {
	FragColor = uColor;
}
)";

enum class QueryType {
	VERTEX,
	FRAGMENT,
	PROGRAM
};

using statusQuery_t = void (*) (GLuint, GLenum, GLint*);
using infoLog_t = void (*) (GLuint, GLsizei, GLsizei*, GLchar*);

static const char* errorMessage(QueryType qt) {
	switch (qt) {
	case QueryType::VERTEX: return "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n";
	case QueryType::FRAGMENT: return "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n";
	case QueryType::PROGRAM: return "ERROR::SHADER::PROGRAM::LINKING_FAILED\n";
	}
	return "ERROR::UNKNOWN\n";
}

/* check compilation status shader, and link status for program.
Throw runtime error if required.*/
static void statusCheck(statusQuery_t statusQuery, infoLog_t getInfoLog, GLuint object, GLenum en, QueryType qt) {
	GLint success;
	char infoLog[512];

	statusQuery(object, en, &success);
	if (!success) {
		getInfoLog(object, 512, nullptr, infoLog);

		throw std::runtime_error(std::format("{} {}", errorMessage(qt), (const char*)infoLog));
	}
}

static GLuint createShader(GLenum shaderType, const char *shaderSource, QueryType qt) {
	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderSource, nullptr);
	glCompileShader(shader);

	statusCheck(glGetShaderiv, glGetShaderInfoLog, shader, GL_COMPILE_STATUS, qt);

	return shader;
}

static GLuint createProgram(GLuint vertexShader, GLuint fragmentShader) {
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	statusCheck(glGetProgramiv, glGetProgramInfoLog, shaderProgram, GL_LINK_STATUS, QueryType::PROGRAM);

	// No longer need shader objects after program linked
	glDetachShader(shaderProgram, vertexShader);
	glDetachShader(shaderProgram, fragmentShader);

	return shaderProgram;
}

namespace excelsior::exercise {

	void initializeShaderProgram() {
		GLuint vertexShader = createShader(GL_VERTEX_SHADER, vertexShaderSource, QueryType::VERTEX);
		GLuint fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentShaderSource, QueryType::FRAGMENT);

		GLuint shaderProgram = createProgram(vertexShader, fragmentShader);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		ex.setShaderProgram(shaderProgram);
	}
}