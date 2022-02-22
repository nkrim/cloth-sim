#version 120

attribute vec3 aPos;
attribute vec3 aNorm;
attribute vec2 aUv;

uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uPerspectiveMatrix;

varying vec3 oNorm;
varying vec2 oUv;

void main() {
    gl_Position = 
        (uPerspectiveMatrix*
            (uViewMatrix*
                (uModelMatrix*
                    vec4(aPos.x, aPos.y, aPos.z, 1.0))));
    oNorm = (uModelMatrix*vec4(aNorm, 0.0)).xyz;
    oUv = aUv;
}