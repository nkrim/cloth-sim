/* **************************************************************************** *
 * AUTHOR:      Noah Krim
 * ASSIGNMENT:  GUI Cloth Sim
 * CLASS:       CS_08
 * ---------------------------------------------------------------------------- *
 * File: cloth.cpp
 *  Definition file for Cloth class
 * **************************************************************************** */

#include "cloth.h"

#include "render.h"
#include "../system/global-entities.h"

#include "../../libs/glm/common.hpp"
#include "../../libs/glm/vector_relational.hpp"
#include "../../libs/glm/geometric.hpp"
#include "../../libs/glm/gtc/matrix_inverse.hpp"
#include "../../libs/glm/gtx/intersect.hpp"
#include "../../libs/glm/ext/matrix_transform.hpp"
#include "../../libs/glm/gtx/rotate_vector.hpp"
#include "../../libs/glm/gtx/vector_angle.hpp"
#include "../../libs/glm/gtx/norm.hpp"

#include <iostream>
#include <cmath>
#include <limits>

using std::cout; using std::endl;

/* ============================================================================ *
 * Static Constant Definitions
 * ============================================================================ */
const Cloth::size_type 
Cloth::DEFAULT_SIDE_VERTEX_COUNT
    = 32;
const Cloth::size_type 
Cloth::PHYSICS_ITERATIONS
    = 16;
const float 
Cloth::DEFAULT_TIME_STEP 
    = 1.0f/60.0f;
const float 
Cloth::NEIGHBOR_FLEX_COEFF   
    = 0.01f;
const float 
Cloth::BENDING_FLEX_COEFF   
    = 0.05f;
const float 
Cloth::NEIGHBOR_TEAR_THRESH
    = 2.0f;
const float 
Cloth::FLOOR_PLANE_Y   
    //= -1.0*DEFAULT_SIDE_VERTEX_COUNT;
    = -1.15f;
const float 
Cloth::FLOOR_PLANE_FRICTION_COEFF   
    = 0.1f;


/* ============================================================================ *
 * Constructors
 * ============================================================================ */
Cloth::Cloth(sf::Vector2u frame_size, float scale, size_type side_vertex_count)
    : _n(side_vertex_count)
    , _scale(scale)
    , _mesh()
    , _vertices()
    , _edges()
    , _model_matrix(1.0f)
    , _a_gravity({0,-9.8,0})
    , _f_wind({0,0,0})
    , _air_resist(100.0f)
    , _time_step(DEFAULT_TIME_STEP)
    , _time_simulated(0)

    , _fpa(Curtain)
    , _grabbed_vertex(0)
    , _grabbed_dist(0)
    , _grabbing(false)

    , _snapshot(*this)
    , _frame_size(frame_size)
    , _last_mouse_pos()
    , _rend_tex()
    , _rend_sprite()
    , _text_tex()
    , _img_tex()
    , _img_color(0xFFFFFFFF)
    , _light_dir(2.0f, 2.0f, -10.0f)
    , _t_phys(0)
    , _cam_yaw(0)
    , _cam_pitch(0)
    , _rect((sf::Vector2f)_frame_size)
    , _outline_color(0)
    , _focused_color(0xffffffff)
    , _paused(false)
{
    sf::ContextSettings settings(24);
    _rend_tex.create(_frame_size.x, _frame_size.y, settings);
    _rend_sprite.setTexture(_rend_tex.getTexture(), true);
    _text_tex.create(_frame_size.x, _frame_size.y);
    
    // default image loading
    loadImgTexFromFile("resources/img/napkin.png");

    restart();
}

// assignment
// Cloth& operator=(const Cloth& o);
// Cloth& operator=(Cloth&& o);

// destructor
Cloth::~Cloth() {
    delete [] _mesh.vertices;
}



/* ============================================================================ *
 * Inherited from GuiComponent (Drawable)
 * ============================================================================ */
void Cloth::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if(getState(States::Hidden))
        return;

    states.transform *= getTransform();
    target.draw(_rect, states);
    target.draw(_rend_sprite, states);
}



/* ============================================================================ *
 * Inherited from GuiComponent (EventHandler)
 * ============================================================================ */
bool Cloth::addEventHandler(sf::RenderWindow& window, const sf::Event& event) {
    if(!getState(States::Focused)) {
        if(_grabbing) {
            _vertices[_grabbed_vertex].fixed = false;
            _grabbing = false;
        }
        return false;
    }

    if(event.type == sf::Event::MouseButtonPressed) { 
        _last_mouse_pos = getInverseTransform()
                .transformPoint({(float)event.mouseButton.x, (float)event.mouseButton.y});
        // grab action
        if(event.mouseButton.button == sf::Mouse::Left) {
            // calculate mouse position as ray into world
            glm::vec3 mouse_ray = render::mouse_to_world_ray(
                    _rend_tex.getSize().x, _rend_tex.getSize().y, 
                    _last_mouse_pos.x, _last_mouse_pos.y);
            glm::vec3 intersect_pos;
            glm::vec3 intersect_norm;
            int closest_vertex = -1;
            float closest_dist = std::numeric_limits<float>::max();
            glm::vec3 closest_world_pos;
            for(size_type i=0; i<_vertices.size(); i++) {
                if(_vertices[i].active) {
                    glm::vec3 world_pos = _model_matrix * glm::vec4(_vertices[i].pos, 1.0f);
                    if(glm::intersectRaySphere(
                        render::context.cam_pos, mouse_ray,
                        world_pos, 0.71f/_n /*1+sqrt(2)*/,
                        intersect_pos, intersect_norm)) 
                    {
                        float dist = glm::length(intersect_pos - render::context.cam_pos);
                        if(dist < closest_dist) {
                            closest_vertex = i;
                            closest_dist = dist;
                            closest_world_pos = world_pos;
                        }
                    }
                }
            }
            if(closest_vertex >= 0) {
                _grabbed_vertex = closest_vertex;
                _grabbed_dist = glm::length(closest_world_pos - render::context.cam_pos);
                _grabbing = true;
                _vertices[_grabbed_vertex].fixed = true;
            }
        }
    }

    else if(event.type == sf::Event::MouseButtonReleased) {
        if(event.mouseButton.button == sf::Mouse::Left && _grabbing) {
            _vertices[_grabbed_vertex].fixed = false;
            _grabbing = false;
        }
    }

    else if(event.type == sf::Event::KeyPressed) {
        if(event.key.code == sf::Keyboard::P) {
            togglePause();
        }
    }

    return false;
}

void Cloth::update(float t) {
    // set outline color
    if(getState(States::Focused))
        _rect.setOutlineColor(_focused_color);
    else
        _rect.setOutlineColor(_outline_color);

    // camera controls
    if(getState(States::Focused)) {
        const float speed = 1.0f;
        glm::vec3 cam_trans(0,0,0);
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            _cam_yaw += t;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            _cam_yaw -= t;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            _cam_pitch = std::min(_cam_pitch + t, 1.0f);
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            _cam_pitch = std::max(_cam_pitch - t, -1.0f);
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            cam_trans.z -= t*speed;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            cam_trans.z += t*speed;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            cam_trans.x -= t*speed;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            cam_trans.x += t*speed;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            cam_trans.y += t*speed;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
            cam_trans.y -= t*speed;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q) && _grabbing)
            _grabbing = false;
        // perform camera update
        cam_trans = glm::rotateY(cam_trans, _cam_yaw);
        render::context.cam_pos += cam_trans;
        render::context.cam_look = glm::rotateX(glm::vec3(0,0,-1.0), _cam_pitch);
        render::context.cam_look = glm::rotateY(render::context.cam_look, _cam_yaw);
        render::context.cam_look += render::context.cam_pos;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
            _cam_yaw = 0;
            _cam_pitch = 0;
            render::context.cam_pos = {0,0,1.0f};
            render::context.cam_look = {0,0,-1.0f};
            restart();
        }
    }

    // update mouse movement
    update_mouse_movement();

    // update physics
    _t_phys += t*(!_paused);
    while(_t_phys > _time_step) {
        update_physics();
        _t_phys -= _time_step;
    }

    // update and render mesh
    compute_normals();
    compute_mesh();
    render_mesh();
}


/* ============================================================================ *
 * Inherited from GuiComponent (Snapshottable)
 * ============================================================================ */
const Snapshot& Cloth::getSnapshot() const {
    return _snapshot;
}
void Cloth::applySnapshot(Snapshot& snapshot) 
{ }
void Cloth::revertSnapshot(Snapshot& snapshot)
{ }



/* ============================================================================ *
 * Inherited from GuiComponent (Snapshottable)
 * ============================================================================ */
sf::FloatRect Cloth::getClickableBounds() const {
    return getTransform().transformRect(sf::FloatRect({0,0}, (sf::Vector2f)_frame_size));
}
bool Cloth::hover(const sf::Vector2f& hover_pos) {
    _last_mouse_pos = getInverseTransform().transformPoint(hover_pos);
    return true;
}



/* ============================================================================ *
 * Accessors
 * ============================================================================ */
float Cloth::get_time_step() const {
    return _time_step;
}
const glm::mat4& Cloth::get_model_matrix() const {
    return _model_matrix;
}
float Cloth::get_scale() const {
    return _scale;
}
Cloth::size_type Cloth::get_n_vertices() const {
    return _n;
}
Cloth::FixedPointArrangement Cloth::get_fixed_point_arrangement() const {
    return _fpa;
}



/* ============================================================================ *
 * Mutators
 * ============================================================================ */
void Cloth::set_fixed_point(size_type v, bool fixed) {
    _vertices[v].fixed = fixed;
}
void Cloth::set_gravity(const glm::vec3& gravity) {
    _a_gravity = gravity;
}
void Cloth::set_wind_force(const glm::vec3& wind) {
    _f_wind = wind;
}
void Cloth::set_light_dir(const glm::vec3& light_dir) {
    _light_dir = light_dir;
}
void Cloth::set_scale(float scale) {
    if(_scale != scale) {
        _scale = scale;
        restart();
    }
}
void Cloth::set_n_vertices(size_type n) {
    if(_n != n) {
        _n = n;
        delete [] _mesh.vertices;
        _mesh.vertices = nullptr;
        restart();
    }
}
void Cloth::set_fixed_point_arrangement(FixedPointArrangement fpa) {
    if(_fpa != fpa) {
        _fpa = fpa;
        restart();
    }
}
void Cloth::set_image_color(const sf::Color& color) {
    _img_color = color;
}
void Cloth::set_image_texture(const sf::Texture& tex) {
    _img_tex = tex;
}
void Cloth::set_text_texture(const sf::Texture& tex) {
    _text_tex = tex;
}



/* ============================================================================ *
 * GUI Appearance
 * ============================================================================ */
void Cloth::setOutlineColor(const sf::Color& outline_color) {
    _outline_color = outline_color;
}
void Cloth::setFocusedOutlineColor(const sf::Color& focused_outline_color) {
    _focused_color = focused_outline_color;
}
void Cloth::setOutlineThickness(float outline_thickness) {
    _rect.setOutlineThickness(outline_thickness);
}



/* ============================================================================ *
 * GUI Appearance
 * ============================================================================ */
void Cloth::loadImgTexFromFile(const std::string& path) {
    _img_tex.loadFromFile(path);
    _img_tex.setSmooth(true);
}



/* ============================================================================ *
 * Graph Accessors
 * ============================================================================ */
Cloth::size_type Cloth::edge_connection(size_type e, size_type v) const {
    const cloth_edge& edge = _edges[e];
    return v ^ edge.vertex_a ^ edge.vertex_b;
}
Cloth::size_type Cloth::edge_connection(const cloth_edge& e, size_type v) const {
    return v ^ e.vertex_a ^ e.vertex_b;
}



/* ============================================================================ *
 * Update Functions
 * ============================================================================ */
void Cloth::update_mouse_movement() {
    if(!getState(States::Focused))
        return;

    if(_grabbing && _vertices[_grabbed_vertex].active) {
        // calculate mouse position as ray into world
        _vertices[_grabbed_vertex].pos_old = _vertices[_grabbed_vertex].pos;
        glm::vec3 mouse_ray = render::mouse_to_world_ray(
                _rend_tex.getSize().x, _rend_tex.getSize().y, 
                _last_mouse_pos.x, _last_mouse_pos.y);
        glm::vec4 grabbed_world_pos = glm::vec4(render::context.cam_pos + _grabbed_dist*mouse_ray, 1.0f);
        _vertices[_grabbed_vertex].pos = glm::affineInverse(_model_matrix)*grabbed_world_pos;
    }
    else if(sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
        // calculate mouse position as ray into world
        glm::vec3 mouse_ray = render::mouse_to_world_ray(
                _rend_tex.getSize().x, _rend_tex.getSize().y, 
                _last_mouse_pos.x, _last_mouse_pos.y);
        glm::vec3 intersect_pos;
        glm::vec3 intersect_norm;
        int closest_vertex = -1;
        float closest_dist = std::numeric_limits<float>::max();
        glm::vec3 closest_world_pos;
        for(size_type i=0; i<_vertices.size(); i++) {
            if(_vertices[i].active) {
                glm::vec3 world_pos = _model_matrix * glm::vec4(_vertices[i].pos, 1.0f);
                if(glm::intersectRaySphere(
                    render::context.cam_pos, mouse_ray,
                    world_pos, 0.71f/_n /*1+sqrt(2)*/,
                    intersect_pos, intersect_norm)) 
                {
                    float dist = glm::length(intersect_pos - render::context.cam_pos);
                    if(dist < closest_dist) {
                        closest_vertex = i;
                        closest_dist = dist;
                        closest_world_pos = world_pos;
                    }
                }
            }
        }
        if(closest_vertex >= 0) {
            _erase_vertex(closest_vertex);
        }
    }
}

void Cloth::update_physics() {
    float ts_sqr = _time_step*_time_step;
    for(auto it=_vertices.begin(); it!=_vertices.end(); ++it) {
        if(it->active && !it->fixed) {
            // // calculate vertex's mass by summing cardinal neighbors
            // unsigned num_cardinal = it->edge_up + it->edge_down + it->edge_left + it->edge_right;
            // if(num_cardinal == 4)
            //     it->mass = 4.0f;
            // determine vertice's new position
            // - uses verlet integration
            // - equation source: https://graphics.stanford.edu/~mdfisher/cloth.html
            // glm::vec3 vel = it->pos - it->pos_old;
            glm::vec3 pos_new = it->pos;
            pos_new *= 2.0;
            pos_new -= it->pos_old;
            // gravity
            glm::vec3 a = it->mass*_a_gravity;
            // wind
            a += _f_wind;
            // air resist
            // a += -_air_resist*vel*vel;
            a /= it->mass;
            a *= ts_sqr;
            pos_new += a;
            // apply new position
            it->pos_old = it->pos;
            it->pos = pos_new;
            // resolve plane collision
            _resolve_plane_intersection(*it);
        }
    }
    for(int i=0; i<PHYSICS_ITERATIONS; i++)
        _resolve_physics_constraints();
    // resolve any plane collisions that may have occurred during physics contraints 
    for(auto it=_vertices.begin(); it!=_vertices.end(); ++it) {
        if(it->active && !it->fixed)
            _resolve_plane_intersection(*it);
    }
}

void Cloth::restart() {
    _model_matrix = glm::mat4(1.0f);
    float inv_scale = 1.0f/_scale;
    _model_matrix = glm::scale(_model_matrix, {inv_scale, inv_scale, inv_scale});
    float offset_x = -_scale/2.0f;
    float offset_y = 1.1f*_scale/2.0f;
    _model_matrix = glm::translate(_model_matrix, {offset_x, offset_y, 0});
    _vertices.clear();
    _edges.clear();
    _initialize_cloth_vertices();
}


/* ============================================================================ *
 * Mesh Retrieval
 * ============================================================================ */
const render::mesh& Cloth::get_mesh() const {
    return _mesh;
}

void Cloth::compute_normals() {
    for(auto it=_vertices.begin(); it!=_vertices.end(); ++it)
        it->norm = {0,0,0};
    for(size_type r=1; r<_n; r++) {
        for(size_type c=0; c<_n-1; c++) {
            cloth_vertex& bl = _vertices[r*_n + c];
            cloth_vertex& br = _vertices[r*_n + c + 1];
            cloth_vertex& al = _vertices[(r-1)*_n + c];
            cloth_vertex& ar = _vertices[(r-1)*_n + c + 1];
            if(bl.active && ar.active) {
                if(al.active) {
                    glm::vec3 edge_1 = bl.pos-al.pos;
                    glm::vec3 edge_2 = ar.pos-al.pos;
                    glm::vec3 edge_3 = bl.pos-ar.pos;
                    glm::vec3 p = glm::cross(edge_1, edge_2);
                    edge_1 = glm::normalize(edge_1);
                    edge_2 = glm::normalize(edge_2);
                    edge_3 = glm::normalize(edge_3);
                    bl.norm += p*glm::angle(edge_1, edge_3);
                    al.norm += p*glm::angle(edge_1, edge_2);
                    ar.norm += p*glm::angle(-edge_2, edge_3);
                }
                if(br.active) {
                    glm::vec3 edge_1 = ar.pos-br.pos;
                    glm::vec3 edge_2 = bl.pos-br.pos;
                    glm::vec3 edge_3 = bl.pos-ar.pos;
                    glm::vec3 p = glm::cross(edge_1, edge_2);
                    edge_1 = glm::normalize(edge_1);
                    edge_2 = glm::normalize(edge_2);
                    edge_3 = glm::normalize(edge_3);
                    bl.norm += p*glm::angle(edge_2, edge_3);
                    br.norm += p*glm::angle(edge_1, edge_2);
                    ar.norm += p*glm::angle(-edge_1, edge_3);
                }
            }
        }
    }
}

void Cloth::compute_mesh() {
    if(_mesh.vertices == nullptr) 
        _mesh.vertices = new render::vertex[(_n-1)*(_n-1)*6];
    _mesh.size = 0;
    for(auto it=_vertices.begin(); it!=_vertices.end(); ++it) {
        if(it->active) {
            if(it->edge_right >= 0 && it->edge_down >= 0) {
                int tr = edge_connection(it->edge_right, it->index);
                int bl = edge_connection(it->edge_down, it->index);
                _mesh.vertices[_mesh.size++] = {_vertices[tr].pos, _vertices[tr].norm, _vertices[tr].uv};
                _mesh.vertices[_mesh.size++] = {it->pos,           it->norm,           it->uv};
                _mesh.vertices[_mesh.size++] = {_vertices[bl].pos, _vertices[bl].norm, _vertices[bl].uv};
            }
            if(it->edge_left >= 0 && it->edge_up >= 0) {
                int tr = edge_connection(it->edge_up, it->index);
                int bl = edge_connection(it->edge_left, it->index);
                _mesh.vertices[_mesh.size++] = {_vertices[tr].pos, _vertices[tr].norm, _vertices[tr].uv};
                _mesh.vertices[_mesh.size++] = {_vertices[bl].pos, _vertices[bl].norm, _vertices[bl].uv};
                _mesh.vertices[_mesh.size++] = {it->pos,           it->norm,           it->uv};
            }
        }
    }
}

// void Cloth::compute_mesh() {
//     if(_mesh.vertices == nullptr) 
//         _mesh.vertices = new render::vertex[_vertices.size()*6];
//     _mesh.size = 0;
//     for(size_type r=0; r<_n-1; r++) {
//         for(size_type c=0; c<_n-1; c++) {
//             cloth_vertex& tl = _vertices[r*_n + c];
//             // full quad
//             if(tl.active && tl.edge_right >= 0 && tl.edge_down >= 0 && tl.edge_diag_br >= 0) {
//                 cloth_vertex& tr = _vertices[edge_connection(tl.edge_right, tl.index)];
//                 cloth_vertex& bl = _vertices[edge_connection(tl.edge_down, tl.index)];
//                 cloth_vertex& br = _vertices[edge_connection(tl.edge_diag_br, tl.index)];
//                 // determine triangle orientation to prevent buckling 
//                 if(glm::distance2(tl.pos, br.pos) < glm::distance2(tr.pos, bl.pos)) {
//                     _mesh.vertices[_mesh.size++] = {tl.pos, tl.norm, tl.uv};
//                     _mesh.vertices[_mesh.size++] = {bl.pos, bl.norm, bl.uv};
//                     _mesh.vertices[_mesh.size++] = {br.pos, br.norm, br.uv};
//                     _mesh.vertices[_mesh.size++] = {br.pos, br.norm, br.uv};
//                     _mesh.vertices[_mesh.size++] = {tr.pos, tr.norm, tr.uv};
//                     _mesh.vertices[_mesh.size++] = {tl.pos, tl.norm, tl.uv};
//                 }
//                 else {
//                     _mesh.vertices[_mesh.size++] = {tr.pos, tr.norm, tr.uv};
//                     _mesh.vertices[_mesh.size++] = {tl.pos, tl.norm, tl.uv};
//                     _mesh.vertices[_mesh.size++] = {bl.pos, bl.norm, bl.uv};
//                     _mesh.vertices[_mesh.size++] = {bl.pos, bl.norm, bl.uv};
//                     _mesh.vertices[_mesh.size++] = {br.pos, br.norm, br.uv};
//                     _mesh.vertices[_mesh.size++] = {tr.pos, tr.norm, tr.uv};
//                 }
//             }
//         }
//     }
// }

void Cloth::render_mesh() {
    render::render(_rend_tex, _mesh, _model_matrix, _light_dir, _img_tex, _text_tex, _img_color);
    _rend_tex.display(); 
}



/* ============================================================================ *
 * Renderer Management
 * ============================================================================ */
void Cloth::init_renderer() {
    render::init(_rend_tex);
}



/* ============================================================================ *
 * Runtime
 * ============================================================================ */
void Cloth::pause() {
    _paused = true;
}
void Cloth::resume() {
    _paused = false;
}
bool Cloth::togglePause() {
    _paused = !_paused;
    return _paused;
}
bool Cloth::isPaused() {
    return _paused;
}



/* ============================================================================ *
 * Private Functions - Initialization
 * ============================================================================ */
void Cloth::_initialize_cloth_vertices() {
    cloth_vertex v;
    for(int r=0; r<_n; r++) {
        for(int c=0; c<_n; c++) {
            size_type index = _vertices.size();
            // setup basic vertex data
            v.pos_old = {_scale*c/_n, _scale*(-r)/_n, 0.001f*_scale*r/_n};
            v.pos = v.pos_old;
            v.edge_indices.clear();
            v.edge_indices.clear();
            v.uv = glm::vec2((float)c/(_n-1), (float)r/(_n-1));
            v.edge_up = -1;
            v.edge_right = -1;
            v.edge_down = -1;
            v.edge_left = -1;
            v.edge_diag_al = -1;
            v.edge_diag_ar = -1;
            v.edge_diag_bl = -1;
            v.edge_diag_br = -1;
            v.edge_2up = -1;
            v.edge_2right = -1;
            v.edge_2down = -1;
            v.edge_2left = -1;
            v.index = index;
            v.mass = _scale;
            v.fixed = false;
            v.active = true;
            // push back vertex
            _vertices.push_back(std::move(v));
            _add_edges_init_vertex(v.index, r, c);
            // // add neighbor edges (only for vertices behind this one)
            // _add_edge_relative(index, r, c, -1,  0, NEIGHBOR_SPRING_LENGTH_FLEX_COEFF);    // add above
            // _add_edge_relative(index, r, c,  0, -1, NEIGHBOR_SPRING_LENGTH_FLEX_COEFF);    // add left
            // _add_edge_relative(index, r, c, -1, -1, NEIGHBOR_SPRING_LENGTH_FLEX_COEFF);    // add above-left
            // _add_edge_relative(index, r, c, -1,  1, NEIGHBOR_SPRING_LENGTH_FLEX_COEFF);    // add above-right
            // // add bending edges (only for vertices behind this one)
            // _add_edge_relative(index, r, c, -2,  0, BENDING_SPRING_LENGTH_FLEX_COEFF);    // add above
            // _add_edge_relative(index, r, c,  0, -2, BENDING_SPRING_LENGTH_FLEX_COEFF);    // add left
        }
    }
    // add default fixed points
    _init_fixed_points();
}

void Cloth::_init_fixed_points() {
    switch(_fpa) {
        case Curtain:
            _vertices[0].fixed = true;
            _vertices[_n-1].fixed = true;
            if(_n >= 16) {
                _vertices[_n+1].fixed = true;
                _vertices[2*_n-2].fixed = true;
            }
            break;
        case FlagLeft:
            _vertices[0].fixed = true;
            _vertices[(_n-1)*_n].fixed = true;
            break;
        case FlatRight:
            _vertices[_n-1].fixed = true;
            _vertices[_n*_n-1].fixed = true;
            break;
        case FourCorners:
            _vertices[0].fixed = true;
            _vertices[_n-1].fixed = true;
            _vertices[(_n-1)*_n].fixed = true;
            _vertices[_n*_n-1].fixed = true;
            break;
        case AllTop:
            for(size_type c=0; c<_n; c++)
                _vertices[c].fixed = true;
            break;
        case AllLeft:
            for(size_type r=0; r<_n; r++)
                _vertices[r*_n].fixed = true;
            break;
        case AllRight:
            for(size_type r=0; r<_n; r++)
                _vertices[r*_n+(_n-1)].fixed = true;
            break;
        case AllLeftRight:
            for(size_type r=0; r<_n; r++) {
                _vertices[r*_n].fixed = true;
                _vertices[r*_n+(_n-1)].fixed = true;
            }
            break;
        case AllPerimeter:
            for(size_type c=0; c<_n; c++) {
                _vertices[c].fixed = true;
                _vertices[(_n-1)*_n+c].fixed = true;
            }
            for(size_type r=1; r<_n-1; r++) {
                _vertices[r*_n].fixed = true;
                _vertices[r*_n+(_n-1)].fixed = true;
            }
            break;
        // case Loose:
        default: break;
    }
}

void Cloth::_add_edges_init_vertex(size_type v, int r, int c) {
    cloth_vertex& vert = _vertices[v];

    float neighbor_length = _scale/_n;
    float diag_length = sqrt(2*neighbor_length*neighbor_length);
    float bending_length = 2*neighbor_length;

    // add above
    int r2 = r-1;
    int c2 = c;
    if(r2 >= 0) {
        int v2 = r2*_n + c2;
        float length = neighbor_length;
        size_type e = _add_edge(v, v2, length, NEIGHBOR_FLEX_COEFF);
        vert.edge_up = e;
        _vertices[v2].edge_down = e;
    }
    // add left
    r2 = r;
    c2 = c-1;
    if(c2 >= 0) {
        int v2 = r2*_n + c2;
        float length = neighbor_length;
        size_type e = _add_edge(v, v2, length, NEIGHBOR_FLEX_COEFF);
        vert.edge_left = e;
        _vertices[v2].edge_right = e;
    }
    // add above-left
    r2 = r-1;
    c2 = c-1;
    if(r2 >= 0 && c2 >= 0) {
        int v2 = r2*_n + c2;
        float length = diag_length; // sqrt(2)
        size_type e = _add_edge(v, v2, length, NEIGHBOR_FLEX_COEFF);
        vert.edge_diag_al = e;
        _vertices[v2].edge_diag_br = e;
    }
    // add above-right
    r2 = r-1;
    c2 = c+1;
    if(r2 >= 0 && c2 < _n) {
        int v2 = r2*_n + c2;
        float length = diag_length; // sqrt(2)
        size_type e = _add_edge(v, v2, length, NEIGHBOR_FLEX_COEFF);
        vert.edge_diag_ar = e;
        _vertices[v2].edge_diag_bl = e;
    }
    // add bending above
    r2 = r-2;
    c2 = c;
    if(r2 >= 0) {
        int v2 = r2*_n + c2;
        float length = bending_length;
        size_type e = _add_edge(v, v2, length, BENDING_FLEX_COEFF);
        vert.edge_2up = e;
        _vertices[v2].edge_2down = e;
    }
    // add bending left
    r2 = r;
    c2 = c-2;
    if(c2 >= 0) {
        int v2 = r2*_n + c2;
        float length = bending_length;
        size_type e = _add_edge(v, v2, length, BENDING_FLEX_COEFF);
        vert.edge_2left = e;
        _vertices[v2].edge_2right = e;
    }
}

Cloth::size_type Cloth::_add_edge(size_type a, size_type b, float length, float flex_coeff) {
    if(a > b)
        std::swap(a, b);
    cloth_edge e;
    size_type index = _edges.size();
    e.vertex_a = a;
    e.vertex_b = b;
    e.index = index;
    e.flex_coeff = flex_coeff;
    e.resting_length = length;
    e.active = true;
    _edges.push_back(std::move(e));
    _vertices[a].edge_indices.insert(index);
    _vertices[b].edge_indices.insert(index);
    return index;
}



/* ============================================================================ *
 * Private Functions - Constraint Resolving
 * ============================================================================ */
Cloth::size_type Cloth::_resolve_physics_constraints() {
    size_type count_resolved = 0;
    for(auto it=_edges.begin(); it!=_edges.end(); ++it) {
        if(it->active) {
            // get vertices from edge
            cloth_vertex& a = _vertices[it->vertex_a];
            cloth_vertex& b = _vertices[it->vertex_b];
            // calculate difference length
            glm::vec3 v_diff = b.pos - a.pos;
            // float v_diff_length = glm::length(v_diff);
            float v_diff_length_2 = glm::length2(v_diff);
            // find flexed resting length to resolve with
            float adjusted_resting_length = it->resting_length;
            float adjusted_lower = adjusted_resting_length*(1.0-it->flex_coeff);
            float adjusted_upper = adjusted_resting_length*(1.0+it->flex_coeff);
            if(v_diff_length_2 < adjusted_lower*adjusted_lower)
                adjusted_resting_length = adjusted_lower;
            else if(v_diff_length_2 > adjusted_upper*adjusted_upper)
                adjusted_resting_length = adjusted_upper;
            else 
                adjusted_resting_length = 0;
            // if current edge length outside of range, resolve
            if(adjusted_resting_length > 0) {
                float v_diff_length = sqrt(v_diff_length_2);
                glm::vec3 delt_a = v_diff;
                glm::vec3 delt_b = {0,0,0};
                delt_a *= v_diff_length - it->resting_length;
                delt_a /= v_diff_length;
                if(!a.fixed && !b.fixed) {
                    delt_a *= 0.5;
                    delt_b = delt_a;
                }
                else if(a.fixed && b.fixed) {
                    delt_a = {0,0,0};
                }
                else if(a.fixed) {
                    delt_b = delt_a;
                    delt_a = {0,0,0};
                }
                a.pos += delt_a;
                b.pos -= delt_b;
                count_resolved++;
            }
        }
    }
    return count_resolved;
}

bool Cloth::_resolve_plane_intersection(cloth_vertex& v) {
    float floor_y = _scale*FLOOR_PLANE_Y;
    if(v.pos.y >= floor_y)
        return false;

    glm::vec3 frame_path = v.pos - v.pos_old;
    float ratio = std::abs((floor_y - v.pos.y) / frame_path.y);
    frame_path *= ratio;
    v.pos -= frame_path;
    frame_path.y = 0;
    frame_path *= FLOOR_PLANE_FRICTION_COEFF;
    v.pos += frame_path;
    v.pos.y = floor_y + 0.0001f;
    return true;
}



/* ============================================================================ *
 * Private Functions - Mesh Manipulation
 * ============================================================================ */
bool Cloth::_erase_edge(size_type e) {
    return _erase_edge(_edges[e]);
}

bool Cloth::_erase_edge(cloth_edge& edge) {
    size_type e = edge.index;
    edge.active = false;
    cloth_vertex& a = _vertices[edge.vertex_a];
    cloth_vertex& b = _vertices[edge.vertex_b];
    a.edge_indices.erase(e);
    if(a.edge_up == e) {
        a.edge_up = -1;
        if(a.edge_2up >= 0) _erase_edge(a.edge_2up);
    }
    if(a.edge_right == e) {
        a.edge_right = -1;
        if(a.edge_2right >= 0) _erase_edge(a.edge_2right);
    }
    if(a.edge_down == e) {
        a.edge_down = -1;
        if(a.edge_2down >= 0) _erase_edge(a.edge_2down);
    }
    if(a.edge_left == e) {
        a.edge_left = -1;
        if(a.edge_2left >= 0) _erase_edge(a.edge_2left);
    }
    if(a.edge_diag_al == e)  a.edge_diag_al = -1;
    if(a.edge_diag_ar == e)  a.edge_diag_ar = -1;
    if(a.edge_diag_bl == e)  a.edge_diag_bl = -1;
    if(a.edge_diag_br == e)  a.edge_diag_br = -1;
    if(a.edge_up < 0) {
        if(a.edge_left < 0 && a.edge_diag_al >= 0)
            _erase_edge(a.edge_diag_al);
        if(a.edge_right < 0 && a.edge_diag_ar >= 0)
            _erase_edge(a.edge_diag_ar);
    }
    if(a.edge_down < 0) {
        if(a.edge_left < 0 && a.edge_diag_bl >= 0)
            _erase_edge(a.edge_diag_bl);
        if(a.edge_right < 0 && a.edge_diag_br >= 0)
            _erase_edge(a.edge_diag_br);
    }
    b.edge_indices.erase(e);
    if(b.edge_up == e) {
        b.edge_up = -1;
        if(b.edge_2up >= 0) _erase_edge(b.edge_2up);
    }
    if(b.edge_right == e) {
        b.edge_right = -1;
        if(b.edge_2right >= 0) _erase_edge(b.edge_2right);
    }
    if(b.edge_down == e) {
        b.edge_down = -1;
        if(b.edge_2down >= 0) _erase_edge(b.edge_2down);
    }
    if(b.edge_left == e) {
        b.edge_left = -1;
        if(b.edge_2left >= 0) _erase_edge(b.edge_2left);
    }
    if(b.edge_diag_al == e)  b.edge_diag_al = -1;
    if(b.edge_diag_ar == e)  b.edge_diag_ar = -1;
    if(b.edge_diag_bl == e)  b.edge_diag_bl = -1;
    if(b.edge_diag_br == e)  b.edge_diag_br = -1;
    if(b.edge_up < 0) {
        if(b.edge_left < 0 && b.edge_diag_al >= 0)
            _erase_edge(b.edge_diag_al);
        if(b.edge_right < 0 && b.edge_diag_ar >= 0)
            _erase_edge(b.edge_diag_ar);
    }
    if(b.edge_down < 0) {
        if(b.edge_left < 0 && b.edge_diag_bl >= 0)
            _erase_edge(b.edge_diag_bl);
        if(b.edge_right < 0 && b.edge_diag_br >= 0)
            _erase_edge(b.edge_diag_br);
    }
    // check if either vertex should be erased
    if((a.edge_up < 0 && a.edge_down < 0) || (a.edge_left < 0 && a.edge_right < 0)) {
        _erase_vertex(a);
    }
    if((b.edge_up < 0 && b.edge_down < 0) || (b.edge_left < 0 && b.edge_right < 0)) {
        _erase_vertex(b);
    }
    return true;
}

bool Cloth::_erase_vertex(size_type v) {
    return _erase_vertex(_vertices[v]);
}

bool Cloth::_erase_vertex(cloth_vertex& vert) {
    vert.active = false;
    while(!vert.edge_indices.empty()) {
        _erase_edge(*(vert.edge_indices.begin()));
    }
    return true;
}

// bool Cloth::_split_vertex(size_type v, bool horizontal) {
//     return _split_vertex(_vertices[v], horizontal);
// }

// bool Cloth::_split_vertex(cloth_vertex& v, bool horizontal) {
//     return horizontal ? _split_vertex_horizontal(v) : _split_vertex_vertical(v);
// }

// bool Cloth::_split_vertex_horizontal(cloth_vertex& v) {
//     if(v.edge_up < 0 || v.edge_down < 0)
//         return false;
//     _vertices.push_back(v);
//     cloth_vertex& new_v = _vertices.back();
//     v.edge_down = -1;
//     new_v.index = _vertices.size()-1;
//     new_v.edge_indices.erase(new_v.edge_up);
//     new_v.edge_up = -1;
//     cloth_edge& e_down = _edges[new_v.edge_down];
//     if(e_down.vertex_a == v.index)
//         e_down.vertex_a = new_v.index;
//     else
//         e_down.vertex_b = new_v.index;
//     return true;
// }

// bool Cloth::_split_vertex_vertical(cloth_vertex& v) {
//     return false;
// }
