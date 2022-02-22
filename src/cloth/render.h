/* **************************************************************************** *
 * AUTHOR:      Noah Krim
 * ASSIGNMENT:  GUI Cloth Sim
 * CLASS:       CS_08
 * ---------------------------------------------------------------------------- *
 * File: render.h
 *  Header file for OpenGL related functions
 * **************************************************************************** */

#ifndef RENDER_H
#define RENDER_H

#include <SFML/Graphics.hpp>

#include <OpenGL/gl3.h>
#include "../../libs/glm/vec3.hpp"
#include "../../libs/glm/mat4x4.hpp"

#include <string>

#include "render_mesh.h"

namespace render {
    // structs
    struct render_context {
        glm::mat4 proj_matrix;
        glm::mat4 view_matrix;
        glm::vec3 cam_pos;
        glm::vec3 cam_look;
        GLuint vao;
        GLuint vbo;
        GLuint shader_prog;
    };
    
    // static render context
    extern render_context context;

    // main renering entrypoints
    void init(sf::RenderTarget& target);
    void render(sf::RenderTarget& target, const mesh& draw_mesh, const glm::mat4& model_matrix, const glm::vec3& light_dir, const sf::Texture& tex_1, const sf::Texture& tex_2, const sf::Color& color);

    // mouse utility functions
    glm::vec3 mouse_to_world_ray(unsigned window_width, unsigned window_height, int mouse_pos_x, int mouse_pos_y);

    // opengl init utility functions
    GLuint createStaticBuffers(const vertex* vertex_data, GLint vertex_count);
    void createBuffers();
    void updateBuffers(const mesh& draw_mesh);
    GLuint compileShader(const std::string& path, GLenum shaderType);
    GLuint createShaderProgram(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
};

#endif