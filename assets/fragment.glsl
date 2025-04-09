#version 320 es

precision mediump float;

// Albedo UV coordinates
layout (location = 4) in vec2 UV;
// World space normal vector
layout (location = 5) in vec3 NORMAL;

// Output color
out mediump vec4 COLOR;

// Albedo texture
uniform sampler2D ALBEDO_TEXTURE;
// Overlay texture
uniform sampler2D VERLAY_TEXTURE;
// Screen size in pixels/characters
uniform vec2 SCREEN_SIZE;

// Remap a value from one range to another
float map(float value, float min1, float max1, float min2, float max2) {
    return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}

void main()
{
    // Calculate basic UVs
    vec2 uv = UV;
    uv.y = 1.0 - uv.y;
    vec2 screen_uv = gl_FragCoord.xy / SCREEN_SIZE;
    screen_uv.y = 1.0 - screen_uv.y;

    // Shrink to center since the shark does not cover the whole screen
    // It's OK to go outside the 0-1 range as the texture is clamped
    screen_uv.x = map(screen_uv.x, 0.0, 1.0, -0.75, 1.75);
    screen_uv.y = map(screen_uv.y, 0.0, 1.0, -0.75, 1.75);

    COLOR = texture(ALBEDO_TEXTURE, uv);

    // Calculate the light direction
    vec3 light_dir = normalize(vec3(0.2, 0.0, 0.8));

    // Calculate the shading
    float shading = dot(NORMAL, light_dir);
    shading = shading == 1.0 ? 1.0 : 1.0 - pow(2.0, -5.0 * shading);
    shading = max(0.6, shading);
    COLOR.rgb *= vec3(shading);

    // Mix in the overlay texture
    vec4 overlay = texture(OVERLAY_TEXTURE, screen_uv);
    COLOR.rgb = mix(COLOR.rgb, overlay.rgb, 0.5 * overlay.a);
}