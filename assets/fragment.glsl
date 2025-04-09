#version 320 es

layout (location = 4) in mediump vec2 UV;
layout (location = 5) in mediump vec3 NORMAL;
out mediump vec4 COLOR;

uniform sampler2D TEXTURE;
uniform mediump float ROTATION;

void main()
{
    mediump vec2 uv = UV;
    uv.y = 1.0 - uv.y;
    COLOR = texture(TEXTURE, uv);

    mediump vec3 light_dir = normalize(vec3(0.0, 0.0, 1.0));
    mediump vec3 normal = normalize(NORMAL);

    mediump float shading = dot(normal, light_dir);
    shading = shading == 1.0 ? 1.0 : 1.0 - pow(2.0, -10.0 * shading);
    shading = max(0.6, shading);
    COLOR.rgb *= vec3(shading);
}