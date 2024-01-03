#include "lab_m1/tema3/tema3.h"
#include "lab_m1/tema3/transform3D.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;


Tema3::Tema3()
{
    skyColor = glm::vec3(0, 0.05f, 0.15f);
    islandPosition = glm::vec3(0, 0, 0);
    moonPosition = glm::vec3(-5.5f, 3.5f, -10.5f);

    nrBoats = 4;
    waterScale = 50;

    GetSceneCamera()->SetPosition(glm::vec3(0, 3, 9));
}


Tema3::~Tema3()
{
}


void Tema3::Init()
{
    const string sourceTextureDir = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "textures");

    // Load moon texture.
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "moon.png").c_str(), GL_REPEAT);
        mapTextures["moon"] = texture;
    }

    // Load grass texture.
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "grass.png").c_str(), GL_REPEAT);
        mapTextures["grass"] = texture;
    }

    // Load grunge texture for lighthouse.
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "grunge.png").c_str(), GL_REPEAT);
        mapTextures["grunge"] = texture;
    }

    // Load blank texture.
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "empty.png").c_str(), GL_REPEAT);
        mapTextures["empty"] = texture;
    }

    // Load water texture.
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "water.png").c_str(), GL_REPEAT);
        mapTextures["water"] = texture;
    }

    // Load wood texture.
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "wood.png").c_str(), GL_REPEAT);
        mapTextures["wood"] = texture;
    }
    
    // Load flag texture.
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "flag.png").c_str(), GL_REPEAT);
        mapTextures["flag"] = texture;
    }

    // Load meshes
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Load the sphere mesh.
    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Load the cylinder mesh.
    {
        Mesh* mesh = new Mesh("cylinder");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "cylinder.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Create quad meshes.
    {
        vector<glm::vec3> vertices
        {
            glm::vec3(0.5f,   0.5f, 0.0f),
            glm::vec3(0.5f,  -0.5f, 0.0f),
            glm::vec3(-0.5f, -0.5f, 0.0f),
            glm::vec3(-0.5f,  0.5f, 0.0f),
        };

        vector<glm::vec3> normals
        {
            glm::vec3(0, 1, 1),
            glm::vec3(1, 0, 1),
            glm::vec3(1, 0, 0),
            glm::vec3(0, 1, 0)
        };

        vector<glm::vec2> textureCoords
        {
            glm::vec2(0.0f, 0.0f),
            glm::vec2(0.0f, 10.0f),
            glm::vec2(10.0f, 10.0f),
            glm::vec2(10.0f, 0.0f)
        };

        vector<unsigned int> indices =
        {
            0, 1, 3,
            1, 2, 3
        };

        Mesh* mesh = new Mesh("quad");
        mesh->InitFromData(vertices, normals, textureCoords, indices);
        meshes[mesh->GetMeshID()] = mesh;

        // For the normal quad with non repeating texture.
        textureCoords =
        {
            glm::vec2(0.0f, 0.0f),
            glm::vec2(0.0f, 1.0f),
            glm::vec2(1.0f, 1.0f),
            glm::vec2(1.0f, 0.0f)
        };

        mesh = new Mesh("quad_non_repeating");
        mesh->InitFromData(vertices, normals, textureCoords, indices);
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Create triangle mesh.
    {
        float sideLength = 1.0f;
        float height = sqrt(3.0f) * sideLength / 2.0f;

        vector<glm::vec3> vertices
        {
            glm::vec3(0.0f, height, 0.0f),
            glm::vec3(-sideLength / 2.0f, -height / 2.0f, 0.0f),
            glm::vec3(sideLength / 2.0f, -height / 2.0f, 0.0f)
        };

        vector<glm::vec3> normals
        {
            glm::vec3(0, 0, 1),
            glm::vec3(0, 0, 1),
            glm::vec3(0, 0, 1)
        };

        vector<glm::vec2> textureCoords
        {
            glm::vec2(0.0f, 1.0f),
            glm::vec2(0.0f, 0.0f),
            glm::vec2(1.0f, 0.0f)
        };

        vector<unsigned int> indices =
        {
            0, 1, 2
        };

        Mesh* mesh = new Mesh("triangle");
        mesh->InitFromData(vertices, normals, textureCoords, indices);
        meshes[mesh->GetMeshID()] = mesh;
    }


    // Create the shader.
    {
        Shader* shader = new Shader("ShaderTema");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // Generate boat entities.
    GenerateBoats();
}


void Tema3::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(skyColor.x, skyColor.y, skyColor.z, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();

    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema3::Update(float deltaTimeSeconds)
{
    
    // Render the water.
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(0, 0, 0);
        modelMatrix *= transform3D::Scale(waterScale, waterScale, waterScale);
        modelMatrix *= transform3D::RotateOX(glm::radians(90.0f));

        RenderSimpleMesh(meshes["quad"], shaders["ShaderTema"], modelMatrix, mapTextures["water"], true);
    }

    // Render the moon.
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(moonPosition.x, moonPosition.y, moonPosition.z);
        modelMatrix *= transform3D::Scale(2, 2, 2);
        RenderSimpleMesh(meshes["sphere"], shaders["ShaderTema"], modelMatrix, mapTextures["moon"], false);
    }

    // Render the island.
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(islandPosition.x, islandPosition.y, islandPosition.z);
        modelMatrix *= transform3D::Scale(2, 1, 2);
        RenderSimpleMesh(meshes["sphere"], shaders["ShaderTema"], modelMatrix, mapTextures["grass"], false);
    }

    // Render the lighthouse base.
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(islandPosition.x, islandPosition.y + 1.3f, islandPosition.z);
        modelMatrix *= transform3D::Scale(0.4f, 1.2f, 0.4f);
        RenderSimpleMesh(meshes["cylinder"], shaders["ShaderTema"], modelMatrix, mapTextures["grunge"], false);
    }

    // Render the lighthouse middle.
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(islandPosition.x, islandPosition.y + 2.7f, islandPosition.z);
        modelMatrix *= transform3D::Scale(0.38f, 0.2f, 0.38f);
        RenderSimpleMesh(meshes["cylinder"], shaders["ShaderTema"], modelMatrix, mapTextures["empty"], false);
    }

    // Render the lighthouse top.
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(islandPosition.x, islandPosition.y + 3, islandPosition.z);
        modelMatrix *= transform3D::Scale(0.4f, 0.15f, 0.4f);
        RenderSimpleMesh(meshes["cylinder"], shaders["ShaderTema"], modelMatrix, mapTextures["grunge"], false);
    }

    // Render the boats.
    RenderBoats();

    // Process the movement of the boats.
    ProcessBoats(deltaTimeSeconds);
}


void Tema3::GenerateBoats() {

    for (int i = 0; i < nrBoats; i++) {
        
        auto boat = new Entity();
        
        boat->setPosition(glm::vec3((i + 3) * 1.5 * pow(-1, i), 0.1f, i));
        boat->setScaleX(1);
        boat->setScaleY(0.5f);
        boat->setScaleZ(2);
        boat->setDirection(pow(-1, i));

        // Generate a random number from 0.5 to 0.8.
        float randomSpeed = 0.5f + static_cast<float>(rand()) / static_cast<float>(RAND_MAX / 0.3f);
        boat->setSpeed(randomSpeed);

        if (boat->getDirection() == 1) {
            boat->setHeading(360.0f);
        }
        else {
            boat->setHeading(180.0f);
        }

        boats.push_back(boat);
    }
}

void Tema3::RenderBoats() {
    
    for (auto boat : boats) {

        // Render the hull.
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(boat->getX(), boat->getY(), boat->getZ());
        modelMatrix *= transform3D::RotateOY(-glm::radians(boat->getHeading()));
        modelMatrix *= transform3D::Scale(boat->getScaleX(), boat->getScaleY(), boat->getScaleZ());
        RenderSimpleMesh(meshes["box"], shaders["ShaderTema"], modelMatrix, mapTextures["wood"], false);

        // Render the mast.
        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(boat->getX(), boat->getY(), boat->getZ());
        modelMatrix *= transform3D::RotateOY(-glm::radians(boat->getHeading()));
        modelMatrix *= transform3D::Scale(0.05f * boat->getScaleX(), 5 * boat->getScaleY(), 0.05f * boat->getScaleZ());
        RenderSimpleMesh(meshes["box"], shaders["ShaderTema"], modelMatrix, mapTextures["grunge"], false);

        // Render the flag.
        float offsetDistance = 0.15f;
        float offsetX = -offsetDistance * boat->getDirection() * sin(glm::radians(boat->getHeading()));
        float offsetZ = -offsetDistance * boat->getDirection() * cos(glm::radians(boat->getHeading()));
        
        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(boat->getX() + offsetX, boat->getY() + 1.25f, boat->getZ() - offsetZ);
        modelMatrix *= transform3D::RotateOY(glm::radians(90.0f));
        modelMatrix *= transform3D::RotateOY(-glm::radians(boat->getHeading()));
        modelMatrix *= transform3D::Scale(0.25f * boat->getScaleX(), 0.5f * boat->getScaleY(), 0.05f * boat->getScaleZ());
        RenderSimpleMesh(meshes["quad_non_repeating"], shaders["ShaderTema"], modelMatrix, mapTextures["flag"], false);

        // Render the sail.
        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(boat->getX(), boat->getY() + 0.5f, boat->getZ());
        modelMatrix *= transform3D::RotateOY(glm::radians(90.0f));
        modelMatrix *= transform3D::RotateOY(-glm::radians(boat->getHeading()));
        modelMatrix *= transform3D::Scale(1.6f, 0.8f, 1.2f);
        RenderSimpleMesh(meshes["triangle"], shaders["ShaderTema"], modelMatrix, mapTextures["empty"], false);
    }
}

void Tema3::ProcessBoats(float deltaTime) {

    for (auto boat : boats) {

        float rotationRate = 60.0f * boat->getSpeed();
        
        float angle = glm::radians(boat->getHeading());

        // Get the current XY coords.
        float currentX = boat->getX();
        float currentZ = boat->getZ();

        // Calculate the new angle after rotating.
        float deltaAngle = glm::radians(rotationRate * deltaTime);

        // For counter-clockwise rotation, make the deltaAngle negative.
        if (boat->getDirection() == 1) {
            deltaAngle = -deltaAngle;
        }

        // Calculate the new position.
        float newX = cos(deltaAngle) * currentX - sin(deltaAngle) * currentZ;
        float newZ = sin(deltaAngle) * currentX + cos(deltaAngle) * currentZ;

        // Update the position.
        boat->setPosition(newX, boat->getY(), newZ);

        // Compute new heading.
        float newHeading = glm::degrees(angle + deltaAngle);

        // Wrap around values.
        newHeading = fmod(newHeading, 360.0f);
        if (newHeading < 0.0f) {
            newHeading += 360.0f;
        }

        // Update the heading.
        boat->setHeading(newHeading);
    }
}


void Tema3::FrameEnd()
{

}


void Tema3::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture, bool translate)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    if (texture) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
    }
    
    if (translate) {
       
        auto elapsedTime = Engine::GetElapsedTime();

        glUniform1f(glGetUniformLocation(shader->program, "time"), elapsedTime);
    }
    else {
        glUniform1f(glGetUniformLocation(shader->program, "time"), 0);
    }

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}



void Tema3::OnInputUpdate(float deltaTime, int mods)
{
}


void Tema3::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Tema3::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema3::OnWindowResize(int width, int height)
{
}
