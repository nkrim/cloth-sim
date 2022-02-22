#ifndef RENDER_MESH_H
#define RENDER_MESH_H

#include <OpenGL/gl3.h>

#include "../../libs/glm/vec3.hpp"

namespace render {
    struct vertex {
        glm::vec3 pos;
        glm::vec3 norm;
        glm::vec2 uv;
    };
    struct mesh {
        vertex* vertices;
        GLint size;
    };
}

#endif