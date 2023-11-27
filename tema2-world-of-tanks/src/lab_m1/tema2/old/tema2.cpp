#include "lab_m1/tema2/tema2.h"
#include "lab_m1/tema2/transform3D.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;




Tema2::Tema2()
{

    // Set up game parameters.
    score = 0;
    timeLimit = 60.0f;
    elapsedTime = 0;
    freezeScene = false;

    // Set up colors.
    playerBodyColor = glm::vec3(0.05f, 0.15f, 0.03f);
    playerTurretColor = glm::vec3(0.1f, 0.27f, 0.06f);
    cannonColor = glm::vec3(0.77f, 0.77f, 0.77f);
    wheelsColor = glm::vec3(0.77f, 0.77f, 0.77f);
    groundColor = glm::vec3(0.34f, 0.57f, 0.31f);
    skyColor = glm::vec3(0.67f, 0.84f, 0.9f);
    projectileColor = glm::vec3(0.9f, 0.9f, 0.9f);

    // Set up the player's tank.
    player = new Entity();
    player->setType("player");
    player->setHealth(5);
    player->setScale(0.3f);
    player->setX(0);
    player->setY(0);
    player->setZ(0);
    player->setBodyRotation(0);
    player->setTurretRotation(0);
    player->setSpeed(2);
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{
    renderCameraTarget = true;

    camera = new implemented::Camera();
    camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

    // Load the shader.
    /*
    {
        Shader * shader = new Shader("ShaderTema");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
    */


    //projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);

    LoadAllModels();
}


void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(skyColor.x, skyColor.y, skyColor.z, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();

    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema2::Update(float deltaTimeSeconds)
{
    
    
    // Timer.
    if (elapsedTime >= timeLimit && !freezeScene) {

        cout << "Score: " << score << endl;
        freezeScene = true;
    }
    else if (freezeScene == false) {
        elapsedTime += deltaTimeSeconds;
    }


    RenderPlayer();


    // Render ground.
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.01f, 0));
        //RenderMesh(meshes["plane"], shaders["Simple"], modelMatrix, groundColor);
    }


    if (renderCameraTarget)
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
        RenderMesh(meshes["sphere"], shaders["VertexNormal"], modelMatrix, glm::vec3(0,0,0));
    }
}


void Tema2::FrameEnd()
{
    //DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
    DrawCoordinateSystem();
}


void Tema2::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix, const glm::vec3 &color)
{
    if (!mesh || !shader || !shader->program)
        return;

    // render an object using the specified shader and the specified position
    glUseProgram(shader->program);


    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Get shader location for uniform mat4 "Model"
    int modelLocation = glGetUniformLocation(shader->program, "Model");

    // Set shader uniform "Model" to modelMatrix
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Get shader location for uniform mat4 "View"
    int viewLocation = glGetUniformLocation(shader->program, "View");

    // Set shader uniform "View" to viewMatrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Get shader location for uniform mat4 "Projection"
    int projectionLocation = glGetUniformLocation(shader->program, "Projection");

    // Set shader uniform "Projection" to projectionMatrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Set shader uniform "Color" to color.
    //int colorLocation = glGetUniformLocation(shader->program, "Color");
    //glUniform3fv(colorLocation, 1, glm::value_ptr(color));

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


void Tema2::RenderPlayer() {

    float x = player->getX();
    float y = player->getY();
    float z = player->getZ();

    float scaleX = player->getScaleX();
    float scaleY = player->getScaleY();
    float scaleZ = player->getScaleZ();
    float bodyRotation = player->getBodyRotation();
    float turretRotation = player->getTurretRotation();

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(x, y, z);
        modelMatrix *= transform3D::Scale(scaleX, scaleY, scaleZ);
        modelMatrix *= transform3D::RotateOY(-glm::radians(bodyRotation));
        

        RenderMesh(meshes["body"], shaders["Simple"], modelMatrix, playerBodyColor);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(x, y, z);
        modelMatrix *= transform3D::Scale(scaleX, scaleY, scaleZ);
        modelMatrix *= transform3D::RotateOY(-glm::radians(bodyRotation));

        RenderMesh(meshes["wheels"], shaders["Simple"], modelMatrix, wheelsColor);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(x, y, z);
        modelMatrix *= transform3D::Scale(scaleX, scaleY, scaleZ);
        RenderMesh(meshes["cannon"], shaders["Simple"], modelMatrix, cannonColor);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(x, y, z);
        modelMatrix *= transform3D::Scale(scaleX, scaleY, scaleZ);
        RenderMesh(meshes["turret"], shaders["Simple"], modelMatrix, playerTurretColor);
    }
}

void Tema2::LoadAllModels() {

    {
        Mesh* mesh = new Mesh("body");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "meshes"), "body.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("cannon");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "meshes"), "cannon.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("turret");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "meshes"), "turret.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("wheels");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "meshes"), "wheels.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("plane");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
}




void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    // Player's tank controls.


    float rotationSpeed = 90.0f;
    float cameraSpeed = 2.0f;

    if (window->KeyHold(GLFW_KEY_W)) {
        // Move the tank forward.
        float forwardDelta = player->getSpeed() * deltaTime;
        player->setX(player->getX() + forwardDelta * sin(glm::radians(player->getBodyRotation())));
        player->setZ(player->getZ() - forwardDelta * cos(glm::radians(player->getBodyRotation())));
    }

    if (window->KeyHold(GLFW_KEY_S)) {

        // Move the tank backward.
        float backwardDelta = player->getSpeed() * deltaTime;
        player->setX(player->getX() - backwardDelta * sin(glm::radians(player->getBodyRotation())));
        player->setZ(player->getZ() + backwardDelta * cos(glm::radians(player->getBodyRotation())));

        
    }

    if (window->KeyHold(GLFW_KEY_A)) {

        // Rotate the tank left.
        player->setBodyRotation(player->getBodyRotation() - rotationSpeed * deltaTime);
    }

    if (window->KeyHold(GLFW_KEY_D)) {
        
        // Rotate the tank right.
        player->setBodyRotation(player->getBodyRotation() + rotationSpeed * deltaTime);
    }
}


void Tema2::OnKeyPress(int key, int mods)
{
    
}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
