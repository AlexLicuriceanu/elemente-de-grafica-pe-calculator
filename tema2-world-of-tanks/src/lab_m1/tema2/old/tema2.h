#pragma once

#include "components/simple_scene.h"
#include "lab_m1/tema2/lab_camera.h"
#include "lab_m1/tema2/entity.h"

#define Z_NEAR (.01f)
#define Z_FAR (200.0f)

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

        void RenderMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, const glm::vec3 &color);

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

     protected:
         implemented::Camera* camera;
         glm::mat4 projectionMatrix;
         bool renderCameraTarget;

        int score;
        float timeLimit;
        float elapsedTime;
        bool freezeScene;

        glm::vec3 groundColor;
        glm::vec3 playerBodyColor;
        glm::vec3 playerTurretColor;
        glm::vec3 wheelsColor;
        glm::vec3 cannonColor;
        glm::vec3 skyColor;
        glm::vec3 projectileColor;

        Entity* player;

    };
}   // namespace m1
