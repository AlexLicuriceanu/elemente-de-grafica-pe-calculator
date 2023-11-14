#pragma once

#include <unordered_map>
#include "components/simple_scene.h"
#include "./entity.h"
#include <vector>


namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
     public:
        Tema1();
        ~Tema1();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        // My functions.

        void loadAllMeshes();

        void renderStaticUI();
        void renderDraggedEntity();
        void renderActiveTurrets();
        void renderRandomStars();
        void renderHexagons();
        void renderProjectiles();

        double round2(double x);
        std::string getPixelColor(int x, int y, std::unordered_map<std::string, glm::vec3> colorMap);
        glm::ivec2 getCellIndicesFromCoords(int x, int y);

        void spawnRandomStars();
        void spawnRandomHexagons();
        void spawnProjectiles();

        void processHexagons();
        void processClickedStar(int mouseX, int mouseY);
        void processClickedCell(int mouseX, int mouseY);
        void processCollisions();
        void processProjectiles();
        
        

     protected:
        glm::mat3 modelMatrix;

        // Colors.
        std::unordered_map<std::string, glm::vec3> colors;
        std::unordered_map<std::string, glm::vec3> entityColors;
        std::unordered_map<std::string, int> turretColorPrice;


        // Grid.
        int gridX, gridY;
        float gridSpace;
        std::vector<std::vector<Entity*>> grid;

        float gridStartX, gridStartY;
        float buyMenuStartX, buyMenuStartY;


        // Pink stars.
        std::string randomStarsColor;
        std::vector<Entity*> stars;
        int maxRandomStars, maxTotalRandomStars;
        float randomStarsInterval;
        float randomStarsCounter;

        // Hexagons.
        std::vector<std::vector<Entity*>> hexagons;
        int nrRandomHexagons;
        int maxHexagonHealth;
        float hexagonSpeed;
        float randomHexagonsCounter;
        float randomHexagonsInterval;

        // Projectiles.
        std::vector<std::vector<Entity*>> projectiles;
        float projectileSpeed;
       

        // Game.
        int lives, money, maxMoney;
        bool drag;
        Entity* draggedEntity;
        int mouseX, mouseY, deltaMouseX, deltaMouseY;
        float deltaTimeSeconds;
        int score;
        glm::ivec2 resolution;


        // Mesh properties.
        float squareWidth;
        float rectangleWidth;
        float rectangleHeight;
        float heartSize;
        float bigHexagonSize;
        float smallHexagonSize;
        float turretSize;
        float starSize;
        
    };
}   // namespace m1
