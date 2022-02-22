#version 120

uniform vec3 uLightDir;
uniform sampler2D uTex1;
uniform sampler2D uTex2;
uniform vec4 uColor;

varying vec3 oNorm;
varying vec2 oUv;

const float ambient = 0.25;
const float diffuse = 0.75;

void main() {
    float shading = ambient + diffuse*max(0, dot(normalize((gl_FrontFacing ? 1.0 : -1.0)*oNorm), -normalize(uLightDir)));
    vec4 sample1 = texture2D(uTex1, oUv);
    vec4 sample2 = texture2D(uTex2, oUv);
    vec3 colored_sample1 = sample1.rgb*uColor.rgb;
    sample1.rgb = mix(sample1.rgb, colored_sample1, uColor.a);
    sample1.rgb *= sample1.a;
    vec3 outcolor = mix(sample1.rgb, sample2.rgb, sample2.a);
    gl_FragColor = vec4(shading*outcolor, 1.0);
} 