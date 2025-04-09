#version 320 es

precision mediump float;

// Model space vertex position
layout (location = 0) in vec3 VERTEX;
// Texture coordinates
layout (location = 1) in vec2 VERTEX_UV;
// Model space normal vector
layout (location = 2) in vec3 VERTEX_NORMAL;

// World -> Clip space matrix
uniform mat4 PROJECTION;
// Model -> World matrix
uniform mat4 WORLD;

// UV output to the fragment shader
layout (location = 4) out vec2 UV;
// Normal vector output to the fragment shader
layout (location = 5) out vec3 NORMAL;

void main()
{
    // Transform the vertex position to clip space
    gl_Position = PROJECTION * WORLD * vec4(VERTEX, 1.0);
    // Pass the UV coordinates to the fragment shader
    UV = VERTEX_UV;
    // Transform the normal vector to world space and pass it to the fragment shader
    vec3 normal = normalize(VERTEX_NORMAL);
    normal = normalize(vec3(WORLD * vec4(normal, 0.0)));
    NORMAL = normal;
}