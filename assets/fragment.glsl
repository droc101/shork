#version 320 es

precision mediump float;

layout (location = 4) in vec2 UV;
layout (location = 5) in vec3 NORMAL;
out mediump vec4 COLOR;

uniform mat4 WORLD;
uniform sampler2D TEXTURE;
uniform float ROTATION;

void main()
{
    vec2 uv = UV;
    uv.y = 1.0 - uv.y;
    COLOR = texture(TEXTURE, uv);

    vec3 light_dir = normalize(vec3(0.2, 0.0, 0.8));
    vec3 normal = normalize(NORMAL);
    normal = normalize(vec3(WORLD * vec4(normal, 0.0)));

    float shading = dot(normal, light_dir);
    shading = shading == 1.0 ? 1.0 : 1.0 - pow(2.0, -5.0 * shading);
    shading = max(0.6, shading);
    COLOR.rgb *= vec3(shading);
}