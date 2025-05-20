#pragma once
#include <iostream>
#include <sil/sil.hpp>
#include <Eigen/Dense>
#include <cmath>
#include <vector>


bool check_in_greed(glm::vec2& pos, glm::vec2& size, int& subdivision, int& thickness, glm::vec2 currentXY);
glm::vec2 apply_rotation(int x, int y, double angle, glm::vec2 size, glm::vec2 pos);
void draw_grid(glm::vec2 pos, glm::vec2 size, int subdivision, int thickness, double angle, sil::Image& image, glm::vec3 color);