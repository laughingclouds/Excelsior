#pragma once

namespace excelsior {
	struct AppWorkArea;
}

namespace excelsior::exercise {

	inline constexpr float orangeColVec[] = { 1.0f, 0.5f, 0.2f, 1.0f };
	inline constexpr float yellowColVec[] = { 1.0f, 1.0f, 0.0f, 1.0f };

	enum class Type {
		DEFAULT_TRIANGLE,
		RECTANGLE,
		TWO_TRIANGLES, // next to each other
		TWO_TRIANGLES_DIFF_COL // next to ech other, different color
	};

	enum class GLState {
		GLOBAL,			// default state
		INITIALIZED,	// initGL called
		DESTROYED,		// destroyGL called
	};

	class Exercise {
	public:
		Exercise();

		/*Initialize Vertex Array Object & Buffer Objects (VBO, EBO) */
		void initGL();

		// Destroy all that initGL() (and additional) created.
		void destroyGL();

		void render();

		// draw complete ui
		void draw(const AppWorkArea workArea);

		void setShaderProgram(unsigned int shaderProgram);

	private:
		// draw a render selection window in mid-left
		void drawExerciseSelectWindow(const AppWorkArea workArea);

		Type m_type = Type::DEFAULT_TRIANGLE;
		bool m_isPolygonMode = false;

		const float* m_primaryFragColor;
		const float* m_secondaryFragColor;

		unsigned int m_shaderProgram = 0;
		unsigned int m_vao = 0; // vertex array object
		unsigned int m_vbo = 0; // vertex buffer object
		unsigned int m_ebo = 0; // element buffer object

		GLState m_GLstate = GLState::GLOBAL;
	};

	extern Exercise ex;

}