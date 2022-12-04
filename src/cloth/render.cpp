/* **************************************************************************** *
 * AUTHOR:      Noah Krim
 * ASSIGNMENT:  GUI Cloth Sim
 * CLASS:       CS_08
 * ---------------------------------------------------------------------------- *
 * File: render.cpp
 *  Definition file for OpenGL related functions
 * **************************************************************************** */

#include "render.h"

#include "../../lib/glm/ext/matrix_transform.hpp"
#include "../../lib/glm/ext/matrix_clip_space.hpp"
#include "../../lib/glm/gtc/type_ptr.hpp"
#include "../../lib/glm/gtc/matrix_inverse.hpp"

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <streambuf>
#include <map>

render::render_context render::context {};

// uniform enum
enum UniformEnum {
    uModelMatrix = 0,
    uViewMatrix,
    uPerspectiveMatrix,
    uLightDir,
    uTex1,
    uTex2,
    uColor
};

enum Attribs {
    aPos = 0,
    aNorm = 2,
    aUv = 1
};

// map from uniform enum to uniform index
std::map<UniformEnum,GLint> uniform_map;


/* ============================================================================ *
 * Main Rendering Entrypoints
 * ============================================================================ */
void render::init(sf::RenderTarget& target) {
    target.setActive(true);

    // set camera pos
    context.cam_pos = {0, 0, 1.0f};
    context.cam_look = {0,0,0};

    // compile shaders
    context.shader_prog = createShaderProgram("src/shaders/default.vert", "src/shaders/default.frag");
    // link shader attributes
    glBindAttribLocation(context.shader_prog, aPos, "aPos");
    glBindAttribLocation(context.shader_prog, aNorm, "aNorm");
    glBindAttribLocation(context.shader_prog, aUv, "aUv");
    // glBindAttribLocation(context.shader_prog, 3, "oUv");
    // construct uniform map
    uniform_map[uModelMatrix] = glGetUniformLocation(context.shader_prog, "uModelMatrix");
    uniform_map[uViewMatrix] = glGetUniformLocation(context.shader_prog, "uViewMatrix");
    uniform_map[uPerspectiveMatrix] = glGetUniformLocation(context.shader_prog, "uPerspectiveMatrix");
    uniform_map[uLightDir] = glGetUniformLocation(context.shader_prog, "uLightDir");
    uniform_map[uTex1] = glGetUniformLocation(context.shader_prog, "uTex1");
    uniform_map[uTex2] = glGetUniformLocation(context.shader_prog, "uTex2");
    uniform_map[uColor] = glGetUniformLocation(context.shader_prog, "uColor");

    // create buffers
    createBuffers();

    // deactivate the target's context
    target.setActive(false);
}

void render::render(sf::RenderTarget& target, const mesh& draw_mesh, const glm::mat4& model_matrix, const glm::vec3& light_dir, const sf::Texture& tex_1, const sf::Texture& tex_2, const sf::Color& color) {
    target.setActive(true);

    // set gl states
    // - depth
    glEnable(GL_DEPTH_TEST);
    glClearColor(0,0,0,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDepthFunc(GL_LESS);  
    // - texture
    glEnable(GL_TEXTURE_2D);
    // - face culling
    glDisable(GL_CULL_FACE);
    // - viewport
    glViewport(0, 0, target.getSize().x, target.getSize().y);

    // compute matrices
    // glm::mat4 model_m(1.0); // identity
    // model_m = glm::translate(model_m, {-8, 8, 0});
    context.view_matrix = glm::lookAt(context.cam_pos, context.cam_look, {0,1.0f,0});
    context.proj_matrix = glm::perspectiveFov(
                glm::radians(65.0f) /*fov*/, 
                (float)target.getSize().x, (float)target.getSize().y,
                0.01f, 100.0f);

    // compute lighting
    // glm::vec3 light_dir{2.0f, 2.0f, -10.0f};
    // light_dir = context.view_matrix*glm::vec4(light_dir,0);

    // drawing
    // - set shader program
    glUseProgram(context.shader_prog); 
    // - bind texture
    glActiveTexture(GL_TEXTURE0);
    sf::Texture::bind(&tex_1); 
    glUniform1i(uniform_map[uTex1], 0);
    glActiveTexture(GL_TEXTURE1);
    sf::Texture::bind(&tex_2); 
    glUniform1i(uniform_map[uTex2], 1);
    // - set uniforms
    glUniformMatrix4fv(uniform_map[uModelMatrix], 1, GL_FALSE, 
        (const float*)glm::value_ptr(model_matrix));
    glUniformMatrix4fv(uniform_map[uViewMatrix], 1, GL_FALSE, 
        (const float*)glm::value_ptr(context.view_matrix));
    glUniformMatrix4fv(uniform_map[uPerspectiveMatrix], 1, GL_FALSE, 
        (const float*)glm::value_ptr(context.proj_matrix));
    glUniform3fv(uniform_map[uLightDir], 1, (const float*)glm::value_ptr(light_dir));
    glUniform4f(uniform_map[uColor], color.r/255.0f, color.g/255.0f, color.b/255.0f, color.a/255.0f);
    // - update buffers
    glBindVertexArray(context.vao);
    updateBuffers(draw_mesh);
    // - draw mesh
    glDrawArrays(GL_TRIANGLES, 0, draw_mesh.size);

    // deactivate the target's context
    glBindVertexArray(0);
    target.resetGLStates();
    target.setActive(false);
}



/* ============================================================================ *
 * Mouse Utility Functions
 * ============================================================================ */
glm::vec3 render::mouse_to_world_ray(unsigned window_width, unsigned window_height, int mouse_pos_x, int mouse_pos_y) {
    float ray_x = (2.0f*mouse_pos_x)/window_width - 1.0f;
    float ray_y = 1.0f - (2.0f*mouse_pos_y)/window_height;
    glm::vec4 mouse_ray4(ray_x, ray_y, -1.0f, 1.0f);
    mouse_ray4 = glm::affineInverse(context.proj_matrix)*mouse_ray4;
    mouse_ray4.z = -1.0f;
    mouse_ray4.w = 0;
    mouse_ray4 = glm::affineInverse(context.view_matrix)*mouse_ray4;
    glm::vec3 mouse_ray(mouse_ray4);
    return glm::normalize(mouse_ray);
}



/* ============================================================================ *
 * Initialization Utility Functions 
 * - Modified from source: https://github.com/adrianderstroff/opengl-starter/
 * ============================================================================ */
// GLuint render::createStaticBuffers(const vertex* vertex_data, GLint vertex_count) {
//     // create the vertex array object that holds all vertex buffers
//     GLuint vao;
//     glGenVertexArrays(1, &vao);
//     glBindVertexArray(vao);

//     // create a vertex buffer that contains all vertex positions and copy the vertex positions into that buffer
//     GLuint vbo;
//     glGenBuffers(1, &vbo);
//     glBindBuffer(GL_ARRAY_BUFFER, vbo);
//     glBufferData(GL_ARRAY_BUFFER, vertex_count*sizeof(vertex), vertex_data, GL_STATIC_DRAW);

//     // we need to tell the buffer in which format the data is and we need to explicitly enable it
//     // vertex position
//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(offsetof(struct vertex, pos)));
//     glEnableVertexAttribArray(0);
//     // vertex norm
//     glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(offsetof(struct vertex, norm)));
//     glEnableVertexAttribArray(1);
//     // vertex uv
//     glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(offsetof(struct vertex, uv)));
//     glEnableVertexAttribArray(2);

//     return vao;
// }

void render::createBuffers() {
    // create the vertex array object that holds all vertex buffers
    glGenVertexArrays(1, &context.vao);
    glBindVertexArray(context.vao);

    // create a vertex buffer that contains all vertex positions and copy the vertex positions into that buffer
    glGenBuffers(1, &context.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, context.vbo);
    //glBufferData(GL_ARRAY_BUFFER, vertex_count*sizeof(render::vertex), vertex_data, GL_DYNAMIC_DRAW);

    // we need to tell the buffer in which format the data is and we need to explicitly enable it
    // vertex position
    glEnableVertexAttribArray(aPos);
    glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);//(void*)(offsetof(struct vertex, pos)));
    // vertex norm
    glEnableVertexAttribArray(aNorm);
    glVertexAttribPointer(aNorm, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)12);//(void*)(offsetof(struct vertex, norm)));
    // vertex uv
    glEnableVertexAttribArray(aUv);
    glVertexAttribPointer(aUv, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)24);//(void*)(offsetof(struct vertex, uv)));

    // unbind
    glBindVertexArray(0);
}

void render::updateBuffers(const mesh& draw_mesh) {
    glBindBuffer(GL_ARRAY_BUFFER, context.vbo);
    glBufferData(GL_ARRAY_BUFFER, draw_mesh.size*sizeof(render::vertex), draw_mesh.vertices, GL_DYNAMIC_DRAW);
}

GLuint render::compileShader(const std::string& path, GLenum shaderType) {
    // grab the contents of the file and store the source code in a string
    std::ifstream filestream(path);
    std::string shaderSource((std::istreambuf_iterator<char>(filestream)),
        std::istreambuf_iterator<char>());

    // create and compile the shader
    GLuint shaderHandle = glCreateShader(shaderType);
    const char* shaderSourcePtr = shaderSource.c_str();
    glShaderSource(shaderHandle, 1, &shaderSourcePtr, NULL);
    glCompileShader(shaderHandle);

    // check if compilation was successful
    int  success;
    char infoLog[512];
    glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shaderHandle, 512, NULL, infoLog);
        std::cerr << "Error while compiling shader\n" << infoLog << std::endl;
    }

    // return the shader handle
    return shaderHandle;
}

GLuint render::createShaderProgram(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) {
    // create and compile shaders
    GLenum vertexShader = compileShader(vertexShaderPath, GL_VERTEX_SHADER);
    GLenum fragmentShader = compileShader(fragmentShaderPath, GL_FRAGMENT_SHADER);

    // create a shader program, attach both shaders and link them together
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // check for errors while linking the shaders together
    int  success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "Error while linking shaders\n" << infoLog << std::endl;
    }

    // after creating the shader program we don't need the two shaders anymore
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // return the shader program handle
    return shaderProgram;
}