/* =============================================================================
 * DefaultShader
 * 
 * 
 * ===========================================================================*/
#pragma once

const char *const DEFAULT_VERTEX_SHADER =
R"(#version 100

attribute vec2 vertex_position;
attribute vec4 vertex_color;
attribute vec2 vertex_uv;

varying vec2 fragment_position;
varying vec4 fragment_color;
varying vec2 fragment_uv;

uniform mat4 P;

void main() {
	gl_Position.xy = (P * vec4(vertex_position, 0.0, 1.0)).xy;
	gl_Position.z = 0.0;
	gl_Position.w = 1.0;

	fragment_position = vertex_position;
	fragment_color = vertex_color;

	fragment_uv = vec2(vertex_uv.x, 1.0 - vertex_uv.y);
}
)";

const char *const DEFAULT_FRAGMENT_SHADER =
R"(#version 100

#ifdef GL_ES
precision mediump float;
#endif

varying vec2 fragment_position;
varying vec4 fragment_color;
varying vec2 fragment_uv;

uniform sampler2D u_texture;
uniform ivec2 u_size;
float sharpness = 2.5;

float sharpen(float pix_coord) {
    float norm = (fract(pix_coord) - 0.5) * 2.0;
    float norm2 = norm * norm;
    return floor(pix_coord) + norm * pow(norm2, sharpness) / 2.0 + 0.5;
}

void main() {
    vec2 vres = vec2(u_size.x, u_size.y);
    gl_FragColor = texture2D(u_texture, vec2(
        sharpen(fragment_uv.x * vres.x) / vres.x,
        sharpen(fragment_uv.y * vres.y) / vres.y
    )) * fragment_color;
    //vec4 texture_color = texture2D(u_texture, fragment_uv);
	//gl_FragColor = texture_color * fragment_color;
}
)";
