// Specifically for UI helper functions

#pragma once

struct ImVec2;
struct ImVec4;

namespace excelsior::ui {
	// Vertical space
	void spacerY(float height = 20.0f);

	// Horizontal space
	void spacerX(float width = 10.0f);

	void coloredDummy(const char* text, const ImVec2& size, ImVec4 col);

	// scoped under group
	void coloredDummyGroup(const char* text, const ImVec2& size, ImVec4 col);

	void undecoratedLink(const char* label);
}