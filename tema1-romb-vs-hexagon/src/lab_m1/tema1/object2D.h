#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace object2D
{
    Mesh* CreateSquare(const std::string& name, glm::vec3 bottomLeftCorner, float length, glm::vec3 color, bool fill = false);
    Mesh* CreateRectangle(const std::string& name, glm::vec3 bottomLeftCorner, float width, float height, glm::vec3 color, bool fill = false);
    Mesh* CreateHexagon(const std::string& name, float size, glm::vec3 color, bool fill = false);
    Mesh* CreateHeart(const std::string& name, float size, glm::vec3 color, bool fill = false);
    Mesh* CreateTurret(const std::string& name, float size, glm::vec3 color, bool fill = false);
    Mesh* CreateStar(const std::string& name, float size, glm::vec3 color, bool fill = false);
}

