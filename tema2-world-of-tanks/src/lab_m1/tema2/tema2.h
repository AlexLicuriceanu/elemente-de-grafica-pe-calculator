#pragma once

#include "components/simple_scene.h"
#include "lab_m1/tema2/tema2_camera.h"
#include "lab_m1/tema2/entity.h"
#include <vector>

namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
     public:
        Tema2();
        ~Tema2();


        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix,
            const glm::vec3 &color, int health, int maxHealth, bool variableColor);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        void LoadAllModels();
        void RenderPlayer();
        void GenerateLevel();
        void RenderLevel();

        void ProcessProjectiles();
        void RenderProjectiles();
        void SpawnProjectile(Entity* source);

        void RenderEnemies();
        void ProcessEnemies();
        void GenerateEnemies();

        void ProcessCollisions();
        bool checkSphereBoxCollision(Entity* sphere, Entity* box);
        bool checkSphereCollision(Entity* entity1, Entity* entity2);

    protected:
        implemented::Camera* camera;
        glm::mat4 projectionMatrix;

        int score;
        float timeLimit;
        float elapsedTime;
        bool freezeScene;
        float deltaTime;

        int nrBuildings;
        float rotationSpeed;

        float levelSizeX, levelSizeZ;
        float maxBuildingScaleX, maxBuildingScaleY, maxBuildingScaleZ;

        float maxProjectileTime;
        float projectileSpeed;
        float minCannonCooldown, currentCannonCooldown;
        
        float tankLength, tankWidth, tankHeight;

        glm::vec3 groundColor;
        glm::vec3 playerBodyColor;
        glm::vec3 playerTurretColor;
        glm::vec3 wheelsColor;
        glm::vec3 cannonColor;
        glm::vec3 skyColor;
        glm::vec3 projectileColor;
        glm::vec3 buildingColor;
        glm::vec3 enemyBodyColor;
        glm::vec3 enemyTurretColor;

        Entity* player;
        std::vector<Entity*> buildings;
        std::vector<Entity*> projectiles;
        std::vector<Entity*> enemies;
        std::vector<std::string> movementStates;

        int maxPlayerHealth, maxEnemyHealth;
        float maxEnemyMovementTime;
        float currentEnemyMovementTime;
        int nrEnemiesPerWave;
        int nrEnemiesAlive;

        float tankSpeed;
        float minPlayerEnemyDistance;
        float enemyTankSpeed;
        float enemyMinCannonCooldown;
        float enemyCurrentCannonCooldown;

    };
}   // namespace m1
