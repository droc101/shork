#version 320 es

layout (location = 0) in mediump vec3 VERTEX;
layout (location = 1) in mediump vec2 VERTEX_UV;
layout (location = 2) in mediump vec3 VERTEX_NORMAL;

uniform mediump mat4 PROJECTION;
uniform mediump mat4 WORLD;

layout (location = 4) out mediump vec2 UV;
layout (location = 5) out mediump vec3 NORMAL;

void main()
{
   gl_Position = PROJECTION * WORLD * vec4(VERTEX, 1.0);
   UV = VERTEX_UV;
   NORMAL = VERTEX_NORMAL;
}