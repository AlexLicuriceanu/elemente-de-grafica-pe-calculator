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

// Output
out vec3 frag_position;
out vec3 frag_normal;
out vec3 frag_coordinate;
out vec3 frag_color;

void main()
{
    // Send output to fragment shader
    frag_position = v_position;
    frag_normal = v_normal;
    frag_coordinate = v_coordinate;
    frag_color = Color;

    // Compute gl_Position
    gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}