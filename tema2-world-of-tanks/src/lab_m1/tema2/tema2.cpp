#include "lab_m1/tema2/tema2.h"
#include "lab_m1/tema2/transform3D.h"

#include <vector>
#include <string>
#include <iostream>
#include <random>

using namespace std;
using namespace m1;


Tema2::Tema2()
{

    // Set up game parameters.
    score = 0;
    timeLimit = 60.0f;
    elapsedTime = 0;
    freezeScene = false;

    movementStates = { "GoingForward", "GoingBackward", "InPlaceRotationLeft", "InPlaceRotationRight" };

    maxEnemyMovementTime = 3;
    currentEnemyMovementTime = 0;

    nrEnemiesPerWave = 5;
    minPlayerEnemyDistance = 9;
    projectileSpeed = 10;

    enemyTankSpeed = 2;
    enemyCurrentCannonCooldown = 3;
    enemyMinCannonCooldown = enemyCurrentCannonCooldown;

    // Set up colors.
    playerBodyColor = glm::vec3(0.05f, 0.15f, 0.03f);
    playerTurretColor = glm::vec3(0.1f, 0.27f, 0.06f);
    cannonColor = glm::vec3(0.77f, 0.77f, 0.77f);
    wheelsColor = glm::vec3(0.77f, 0.77f, 0.77f);
    groundColor = glm::vec3(0.34f, 0.57f, 0.31f);
    skyColor = glm::vec3(0.67f, 0.84f, 0.9f);
    projectileColor = glm::vec3(0, 0, 0);
    buildingColor = glm::vec3(0.7f, 0.7f, 0.7f);
    enemyBodyColor = glm::vec3(0.7f, 0, 0);
    enemyTurretColor = glm::vec3(1, 0, 0);

    // Set up the player's tank.
    maxPlayerHealth = 8;
    maxEnemyHealth = 4;
    tankSpeed = 5;
    tankLength = 17.8f;
    tankWidth = 8.0f;
    tankHeight = 5.8f;
    minCannonCooldown = 1;
    currentCannonCooldown = minCannonCooldown;

    player = new Entity();
    player->setHealth(maxPlayerHealth);
    player->setScale(0.1f);
    player->setX(0);
    player->setY(0);
    player->setZ(0);
    player->setBodyRotation(0);
    player->setTurretRotation(0);
    player->setSpeed(tankSpeed);
    player->setCurrentCannonCooldown(currentCannonCooldown);
    player->setMinCannonCooldown(minCannonCooldown);
    player->setBbCoordinates(0, 0.35f, 0);
    player->setBbWidth(tankWidth * player->getScaleX());
    player->setBbHeight(tankHeight * player->getScaleY());
    player->setBbLength(tankLength * player->getScaleZ());


    // Set level parameters.
    levelSizeX = 25.0f;
    levelSizeZ = 25.0f;
    maxBuildingScaleX = 7.0f;
    maxBuildingScaleY = 7.0f;
    maxBuildingScaleZ = 7.0f;
    nrBuildings = 6;
    maxProjectileTime = 3;
    rotationSpeed = 90.0f;
}


Tema2::~Tema2()
{
}

void Tema2::Init()
{
    // Load the shader.
    {
        Shader * shader = new Shader("ShaderTema");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // Set up the camera.
    camera = new implemented::Camera();
    camera->Set(glm::vec3(0, 1, 2), glm::vec3(0, 0.7, 0), glm::vec3(0, 1, 0));

    float distance = glm::distance(camera->position, player->getPosition());
    camera->distanceToTarget = distance;

    projectionMatrix = glm::perspective(RADIANS(50), window->props.aspectRatio, 0.01f, 200.0f);


    LoadAllModels();
    GenerateLevel();

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


    this->deltaTime = deltaTimeSeconds;


    if (player->getHealth() == 0 && freezeScene == false) {
        cout << "You died. Score: " << score << endl;
        freezeScene = true;
    }

    if (elapsedTime >= timeLimit && freezeScene == false) {

        cout << "Out of time. Score: " << score << endl;
        freezeScene = true;

    }
    else if (freezeScene == false) {
        elapsedTime += deltaTimeSeconds;
    }


    // Handle cannon cooldown. Default is 1 second intervals.
    if (player->getCurrentCannonCooldown() < player->getMinCannonCooldown()) {
        player->setCurrentCannonCooldown(player->getCurrentCannonCooldown() + deltaTimeSeconds);
    }


    // Render the level objects and the player's tank.
    RenderLevel();
    RenderPlayer();

    // Render and process the movement of projectiles this frame.
    RenderProjectiles();
    ProcessProjectiles();

    // Render and process enemies.
    GenerateEnemies();
    RenderEnemies();
    ProcessEnemies();

    // Detect collisions between all objects in the level.
    ProcessCollisions();

}


void Tema2::FrameEnd()
{
    //DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}


void Tema2::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix,
    const glm::vec3 &color, int health, int maxHealth, bool variableColor)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    
    // Set shader Color uniform.
    int colorLocation = glGetUniformLocation(shader->program, "Color");
    glUniform3fv(colorLocation, 1, glm::value_ptr(color));

    // Set shader Health uniform.
    int healthLocation = glGetUniformLocation(shader->program, "Health");
    glUniform1i(healthLocation, health);

    // Set shader MaxHealth uniform.
    int maxHealthLocation = glGetUniformLocation(shader->program, "MaxHealth");
    glUniform1i(maxHealthLocation, maxHealth);

    // Set shader VariableColor uniform.
    int VariableColorLocation = glGetUniformLocation(shader->program, "VariableColor");
    glUniform1i(VariableColorLocation, (int) variableColor);
    
    mesh->Render();
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


        RenderMesh(meshes["body"], shaders["ShaderTema"], modelMatrix,
            playerBodyColor, player->getHealth(), maxPlayerHealth, false);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(x, y, z);
        modelMatrix *= transform3D::Scale(scaleX, scaleY, scaleZ);
        modelMatrix *= transform3D::RotateOY(-glm::radians(bodyRotation));

        RenderMesh(meshes["wheels"], shaders["ShaderTema"], modelMatrix,
            wheelsColor, player->getHealth(), player->getHealth(), false);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(x, y, z);
        modelMatrix *= transform3D::RotateOY(-glm::radians(turretRotation));
        modelMatrix *= transform3D::Translate(0, 0, -0.6);
        modelMatrix *= transform3D::Scale(scaleX, scaleY, scaleZ);

        RenderMesh(meshes["cannon"], shaders["ShaderTema"], modelMatrix,
            cannonColor, player->getHealth(), player->getHealth(), false);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(x, y, z);
        modelMatrix *= transform3D::Scale(scaleX, scaleY, scaleZ);
        modelMatrix *= transform3D::RotateOY(-glm::radians(turretRotation));

        RenderMesh(meshes["turret"], shaders["ShaderTema"], modelMatrix,
            playerTurretColor, player->getHealth(), maxPlayerHealth, false);
    }
}


void Tema2::GenerateLevel() {

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<float> randX(-levelSizeX, levelSizeX);
    std::uniform_real_distribution<float> randZ(-levelSizeZ, levelSizeZ);
    std::uniform_real_distribution<float> randScaleX(1, maxBuildingScaleX);
    std::uniform_real_distribution<float> randScaleY(1.5f, maxBuildingScaleY);
    std::uniform_real_distribution<float> randScaleZ(1, maxBuildingScaleZ);

    for (int i = 0; i < nrBuildings; i++) {

        // Generate random XZ coordinate pair.
        float x = randX(gen);
        float z = randZ(gen);

        // Generate the scale of the building.
        

        float scaleX = randScaleX(gen);
        float scaleY = randScaleY(gen);
        float scaleZ = randScaleZ(gen);

        float e = 1.5f;

        float tankRadius = player->getScaleX() * tankWidth + player->getScaleZ() * tankLength;
        tankRadius = player->getBbWidth() + player->getBbLength();

        if (glm::distance(glm::vec3(x, 0, z), glm::vec3(0, 0, 0)) < tankRadius + e) {
            continue;
        }

        auto building = new Entity();

        building->setPosition(x, 0.35, z);
        building->setScaleX(scaleX);
        building->setScaleY(scaleY);
        building->setScaleZ(scaleZ);

        building->setBbCoordinates(x, 0.35, z);
        building->setBbWidth(scaleX);
        building->setBbHeight(scaleY);
        building->setBbLength(scaleZ);

        buildings.push_back(building);
    }
}

void Tema2::RenderLevel() {
    
    // Render the ground.
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Scale(1.5 * levelSizeX / 25, 1, 1.5 * levelSizeZ / 25);

        RenderMesh(meshes["plane"], shaders["ShaderTema"], modelMatrix,
            groundColor, 0, 0, false);
    }

    // Render the buildings.
    {
        for (const auto& building : buildings) {

            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix *= transform3D::Translate(building->getX(), building->getY(), building->getZ());
            modelMatrix *= transform3D::Scale(building->getScaleX(), building->getScaleY(), building->getScaleZ());

            RenderMesh(meshes["box"], shaders["ShaderTema"], modelMatrix,
                buildingColor, 0, 0, false);
        }
    }
}

void Tema2::ProcessProjectiles() {
    
    if (freezeScene) {
        return;
    }

    for (int i = 0; i < projectiles.size(); i++) {

        auto projectile = projectiles[i];
        float timeLeft = projectile->getTimeLeft() - this->deltaTime;

        if (timeLeft <= 0) {

            // Delete projectile.
            projectiles.erase(std::remove(projectiles.begin(), projectiles.end(), projectile), projectiles.end());
            i--;
            continue;
        }

        float delta = projectile->getSpeed() * this->deltaTime;

        projectile->setX(projectile->getX() + delta * sin(glm::radians(projectile->getTurretRotation())));
        projectile->setZ(projectile->getZ() - delta * cos(glm::radians(projectile->getTurretRotation())));

        projectile->setBbCoordinates(projectile->getPosition());

        projectile->setTimeLeft(timeLeft);
    }
}


void Tema2::RenderEnemies() {

    for (auto& enemy : enemies) {

        float x = enemy->getX();
        float y = enemy->getY();
        float z = enemy->getZ();

        float scaleX = enemy->getScaleX();
        float scaleY = enemy->getScaleY();
        float scaleZ = enemy->getScaleZ();
        float bodyRotation = enemy->getBodyRotation();
        float turretRotation = enemy->getTurretRotation();

        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix *= transform3D::Translate(x, y, z);
            modelMatrix *= transform3D::Scale(scaleX, scaleY, scaleZ);
            modelMatrix *= transform3D::RotateOY(-glm::radians(bodyRotation));


            RenderMesh(meshes["body"], shaders["ShaderTema"], modelMatrix,
                enemyBodyColor, enemy->getHealth(), maxEnemyHealth, true);
        }

        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix *= transform3D::Translate(x, y, z);
            modelMatrix *= transform3D::Scale(scaleX, scaleY, scaleZ);
            modelMatrix *= transform3D::RotateOY(-glm::radians(bodyRotation));

            RenderMesh(meshes["wheels"], shaders["ShaderTema"], modelMatrix,
                wheelsColor, enemy->getHealth(), enemy->getHealth(), false);
        }

        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix *= transform3D::Translate(x, y, z);
            modelMatrix *= transform3D::RotateOY(-glm::radians(turretRotation));
            modelMatrix *= transform3D::Translate(0, 0, -0.6);
            modelMatrix *= transform3D::Scale(scaleX, scaleY, scaleZ);

            RenderMesh(meshes["cannon"], shaders["ShaderTema"], modelMatrix,
                cannonColor, enemy->getHealth(), enemy->getHealth(), false);
        }

        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix *= transform3D::Translate(x, y, z);
            modelMatrix *= transform3D::Scale(scaleX, scaleY, scaleZ);
            modelMatrix *= transform3D::RotateOY(-glm::radians(turretRotation));

            RenderMesh(meshes["turret"], shaders["ShaderTema"], modelMatrix,
                enemyTurretColor, enemy->getHealth(), maxEnemyHealth, true);
        }
    }
}

void Tema2::ProcessEnemies() {

    if (freezeScene) {
        return;
    }

    currentEnemyMovementTime += deltaTime;

    if (currentEnemyMovementTime >= maxEnemyMovementTime) {

        currentEnemyMovementTime = 0;

        std::random_device rd;
        std::mt19937 gen(rd());

        std::uniform_int_distribution<int> randStateIdx(0, movementStates.size() - 1);

        for (auto& enemy : enemies) {
            enemy->setState(movementStates[randStateIdx(gen)]);
        }
    }

    for (auto& enemy : enemies) {

        bool isInBounds = enemy->getX() < 1.5 * levelSizeX && enemy->getZ() < 1.5 * levelSizeZ
            && enemy->getX() > -1.5 * levelSizeX && enemy->getZ() > -1.5 * levelSizeZ;

        if (enemy->getState() == "GoingForward" && isInBounds) {
            enemy->moveForward(deltaTime);
        }
        else if (enemy->getState() == "GoingBackward" && isInBounds) {
            enemy->moveBackward(deltaTime);
        }
        else if (enemy->getState() == "InPlaceRotationLeft") {
            enemy->rotateLeft(deltaTime, rotationSpeed * 0.3f);
        }
        else if (enemy->getState() == "InPlaceRotationRight") {
            enemy->rotateRight(deltaTime, rotationSpeed * 0.3f);
        }

        if (glm::distance(enemy->getPosition(), player->getPosition()) < minPlayerEnemyDistance) {

            glm::vec3 directionToPlayer = glm::normalize(player->getPosition() - enemy->getPosition());

            // Calculate the angle between the turret direction and the direction to the player.
            float angleToPlayer = atan2(directionToPlayer.x, directionToPlayer.z);

            angleToPlayer -= glm::pi<float>();

            // Update the turret rotation to face the player
            enemy->setTurretRotation(glm::degrees(-angleToPlayer));


            if (enemy->getCurrentCannonCooldown() >= enemy->getMinCannonCooldown()) {

                enemy->setCurrentCannonCooldown(0);
                SpawnProjectile(enemy);
            }
        }

        if (enemy->getCurrentCannonCooldown() < enemy->getMinCannonCooldown()) {
            enemy->setCurrentCannonCooldown(enemy->getCurrentCannonCooldown() + deltaTime);
        }
    }

}

void Tema2::GenerateEnemies() {

    std::random_device rd;
    std::mt19937 gen(rd());
    
    std::uniform_real_distribution<float> randX(-levelSizeX, levelSizeX);
    std::uniform_real_distribution<float> randZ(-levelSizeZ, levelSizeZ);
    std::uniform_int_distribution<int> randStateIdx(0, movementStates.size()-1);


    for (int i = enemies.size(); i < nrEnemiesPerWave; i++) {

        float x = randX(gen);
        float z = randZ(gen);

        std::string state = movementStates[randStateIdx(gen)];

        auto enemy = new Entity();

        enemy->setHealth(maxEnemyHealth);
        enemy->setScale(0.1f);
        enemy->setX(x);
        enemy->setY(0);
        enemy->setZ(z);
        enemy->setBodyRotation(0);
        enemy->setTurretRotation(0);
        enemy->setSpeed(enemyTankSpeed);

        enemy->setBbCoordinates(x, 0.35f, z);
        enemy->setBbWidth(tankWidth * enemy->getScaleX());
        enemy->setBbHeight(tankHeight * enemy->getScaleY());
        enemy->setBbLength(tankLength * enemy->getScaleZ());

        enemy->setState(state);
        enemy->setCurrentCannonCooldown(enemyMinCannonCooldown);
        enemy->setMinCannonCooldown(enemyCurrentCannonCooldown);

        enemies.push_back(enemy);
    }
}


void Tema2::ProcessCollisions() {
    
    // Check for collisions between player's tank, enemy tanks and buildings.
    for (auto& building : buildings) {

        if (checkSphereBoxCollision(player, building)) {

            glm::vec3 difference = player->getBbCoordinates() - building->getBbCoordinates();
            
            // Find penetration depth.
            auto p1 = player->getBbLength() + glm::max(building->getBbLength(), building->getBbWidth())
                - glm::distance(player->getBbCoordinates(), building->getBbCoordinates());

            auto p = p1 * glm::normalize(difference);

            player->setBbCoordinates(player->getBbCoordinates() + p * 0.01f);
            player->setPosition(player->getPosition() + p * 0.01f);

            camera->position = camera->position + p * 0.01f;
        }

        for (auto& enemy : enemies) {

            if (checkSphereBoxCollision(enemy, building)) {

                glm::vec3 difference = enemy->getBbCoordinates() - building->getBbCoordinates();

                // Find penetration depth.
                auto p1 = enemy->getBbLength() + glm::max(building->getBbLength(), building->getBbWidth())
                    - glm::distance(enemy->getBbCoordinates(), building->getBbCoordinates());

                auto p = p1 * glm::normalize(difference);

                enemy->setBbCoordinates(enemy->getBbCoordinates() + p * 0.01f);
                enemy->setPosition(enemy->getPosition() + p * 0.01f);
            }
        }
    }

    // Check for collisions between projectiles and buildings.
    for (int i = 0; i < projectiles.size(); i++) {

        // Check for enemy projectiles colliding with player.
        if (checkSphereCollision(player, projectiles[i])) {

            // Delete projectile.
            projectiles.erase(std::remove(projectiles.begin(), projectiles.end(), projectiles[i]), projectiles.end());
            i--;

            // Decrement player's health.
            player->setHealth(player->getHealth() - 1);
            continue;
        }

        for (auto& building : buildings) {

            if (checkSphereBoxCollision(projectiles[i], building)) {

                // Delete projectile.
                projectiles.erase(std::remove(projectiles.begin(), projectiles.end(), projectiles[i]), projectiles.end());
                i--;
                break;
            }
        }
    }

    // Check collisions between enemies and player or projectiles.
    for (int i = 0; i < enemies.size(); i++) {

        auto enemy = enemies[i];

        // Check collision of player with enemy.
        if (checkSphereCollision(enemy, player)) {

            glm::vec3 difference = player->getBbCoordinates() - enemy->getBbCoordinates();

            // Find penetration depth.
            auto p1 = player->getBbLength() + enemy->getBbLength()
                - glm::distance(player->getBbCoordinates(), enemy->getBbCoordinates());

            auto p = p1 * glm::normalize(difference);

            player->setBbCoordinates(player->getBbCoordinates() + p * 0.01f);
            player->setPosition(player->getPosition() + p * 0.01f);

            enemy->setBbCoordinates(enemy->getBbCoordinates() - p * 0.01f);
            enemy->setPosition(enemy->getPosition() - p * 0.01f);

            camera->position = camera->position + p * 0.01f;
        }

        // Check collision of enemy with projectile.
        for (int j = 0; j < projectiles.size(); j++) {

            if (checkSphereCollision(enemy, projectiles[j])) {

                // Delete projectile.
                projectiles.erase(std::remove(projectiles.begin(), projectiles.end(), projectiles[j]), projectiles.end());
                j--;

                enemy->setHealth(enemy->getHealth() - 1);

                if (enemy->getHealth() <= 0) {

                    score++;

                    // Increase player health.
                    int health = player->getHealth();

                    if (health < maxPlayerHealth) {
                        health++;
                    }

                    player->setHealth(health);

                    enemies.erase(std::remove(enemies.begin(), enemies.end(), enemies[i]), enemies.end());
                    i--;
                }

                break;
            }
        }
    }
}


bool Tema2::checkSphereBoxCollision(Entity* sphere, Entity* box) {

    glm::vec3 closestPoint = glm::clamp(sphere->getBbCoordinates(),
        box->getBbCoordinates() - 0.5f * glm::vec3(box->getBbWidth(), box->getBbHeight(), box->getBbLength()),
        box->getBbCoordinates() + 0.5f * glm::vec3(box->getBbWidth(), box->getBbHeight(), box->getBbLength()));

    float distance = glm::distance(sphere->getBbCoordinates(), closestPoint);
    return distance < 0.4f * (sphere->getBbLength());
}


bool Tema2::checkSphereCollision(Entity* entity1, Entity* entity2) {

    float distance = glm::distance(entity1->getBbCoordinates(), entity2->getBbCoordinates());
    float sum = 0.5f * (entity1->getBbLength() + entity2->getBbLength());
    return distance < sum;
}


void Tema2::RenderProjectiles() {
    
    for (const auto& projectile : projectiles) {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(projectile->getX(), projectile->getY(), projectile->getZ());
        modelMatrix *= transform3D::Scale(projectile->getScaleX(), projectile->getScaleY(), projectile->getScaleZ());

        RenderMesh(meshes["sphere"], shaders["ShaderTema"], modelMatrix,
            projectileColor, projectile->getHealth(), projectile->getHealth(), false);
    }
}


void Tema2::SpawnProjectile(Entity* source) {

    float cannonLength = 1.1;

    float x = source->getX() + cannonLength * sin(glm::radians(source->getTurretRotation()));
    float y = source->getY() + 0.45;
    float z = source->getZ() - cannonLength * cos(glm::radians(source->getTurretRotation()));
    auto projectile = new Entity();

    projectile->setPosition(x, y, z);
    projectile->setTimeLeft(maxProjectileTime);
    projectile->setTurretRotation(source->getTurretRotation());
    projectile->setScale(0.1f);
    projectile->setSpeed(projectileSpeed);

    projectile->setBbCoordinates(x, y, z);
    projectile->setBbWidth(0.1f);
    projectile->setBbHeight(0.1f);
    projectile->setBbLength(0.1f);

    projectiles.push_back(projectile);
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

    if (freezeScene) {
        return;
    }

    if (window->KeyHold(GLFW_KEY_W)) {

        // Calculate forward delta.
        float forwardDelta = player->getSpeed() * deltaTime;

        // Move the player.
        player->moveForward(deltaTime);

        // Move the camera.
        camera->MoveForward(forwardDelta);
    }

    if (window->KeyHold(GLFW_KEY_S)) {

        // Calculate backward delta.
        float backwardDelta = player->getSpeed() * deltaTime;

        // Move the player.
        player->moveBackward(deltaTime);

        // Move the camera.
        camera->MoveForward(-backwardDelta);
    }

    if (window->KeyHold(GLFW_KEY_A)) {

        // Rotate the player.
        player->rotateLeft(deltaTime, rotationSpeed);

        // Orbit the camera.
        camera->RotateThirdPerson_OY(glm::radians(rotationSpeed * deltaTime));
    }

    if (window->KeyHold(GLFW_KEY_D)) {

        // Rotate the player.
        player->rotateRight(deltaTime, rotationSpeed);

        // Orbit the camera.
        camera->RotateThirdPerson_OY(-glm::radians(rotationSpeed * deltaTime));
    }

    // Wrap around 360 degrees.
    player->setBodyRotation(fmod((fmod(player->getBodyRotation(), 360.0f) + 360.0f), 360.0f));
    player->setTurretRotation(fmod((fmod(player->getTurretRotation(), 360.0f) + 360.0f), 360.0f));
}


void Tema2::OnKeyPress(int key, int mods)
{

}


void Tema2::OnKeyRelease(int key, int mods)
{

}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    float sensitivityOX = 0.001f;
    float sensitivityOY = 0.001f;

    if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {

        player->setTurretRotation(player->getTurretRotation() + glm::degrees(2 * sensitivityOY * deltaX));
    }
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    if (freezeScene) {
        return;
    }

    if (button == 1) {
        if (player->getCurrentCannonCooldown() >= player->getMinCannonCooldown()) {
            SpawnProjectile(player);
            player->setCurrentCannonCooldown(0);
        }
    }
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{

}


void Tema2::OnWindowResize(int width, int height)
{

}
