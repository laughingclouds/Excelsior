# Exercises

From [learnopengl.com](https://learnopengl.com/).

I was/am \[initially\] learning to use opengl with GLFW but since I also had a project
idea (and because I wanted to use SDL3 for that [this] project) I thought to
implement what I learned [and will learn] of opengl in my project too.

The intent is to only use one draw call `exercise::Exercise::draw` in AppUi. This function uses
ImGui to provide different options (different exercises) that will render something different on screen.

Now that I think about it I should've only exposed that one draw call instead of exposing
a complete class object using extern. But fixing that is for later I guess.