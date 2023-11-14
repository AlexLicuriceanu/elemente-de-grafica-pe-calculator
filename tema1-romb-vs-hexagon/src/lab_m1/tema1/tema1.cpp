#include "lab_m1/tema1/tema1.h"

#include <vector>
#include <iostream>
#include <random>

#include "lab_m1/tema1/transform2D.h"
#include "lab_m1/tema1/object2D.h"

using namespace std;
using namespace m1;


Tema1::Tema1()
{

    // Set color values.
    this->colors = {
        {"Red", glm::vec3(1, 0, 0)},
        {"Green", glm::vec3(0.23f, 0.89f, 0.41f)},
        {"Blue", glm::vec3(0, 0, 1)},
        {"Yellow", glm::vec3(1, 1, 0)},
        {"Orange", glm::vec3(1, 0.5f, 0)},
        {"Purple", glm::vec3(0.5f, 0, 0.5f)},
        {"Gray", glm::vec3(0.2f, 0.2f, 0.2f)},
        {"LightGray", glm::vec3(0.5f, 0.5f, 0.5f)},
        {"Turquoise", glm::vec3(0.08f, 0.44f, 0.6f)},
        {"Black", glm::vec3(0, 0, 0)},
        {"Pink", glm::vec3(1, 0.41f, 0.7f)},
    };


    // Set what colors the entities can have.
    this->entityColors = {
        {"Orange", glm::vec3(1, 0.5f, 0)},
        {"Purple", glm::vec3(0.5f, 0, 0.5f)},
        {"Blue", glm::vec3(0, 0, 1)},
        {"Yellow", glm::vec3(1, 1, 0)},
    };

    // Set prices for each color of turret.
    this->turretColorPrice = {
        {"Orange", 1},
        {"Blue", 2},
        {"Yellow", 2},
        {"Purple", 3},
    };

    // Set up the 3x3 game grid.
    this->gridX = 3;
    this->gridY = 3;
    
    // Coords of the botton left corner where the 3x3 grid will start.
    this->gridStartX = 100.0f;
    this->gridStartY = 20.0f;

    // Coords of the bottom left corner of the starting position of the buy menu.
    this->buyMenuStartX = this->gridStartX;
    this->buyMenuStartY = 580.0f;

    // Space between each tile.
    this->gridSpace = 40.0f;

    // Set up a grid with empty cells.
    this->grid.resize(this->gridX, std::vector<Entity*>(this->gridY));
    
    for (int i = 0; i < this->gridX; i++) {
        for (int j = 0; j < this->gridY; j++) {
            grid[i][j] = new Entity();
        }
    }


    // Set up hexagon data structures and parameters.
    this->hexagons = std::vector<std::vector<Entity*>>();
    this->hexagons.resize(this->gridY);
    this->maxHexagonHealth = 3;
    this->hexagonSpeed = 1;
    this->randomHexagonsCounter = 0;
    this->randomHexagonsInterval = 7.0f;
    this->nrRandomHexagons = 1;

    // Set up projectile data structures and parameters.
    this->projectiles = std::vector<std::vector<Entity*>>();
    this->projectiles.resize(this->gridY);
    this->projectileSpeed = 100.0;


    // Set up pink star data structures and parameters.
    this->maxRandomStars = 3;
    this->maxTotalRandomStars = 30;
    this->randomStarsInterval = 4.0f;
    this->randomStarsColor = "Pink";
    this->stars = std::vector<Entity*>();
    this->randomStarsCounter = 0;


    // Other game parameters.
    this->lives = 3;
    this->money = 5;
    this->maxMoney = 11;
    this->drag = false;
    this->draggedEntity = new Entity();
    this->deltaTimeSeconds = 0;
    this->score = 0;

    

    // Set mesh properties.
    this->squareWidth = 120.0f;
    this->rectangleWidth = 50.0f;
    this->rectangleHeight = gridY * squareWidth + (gridY - 1) * gridSpace;
    this->heartSize = 5.0f;
    this->turretSize = 50.0f;
    this->starSize = 50.0f;
    this->bigHexagonSize = 40.0f;
    this->smallHexagonSize = this->bigHexagonSize / 3;
    
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    // Load all meshes into memory.
    loadAllMeshes();

    this->resolution = resolution;
}


/*
 * Loads all required meshes into memory.
 */
void Tema1::loadAllMeshes() {

    glm::vec3 bottomLeftCorner = glm::vec3(0, 0, 0);

    // Load the square.
    Mesh* square = object2D::CreateSquare("square", bottomLeftCorner, this->squareWidth, colors["Green"], true);
    AddMeshToList(square);

    // Load the square outline.
    Mesh* squareOutline = object2D::CreateSquare("squareOutline", bottomLeftCorner, this->squareWidth, colors["Gray"]);
    AddMeshToList(squareOutline);

    // Load the rectangle.
    Mesh* rectangle = object2D::CreateRectangle("rectangle", bottomLeftCorner, this->rectangleWidth, this->rectangleHeight, colors["Gray"], true);
    AddMeshToList(rectangle);

    // Load the turret.
    Mesh* turret = object2D::CreateTurret("turret", this->turretSize, colors["Green"], true);
    AddMeshToList(turret);

    // Load the heart.
    Mesh* heart = object2D::CreateHeart("heart", this->heartSize, colors["Red"], true);
    AddMeshToList(heart);

    // Load the hexagon.
    Mesh* hexagon = object2D::CreateHexagon("hexagon", this->bigHexagonSize, colors["Turquoise"], true);
    AddMeshToList(hexagon);

    // Load the star.
    Mesh* mesh = new Mesh("star");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "star.obj");
    meshes[mesh->GetMeshID()] = mesh;
}

void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(colors["Gray"].x, colors["Gray"].y, colors["Gray"].z, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema1::Update(float deltaTimeSeconds)
{
    this->deltaTimeSeconds = deltaTimeSeconds;
    this->randomStarsCounter += deltaTimeSeconds;
    this->randomHexagonsCounter += deltaTimeSeconds;

    if (this->lives == 0) {
        
        // Exit game.

    }


    if (this->randomStarsCounter >= this->randomStarsInterval) {

        this->randomStarsCounter = 0;

        if (this->stars.size() == 0) {
            spawnRandomStars();
        }

        spawnProjectiles();
    }

    if (this->randomHexagonsCounter >= this->randomHexagonsInterval) {
        this->randomHexagonsCounter = 0;
        spawnRandomHexagons();
    }

    renderDraggedEntity();
    renderRandomStars();
    renderProjectiles();
    renderHexagons();

    processProjectiles();
    processHexagons();
    processCollisions();

    renderActiveTurrets();
    renderStaticUI();
}


/**
 * Gets the name of the color of the given pixel in the specified color map.
 * 
 * @param X coordinate of the pixel.
 * @param Y coordinate of the pixel.
 * @param Color map.
 * @return Color name.
 */
std::string Tema1::getPixelColor(int x, int y, std::unordered_map<std::string, glm::vec3> colorMap) {

    float pixel[3];
    glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, pixel);

    pixel[0] = (float) round2(pixel[0]);
    pixel[1] = (float) round2(pixel[1]);
    pixel[2] = (float) round2(pixel[2]);

    std::string colorName = "";

    for (const auto& c : colorMap) {
        colorName = c.first;

        auto& colorCoords = this->colors[colorName];

        if (colorCoords.x == pixel[0] && colorCoords.y == pixel[1] && colorCoords.z == pixel[2]) {
            return colorName;
        }
    }

    return "";
}


/**
 * Detects collisions between objects.
 */
void Tema1::processCollisions() {

    for (int i = 0; i < this->gridX; i++) {
        for (int j = 0; j < this->gridY; j++) {

            auto turret = this->grid[i][j];

            for (int k = 0; k < hexagons[i].size(); k++) {

                auto distance = glm::distance(turret->getX(), hexagons[i][k]->getX());
                auto radiusSum = this->turretSize * 0.9 + this->bigHexagonSize;

                if (distance < radiusSum) {
                    
                    turret->setHealth(0);
                    turret->setColorName("Black");
                }
            }

        }
    
    }

    for (int i = 0; i < this->gridX; i++) {

        for (int j = 0; j < this->projectiles[i].size(); j++) {

            auto projectile = this->projectiles[i][j];

            for (int k = 0; k < hexagons[i].size(); k++) {

                auto distance = glm::distance(projectile->getX(), hexagons[i][k]->getX());
                auto radiusSum = this->starSize / 2 + this->bigHexagonSize;

                if (distance < radiusSum && hexagons[i][k]->getColorName() == projectile->getColorName()) {

                    this->projectiles[i].erase(std::remove(this->projectiles[i].begin(), this->projectiles[i].end(), projectile), this->projectiles[i].end());
                    j--;

                    int health = hexagons[i][k]->getHealth();
                    health--;

                    hexagons[i][k]->setHealth(health);

                    if (health == 0) {
                        hexagons[i][k]->setColorName("Black");
                    }
                }
            }
        }
    }
}


/**
 * Handles projectile rendering.
 */
void Tema1::renderProjectiles() {

    for (int i = 0; i < this->projectiles.size(); i++) {
        for (int j = 0; j < this->projectiles[i].size(); j++) {

            auto projectile = this->projectiles[i][j];

            int scaleX = projectile->getScale();
            int scaleY = scaleX;

            float rotation = projectile->getRotation();

            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(projectile->getX(), projectile->getY());
            modelMatrix *= transform2D::Scale(scaleX, scaleY);
            modelMatrix *= transform2D::Rotate(glm::radians(rotation));
            RenderMesh2D(meshes[projectile->getModelName()], modelMatrix, colors[projectile->getColorName()]);
        }
    }
}


/**
 * Handles projectile spawning logic.
 */
void Tema1::spawnProjectiles() {

    float eps = 100.0;

    for (int i = 0; i < this->gridX; i++) {
        for (int j = 0; j < this->gridY; j++) {

            // Check if there is a hexagon with the same color on the same line.
            auto turret = this->grid[i][j];

            for (int k = 0; k < this->hexagons[i].size(); k++) {

                if (turret->getColorName() != hexagons[i][k]->getColorName()) {
                    continue;
                }

                
                if (turret->getX() + this->turretSize * 0.9 < hexagons[i][k]->getX() - eps) {

                    auto projectile = new Entity();

                    projectile->setColorName(turret->getColorName());
                    projectile->setModelName("star");
                    projectile->setHealth(1);
                    projectile->setRotation(0);
                    projectile->setScale(10.0f);

                    projectile->setX(turret->getX() + this->turretSize * 0.9);
                    projectile->setY(turret->getY());

                    this->projectiles[i].push_back(projectile);

                    break;
                }
            }
        }
    }
}


/**
 * Handles projectile animations.
 */
void Tema1::processProjectiles() {
    
    for (int i = 0; i < this->gridY; i++) {
        for (int j = 0; j < this->projectiles[i].size(); j++) {

            auto projectile = this->projectiles[i][j];

            float x = projectile->getX();
            float rotation = projectile->getRotation();

            x += this->deltaTimeSeconds * this->projectileSpeed;
            rotation -= this->deltaTimeSeconds * 2 * this->projectileSpeed;

            if (glm::degrees(rotation) <= -360.0) {
                rotation = 360;
            }
            
            projectile->setX(x);
            projectile->setRotation(rotation);

            if (x >= this->resolution.x) {
                this->projectiles[i].erase(std::remove(this->projectiles[i].begin(), this->projectiles[i].end(), projectile), this->projectiles[i].end());
                j--;
            }
        }
    }
}


/**
 * Renders the static elements of the user interface.
 */
void Tema1::renderStaticUI() {


    // Render the red rectangle.
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(20, 20);
    RenderMesh2D(meshes["rectangle"], modelMatrix, colors["Red"]);

    // Render the 3x3 grid.
    for (int i = 0; i < this->gridX; i++) {
        for (int j = 0; j < this->gridY; j++) {

            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(this->gridStartX + (this->squareWidth + this->gridSpace) * i,
                this->gridStartY + (this->squareWidth + this->gridSpace) * j);

            RenderMesh2D(meshes["square"], modelMatrix, colors["Green"]);
        }
    }

    // Render the hearts.
    for (int i = 0; i < this->lives; i++) {
        modelMatrix = glm::mat3(1);

        auto translateX = this->gridStartX / 2 + 5 * (this->squareWidth + this->gridSpace) + this->gridSpace * 2.5f * i;

        modelMatrix *= transform2D::Translate(translateX, 660.0f);
        modelMatrix *= transform2D::Scale(0.5f, 0.5f);

        RenderMesh2D(meshes["heart"], modelMatrix, colors["Red"]);
    }

    // Render the money.
    for (int i = 0; i < this->money; i++) {
        modelMatrix = glm::mat3(1);

        auto translateX = this->gridStartX / 2 + 5 * (this->squareWidth + this->gridSpace) + this->gridSpace * i;

        modelMatrix *= transform2D::Translate(translateX, 580.0f);
        modelMatrix *= transform2D::Scale(4.0f, 4.0f);
        RenderMesh2D(meshes["star"], modelMatrix, colors["LightGray"]);
    }

    // Render the outlines.
    for (int i = 0; i < this->turretColorPrice.size(); i++) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(this->gridStartX / 2 + (this->squareWidth + 2 * this->gridSpace) * i, this->buyMenuStartY);

        RenderMesh2D(meshes["squareOutline"], modelMatrix, colors["Black"]);
    }

    // Render the buyable turrets and prices.
    int i = 0;
    for (const auto& turret : this->turretColorPrice) {
        std::string color = turret.first;
        int price = turret.second;

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(10 + this->buyMenuStartX + (this->squareWidth + 2 * this->gridSpace) * i, this->buyMenuStartY + this->squareWidth / 2);
        modelMatrix *= transform2D::Scale(0.9f, 0.9f);

        RenderMesh2D(meshes["turret"], modelMatrix, colors[color]);

        // Render the star, "price" number of times under the coressponding turret.
        for (int j = 0; j < price; j++) {

            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(this->gridStartX / 1.5f + (this->squareWidth + 2 * this->gridSpace) * i + this->squareWidth / 3  * j, this->buyMenuStartY - 20);
            modelMatrix *= transform2D::Scale(4.0f, 4.0f);
            RenderMesh2D(meshes["star"], modelMatrix, colors["LightGray"]);
        }
        
        i++;
    }
}


/**
 * Renders the entity that is being dragged.
 */
void Tema1::renderDraggedEntity() {

    // Drag boolean is false, no turret is being dragged, exit function.
    if (!this->drag) {
        return;
    }

    // Render the dragged turret at the cursor's position.

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(this->mouseX, this->resolution.y - this->mouseY);
    RenderMesh2D(meshes[this->draggedEntity->getModelName()], modelMatrix, colors[this->draggedEntity->getColorName()]);
}


/**
 * Renders the turrets in the grid.
 */
void Tema1::renderActiveTurrets() {
    for (int i = 0; i < this->gridX; i++) {
        for (int j = 0; j < this->gridY; j++) {

            auto turret = grid[i][j];

            if (turret->getModelName() != "") {
                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(turret->getX(), turret->getY());

                float scaleX = turret->getScale();
                float scaleY = scaleX;

                if (turret->getHealth() == 0) {
                    
                    if (turret->getScale() > 0) {
                        scaleX -= this->deltaTimeSeconds;
                        scaleY = scaleX;

                        turret->setScale(scaleX);

                    }
                    else {

                        grid[i][j] = new Entity();
                        continue;
                    }
                }

                modelMatrix *= transform2D::Scale(scaleX, scaleY);
                RenderMesh2D(meshes[turret->getModelName()], modelMatrix, colors[turret->getColorName()]);
            }
            
        }
    }
}


/**
 * Generates coordinates for the pink stars. 
 */
void Tema1::spawnRandomStars() {
    
    if (this->stars.size() >= this->maxTotalRandomStars) {
        return;
    }

    int eps = 40;
    int minX = 0;
    int minY = 0;
    
    int maxX = this->resolution.x - eps;
    int maxY = this->resolution.y - eps;

    std::random_device rdX;
    std::mt19937 genX(rdX());
    std::uniform_int_distribution<> randX(minX, maxX);
    
    std::random_device rdY;
    std::mt19937 genY(rdY());
    std::uniform_int_distribution<> randY(minY, maxY);

    for (int i = 0; i < this->maxRandomStars; i++) {
        auto newStar = new Entity();

        // Generate random X, Y pair.
        int x = randX(genX);
        int y = randY(genY);

        newStar->setModelName("star");
        newStar->setColorName(this->randomStarsColor);
        newStar->setX(x);
        newStar->setY(y);

        this->stars.push_back(newStar);
    }
}


/**
 * Handles hexagon rendering logic.
 */
void Tema1::renderHexagons() {

    for (int i = 0; i < this->hexagons.size(); i++) {

        for (int j = 0; j < this->hexagons[i].size(); j++) {

            auto hexagon = this->hexagons[i][j];

            float scaleX = hexagon->getScale();
            float scaleY = scaleX;

            if (hexagon->getHealth() == 0) {

                if (hexagon->getScale() > 0) {

                    scaleX -= this->deltaTimeSeconds;
                    scaleY = scaleX;

                    hexagon->setScale(scaleX);
                }
                else {

                    this->hexagons[i].erase(std::remove(this->hexagons[i].begin(), this->hexagons[i].end(), hexagon), this->hexagons[i].end());
                    j--;

                    this->score++;

                }
            }

            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(hexagon->getX(), hexagon->getY());
            modelMatrix *= transform2D::Scale(scaleX * 0.7f, scaleY * 0.7f);
            modelMatrix *= transform2D::Rotate(glm::radians(-45.0));
            RenderMesh2D(meshes[hexagon->getModelName()], modelMatrix, colors["Turquoise"]);

            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(hexagon->getX(), hexagon->getY());
            modelMatrix *= transform2D::Scale(scaleX, scaleY);
            modelMatrix *= transform2D::Rotate(glm::radians(-45.0));
            RenderMesh2D(meshes[hexagon->getModelName()], modelMatrix, colors[hexagon->getColorName()]);

        }
    }
}


/**
 * Handles the movement of the hexagons.
 */
void Tema1::processHexagons() {

    for (int i = 0; i < this->gridY; i++) {

        for (int j = 0; j < hexagons[i].size(); j++) {

            auto hexagon = hexagons[i][j];

            float x = hexagon->getX();
            x = x - this->deltaTimeSeconds * this->hexagonSpeed * 25;

            if (hexagon->getHealth() > 0) {
                hexagon->setX(x);
            }
            

            if (x < 0) {
                this->lives--;
                this->hexagons[i].erase(std::remove(this->hexagons[i].begin(), this->hexagons[i].end(), hexagon), this->hexagons[i].end());
                j--;
            }
        }
    }
    
}


/**
 * Generates coordinates for spawning the hexagons.
 */
void Tema1::spawnRandomHexagons() {

    int eps = 80;

    for (int i = 0; i < this->nrRandomHexagons; i++) {

        int min = 0;
        int max = this->gridY - 1;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> rand(min, max);

        // Pick a random line.
        int randY = rand(gen);

        // Pick a random color.
        max = this->entityColors.size() - 1;

        std::uniform_int_distribution<> randColor(min, max);

       
        int randColorIndex = randColor(gen);
        int j = 0;

        std::string color = "";

        for (const auto& c : this->entityColors) {

            if (j == randColorIndex) {
                color = c.first;
                break;
            }

            j++;
        }
        
        auto hexagon = new Entity("hexagon", color);

        float x = this->resolution.x + eps;
        float y = this->gridStartY + this->squareWidth / 2 + (this->gridY - randY - 1) * (this->squareWidth + this->gridSpace);

        hexagon->setHealth(this->maxHexagonHealth);
        hexagon->setScale(1.0);
        hexagon->setX(x);
        hexagon->setY(y);

        this->hexagons[randY].push_back(hexagon);
    }

}


/**
 * Handles pink star rendering.
 */
void Tema1::renderRandomStars() {

    for (const auto& star : this->stars) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(star->getX(), star->getY());
        modelMatrix *= transform2D::Scale(10.0f, 10.0f);
        RenderMesh2D(meshes[star->getModelName()], modelMatrix, colors[star->getColorName()]);
    }
}


void Tema1::FrameEnd()
{ 
}


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
}


void Tema1::OnKeyPress(int key, int mods)
{
}


void Tema1::OnKeyRelease(int key, int mods)
{
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    this->mouseX = mouseX;
    this->mouseY = mouseY;
    this->deltaMouseX = deltaX;
    this->deltaMouseY = deltaY;
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{

    if (button == 1) {
        processClickedStar(mouseX, mouseY);
    }

    if (button == 2) {
        processClickedCell(mouseX, mouseY);
    }


    // X coordinate of the bottom left corner of the first turret outline.
    auto startX = this->gridStartX / 2;

    // Check if the left button is pressed and if the cursor is placed on the buy menu.
    if (mouseX >= startX && this->resolution.y - mouseY >= this->buyMenuStartY && button == 1) {

        // Get the color of the clicked pixel.
        auto pixelColor = getPixelColor(mouseX, this->resolution.y - mouseY, this->entityColors);

        // If the clicked pixel isn't colored in an entity's color, exit.
        if (pixelColor == "") {
            this->drag = false;
            return;
        }

        if (this->money < this->turretColorPrice[pixelColor]) {
            return;
        }

        // Update the dragged entity fields.
        this->draggedEntity->setModelName("turret");
        this->draggedEntity->setColorName(pixelColor);
        this->drag = true;
    }
    else {

        this->drag = false;
    }
}


/**
 * Called when the player clicks on a cell in the grid.
 * 
 * @param Mouse X coordinate.
 * @param Mouse Y coordinate.
 */
void Tema1::processClickedCell(int mouseX, int mouseY) {
    
    auto cellCoords = getCellIndicesFromCoords(mouseX, this->resolution.y - mouseY);
    int cellX = cellCoords.x;
    int cellY = cellCoords.y;

    if (cellX == -1 && cellY == -1) {
        return;
    }

    auto turret = this->grid[cellX][cellY];

    if (turret->getModelName() == "") {
        return;
    }

    turret->setHealth(0);
    turret->setColorName("Black");
    
}


/**
 * Called when the player clicks a pink star.
 * 
 * @param Mouse X coordinate.
 * @param Mouse Y coordinate.
 */
void Tema1::processClickedStar(int mouseX, int mouseY) {


    std::unordered_map<std::string, glm::vec3> starColor = {
        {this->randomStarsColor, this->colors[this->randomStarsColor]}
    };

    // Check if the clicked pixel is colored in pink.
    if (getPixelColor(mouseX, this->resolution.y - mouseY, starColor) != this->randomStarsColor) {
        return;
    }

    auto minDistanceStar = this->stars[0];
    float minDistance = glm::distance(glm::vec3(minDistanceStar->getX(), minDistanceStar->getY(), 0),
        glm::vec3(mouseX, this->resolution.y - mouseY, 0));

    // Find closest star to the mouse click coords.
    for (const auto& star : this->stars) {

        float distance = glm::distance(glm::vec3(star->getX(), star->getY(), 0),
            glm::vec3(mouseX, this->resolution.y - mouseY, 0));

        if (distance < minDistance) {
            minDistance = distance;
            minDistanceStar = star;
        }
    }

    // Remove the closest star from the vector.
    this->stars.erase(std::remove(this->stars.begin(), this->stars.end(), minDistanceStar), this->stars.end());

    // Increment money.
    if (this->money < this->maxMoney) {
        this->money++;
    }   
}


/**
 * Takes X and Y screen coordinates and returns the cell's indices in the grid
 * if the given X and Y are inside a cell. Otherwise, returns -1, -1.
 *
 * @param X screen coordinate.
 * @param Y screen coordinate.
 * @return Cell indices.
 */
glm::ivec2 Tema1::getCellIndicesFromCoords(int x, int y) {


    // Compute bottom left corner coordinates of each square.
    auto currentSquareY = this->gridStartY + (this->gridY - 1) * (this->squareWidth + this->gridSpace);
    auto currentSquareX = this->gridStartX;

    for (int i = 0; i < this->gridX; i++) {

        // Compute bottom left corner coordinates of each square.
        currentSquareX = this->gridStartX;

        for (int j = 0; j < this->gridY; j++) {

            if (x >= currentSquareX && x <= currentSquareX + this->squareWidth &&
                y >= currentSquareY && y <= currentSquareY + this->squareWidth) {

                return glm::ivec2(i, j);
            }

            currentSquareX += this->squareWidth + this->gridSpace;
        }

        currentSquareY -= this->squareWidth + this->gridSpace;
    }

    return glm::ivec2(-1, -1);
}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    if (button == 1 && this->drag == true) {
        this->drag = false;


        // Get the indices of the cell where the mouse button was released while dragging
        // or (-1, -1) if the dragging was not stopped on a cell.
        auto cellCoords = getCellIndicesFromCoords(mouseX, this->resolution.y - mouseY);

        // Unpack.
        int cellX = cellCoords.x;
        int cellY = cellCoords.y;

        // If the turret was not dropped on a cell, exit.
        if (cellX == -1 && cellY == -1) {
            return;
        }

        // Process the drop.
        if (this->grid[cellX][cellY]->getModelName() == "") {

            auto price = this->turretColorPrice[this->draggedEntity->getColorName()];
            if (this->money < price) {
                return;
            }

            this->money -= price;

            auto newTurret = new Entity("turret", this->draggedEntity->getColorName());

            float x = this->gridStartX + this->squareWidth / 2;
            float y = this->gridStartY + (this->gridY - 1) * this->squareWidth + 2 * this->gridSpace;
            
            x = x + (this->squareWidth + this->gridSpace) * cellY;
            y = y - (this->squareWidth + this->gridSpace) * cellX + this->squareWidth / 2;

            newTurret->setX(x);
            newTurret->setY(y);
            newTurret->setScale(1.0);

            this->grid[cellX][cellY] = newTurret;
        }
        else {
            return;
        }

        return;
    }
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
    this->resolution.x = width;
    this->resolution.y = height;
}


/**
 * Function that rounds a value to the first 2 decimals.
 *
 * @param The number.
 * @return The rounded number.
 */
double Tema1::round2(double x) {
    return glm::round(x * 100) / 100;
}