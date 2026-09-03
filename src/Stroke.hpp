#pragma once

#include <string>
#include <vector>

struct SDL_FPoint;

std::string getMsg();

typedef struct {
	std::vector<SDL_FPoint> points;
} Stroke;