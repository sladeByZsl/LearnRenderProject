#version 330 core
layout (location = 0) in vec3 aPos;

uniform vec2 uPosition;
uniform vec2 uScale;
uniform vec2 uRotation;

void main()
{
    vec2 scaled = aPos.xy * uScale;
    vec2 rotated = vec2(
        scaled.x * uRotation.x - scaled.y * uRotation.y,
        scaled.x * uRotation.y + scaled.y * uRotation.x
    );
    vec2 moved = rotated + uPosition;
    gl_Position = vec4(moved, aPos.z, 1.0);
}
