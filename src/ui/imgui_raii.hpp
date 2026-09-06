#pragma once

#include <imgui.h>

namespace excelsior {
	
	struct GroupScope {
		inline GroupScope() { ImGui::BeginGroup(); }
		inline ~GroupScope() { ImGui::EndGroup(); }

		GroupScope(const GroupScope&) = delete;
		GroupScope& operator=(const GroupScope&) = delete;
	};
}

// concatenate two tokens, would become ab or
// ui_group_42 (if ran on line 42)
#define UI_CONCAT_IMPL(a, b) a ## b
#define UI_CONCAT(a, b) UI_CONCAT_IMPL(a, b)

/* Secretly declare a variable with a unique name
when the variable is declared its constructor gets called
when variable goes out of scope its destructor gets called
and since both are inline, we essentially wrap code between BeginGroup & EndGroup
*/
#define UI_GROUP() ::excelsior::GroupScope UI_CONCAT(ui_group_, __LINE__)