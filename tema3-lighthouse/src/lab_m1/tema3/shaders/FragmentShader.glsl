#version 330

uniform sampler2D texture_1;
in vec2 frag_texture_coord;

layout(location = 0) out vec4 out_color;

uniform float time;

void main()
{
    // Speed of the water waves.
    float speed = 0.075;

    // Translate the texture coordinates based on time.
    vec2 translated_frag_texture_coord = frag_texture_coord + vec2(speed * time, 0.0);

    vec4 color;

    if (time != 0) {

        vec2 modulated_coords = translated_frag_texture_coord +
            vec2(sin(frag_texture_coord.x * 10) * 0.035, cos(frag_texture_coord.y * 10) * 0.05); 

        color = texture2D(texture_1, modulated_coords);

    }
    else {
        color = texture2D(texture_1, translated_frag_texture_coord);
    }

    out_color = color;
}
