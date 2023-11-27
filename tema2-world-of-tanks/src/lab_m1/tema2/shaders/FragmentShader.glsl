#version 330

// Input
in vec3 frag_color;
in vec3 frag_normal;

flat in int frag_health;
flat in int frag_max_health;
flat in int frag_variable_color;

// Output
layout(location = 0) out vec4 out_color;

void main()
{
    // Write pixel out color

    if (frag_variable_color == 1) {
        out_color = vec4(frag_color - 0.15 * (frag_max_health - frag_health), 1);
    }
    else {
        out_color = vec4(frag_color, 1);
    }

}