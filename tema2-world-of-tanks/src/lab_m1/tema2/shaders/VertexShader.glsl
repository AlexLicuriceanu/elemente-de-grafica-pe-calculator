#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec3 v_coordinate;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform vec3 Color;
uniform int Health;
uniform int MaxHealth;
uniform int VariableColor;

// Output
out vec3 frag_position;
out vec3 frag_normal;
out vec3 frag_coordinate;
out vec3 frag_color;

flat out int frag_health;
flat out int frag_max_health;
flat out int frag_variable_color;

void main() {
    
    // Compute deformation based on health values.
    float displacement = 0.1 * (MaxHealth - Health) * tan(v_position.x * 0.15 + v_position.z );
    vec3 deformed_position = v_position + displacement * normalize(v_normal);

    // Send output to fragment shader
    frag_position = deformed_position;
    frag_normal = v_normal;
    frag_coordinate = v_coordinate;
    frag_color = Color;

    frag_health = Health;
    frag_max_health = MaxHealth;
    frag_variable_color = VariableColor;


    // Compute gl_Position
    gl_Position = Projection * View * Model * vec4(deformed_position, 1.0);
}