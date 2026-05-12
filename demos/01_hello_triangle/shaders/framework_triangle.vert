#version 330 core
layout (location = 0) in vec3 aPos;

uniform vec2 uPosition;
uniform vec2 uScale;

void main()
{
    vec2 scaled = aPos.xy * uScale;
    vec2 moved = scaled + uPosition;
    gl_Position = vec4(moved, aPos.z, 1.0);
}
