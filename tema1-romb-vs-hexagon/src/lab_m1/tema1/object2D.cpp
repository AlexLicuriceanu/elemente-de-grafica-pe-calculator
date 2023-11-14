#include "object2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object2D::CreateSquare(const std::string &name, glm::vec3 bottomLeftCorner, float length, glm::vec3 color, bool fill)
{
    glm::vec3 corner = bottomLeftCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}


Mesh* object2D::CreateRectangle(const std::string& name, glm::vec3 bottomLeftCorner, float width, float height, glm::vec3 color, bool fill)
{
    glm::vec3 corner = bottomLeftCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(width, 0, 0), color),
        VertexFormat(corner + glm::vec3(width, height, 0), color),
        VertexFormat(corner + glm::vec3(0, height, 0), color)
    };

    Mesh* rectangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        rectangle->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    rectangle->InitFromData(vertices, indices);
    return rectangle;
}


Mesh* object2D::CreateHexagon(const std::string& name, float size, glm::vec3 color, bool fill)
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    for (int i = 0; i < 6; i++) {
        auto x = size * glm::cos(2 * M_PI * i / 6);
        auto y = size * glm::sin(2 * M_PI * i / 6);

        vertices.push_back(VertexFormat(glm::vec3(x, y, 0), color));
        indices.push_back(i);
    }

    Mesh* hexagon = new Mesh(name);

    indices.push_back(0);

    if (!fill) {
        hexagon->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        hexagon->SetDrawMode(GL_TRIANGLE_FAN);
    }

    hexagon->InitFromData(vertices, indices);
    return hexagon;
}

/*
    Source: geogebra.org/m/rqtfrhk9
*/
Mesh* object2D::CreateHeart(const std::string& name, float size, glm::vec3 color, bool fill)
{
    int nrPoints = 25;
    auto increment = 2 * M_PI / nrPoints;

    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;


    for (int i = 0; i < nrPoints; ++i) {

        auto angle = increment * i;

        auto x = size * (16 * pow(glm::sin(angle), 3));
        auto y = size * (13 * glm::cos(angle) - 5 * glm::cos(2 * angle) - 2 * glm::cos(3 * angle) - glm::cos(4 * angle));

        vertices.push_back(VertexFormat(glm::vec3(x, y, 0), color));
        indices.push_back(i);
    }

    indices.push_back(0);

    Mesh* heart = new Mesh(name);

    if (!fill) {
        heart->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        heart->SetDrawMode(GL_TRIANGLE_FAN);
    }

    heart->InitFromData(vertices, indices);
    return heart;
}


Mesh* object2D::CreateTurret(const std::string& name, float size, glm::vec3 color, bool fill)
{
    std::vector<VertexFormat> vertices = {
        VertexFormat(size * glm::vec3(-1, 0, 0), color),
        VertexFormat(size * glm::vec3(-0.5f, 1, 0), color),
        VertexFormat(size * glm::vec3(-0.1f, 0.25f, 0), color),
        VertexFormat(size * glm::vec3(1, 0.25f, 0), color),
        VertexFormat(size * glm::vec3(1, -0.25f, 0), color),
        VertexFormat(size * glm::vec3(-0.1f, -0.25f, 0), color),
        VertexFormat(size * glm::vec3(-0.5f, -1, 0), color),
    };

    std::vector<unsigned int> indices = { 0, 1, 2, 3, 4, 5, 6 };

    Mesh* turret = new Mesh(name);

    if (!fill) {
        turret->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        turret->SetDrawMode(GL_TRIANGLE_FAN);
    }

    turret->InitFromData(vertices, indices);
    return turret;
}


Mesh* object2D::CreateStar(const std::string& name, float size, glm::vec3 color, bool fill)
{

    std::vector<VertexFormat> vertices = {
         VertexFormat(size * glm::vec3(0, 1, 0), color), //a 
         VertexFormat(size * glm::vec3(-0.2, 0.7, 0), color), //b
         VertexFormat(size * glm::vec3(-0.4, 0.7, 0), color), //d

         VertexFormat(size * glm::vec3(-0.2f, 0.5, 0), color), //f
         VertexFormat(size * glm::vec3(-0.3, 0.3, 0), color), //h
         
         VertexFormat(size * glm::vec3(0, 0.5, 0), color), // g
         VertexFormat(size * glm::vec3(0.3, 0.3, 0), color), //e 

         VertexFormat(size * glm::vec3(0.2, 0.5, 0), color), //c

         VertexFormat(size * glm::vec3(0.4, 0.7, 0), color), // i
         
         VertexFormat(size * glm::vec3(0.2, 0.7, 0), color), // j

         //VertexFormat(size * glm::vec3(1, -0.25f, 0), color),
         //VertexFormat(size * glm::vec3(-0.1f, -0.25f, 0), color),
         //VertexFormat(size * glm::vec3(-0.5f, -1, 0), color),
    };

    std::vector<unsigned int> indices = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };

    Mesh* star = new Mesh(name);

    if (!fill) {
        star->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        star->SetDrawMode(GL_TRIANGLES);
    }

    star->InitFromData(vertices, indices);
    return star;
}

