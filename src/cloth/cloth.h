/* **************************************************************************** *
 * AUTHOR:      Noah Krim
 * ASSIGNMENT:  GUI Cloth Sim
 * CLASS:       CS_08
 * ---------------------------------------------------------------------------- *
 * File: cloth.h
 *  Header file for Cloth class
 * **************************************************************************** */

#ifndef CLOTH_H
#define CLOTH_H

#include <SFML/Graphics.hpp>

#include "../../libs/glm/vec3.hpp"
#include "../../libs/glm/mat4x4.hpp"

#include "render_mesh.h"
#include "../gui/component-interface/gui-component.h"
#include "../snapshots/int-snapshot.h"

#include <vector>
#include <set>
#include <string>

class Cloth : public GuiComponent {
public:
    // typdefs
    typedef std::size_t size_type;

public:
    // static constants
    static const size_type DEFAULT_SIDE_VERTEX_COUNT;
    static const size_type PHYSICS_ITERATIONS;
    static const float DEFAULT_TIME_STEP;
    static const float NEIGHBOR_FLEX_COEFF;
    static const float BENDING_FLEX_COEFF;
    static const float NEIGHBOR_TEAR_THRESH;
    static const float FLOOR_PLANE_Y;
    static const float FLOOR_PLANE_FRICTION_COEFF;

    // enums
    enum FixedPointArrangement {
        Curtain=0,
        FlagLeft,
        FlatRight,
        FourCorners,
        AllTop,
        AllLeft,
        AllRight,
        AllLeftRight,
        AllPerimeter,
        Loose   
    };

private:
    // structs
    struct cloth_vertex {
        glm::vec3 pos_old;
        glm::vec3 pos;
        glm::vec3 norm;
        glm::vec2 uv;
        std::set<size_type> edge_indices;
        int edge_up;
        int edge_right;
        int edge_down;
        int edge_left;
        int edge_diag_al;
        int edge_diag_ar;
        int edge_diag_bl;
        int edge_diag_br;
        int edge_2up;
        int edge_2right;
        int edge_2down;
        int edge_2left;
        size_type index;
        float mass;
        bool fixed;
        bool active;
    };
    struct cloth_edge {
        size_type vertex_a;
        size_type vertex_b;
        size_type index;
        float flex_coeff;
        float resting_length;
        bool active;
    };

    // private data members
    size_type _n; // side vertex count
    float _scale;
    render::mesh _mesh;
    std::vector<cloth_vertex> _vertices;
    std::vector<cloth_edge> _edges;
    glm::mat4 _model_matrix;
    glm::vec3 _a_gravity;
    glm::vec3 _f_wind;
    float _air_resist;
    float _time_step;
    float _time_simulated;

    FixedPointArrangement _fpa;
    size_type _grabbed_vertex;
    float _grabbed_dist;
    bool _grabbing;

    IntSnapshot _snapshot;
    sf::Vector2u _frame_size;
    sf::Vector2f _last_mouse_pos;
    sf::RenderTexture _rend_tex;
    sf::Sprite _rend_sprite;
    sf::Texture _text_tex;
    sf::Texture _img_tex;
    sf::Color _img_color;
    glm::vec3 _light_dir;
    float _t_phys;
    float _cam_yaw;
    float _cam_pitch;
    sf::RectangleShape _rect;
    sf::Color _outline_color;
    sf::Color _focused_color;
    bool _paused;

protected:
    // inherited from GuiComponent (sf::Drawable)
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

public:
    // constructors
    Cloth(sf::Vector2u _frame_size={1400,1000}, float scale=1.0f, size_type side_vertex_count=DEFAULT_SIDE_VERTEX_COUNT);
    Cloth(const Cloth& o);
    Cloth(Cloth&& o);

    // assignment
    Cloth& operator=(const Cloth& o);
    Cloth& operator=(Cloth&& o);

    // destructor
    virtual ~Cloth();

    // inherited from GuiComponent (EventHandler)
    bool addEventHandler(sf::RenderWindow& window, const sf::Event& event);
    void update(float t=0);

    // inherited from GuiComponent (Snapshotable)
    virtual const Snapshot& getSnapshot() const;
    virtual void applySnapshot(Snapshot& snapshot);
    virtual void revertSnapshot(Snapshot& snapshot);

    // inherited from GuiComponent (Clickable)
    virtual sf::FloatRect getClickableBounds() const;
    virtual bool hover(const sf::Vector2f& hover_pos);

    // accessors
    float get_time_step() const;
    const glm::mat4& get_model_matrix() const;
    float get_scale() const;
    size_type get_n_vertices() const;
    FixedPointArrangement get_fixed_point_arrangement() const;

    // mutators
    void set_fixed_point(size_type v, bool fixed);
    void set_gravity(const glm::vec3& gravity);
    void set_wind_force(const glm::vec3& wind);
    void set_light_dir(const glm::vec3& light_dir);
    void set_scale(float scale);
    void set_n_vertices(size_type n);
    void set_fixed_point_arrangement(FixedPointArrangement fpa);
    void set_image_color(const sf::Color& color);
    void set_image_texture(const sf::Texture& tex);
    void set_text_texture(const sf::Texture& tex);

    // gui appearance
    void setOutlineColor(const sf::Color& outline_color);
    void setFocusedOutlineColor(const sf::Color& focused_outline_color);
    void setOutlineThickness(float outline_thickness);

    // texture mutators
    void set_img_tex(const std::string& path);

    // graph accessors
    size_type edge_connection(size_type e, size_type v) const;
    size_type edge_connection(const cloth_edge& e, size_type v) const;

    // update functions
    void update_mouse_movement();
    void update_physics();
    void restart();

    // mesh retrieval
    void compute_normals();
    void compute_mesh();
    const render::mesh& get_mesh() const;
    void render_mesh();

    // renderer management
    void init_renderer();

    // runtime
    void pause();
    void resume();
    bool togglePause();
    bool isPaused();

private:
    // private functions
    // - initialization
    void _initialize_cloth_vertices();
    void _init_fixed_points();
    void _add_edges_init_vertex(size_type v, int r, int c);
    size_type _add_edge(size_type a, size_type b, float length, float flex_coeff);
    // - constraint resolving
    size_type _resolve_physics_constraints();
    bool _resolve_plane_intersection(cloth_vertex& v);
    // - mesh manipulation
    bool _erase_edge(size_type e);
    bool _erase_edge(cloth_edge& edge);
    bool _erase_vertex(size_type v);
    bool _erase_vertex(cloth_vertex& vert);
    // bool _split_vertex(size_type v, bool horizontal);
    // bool _split_vertex(cloth_vertex& v, bool horizontal);
    // bool _split_vertex_horizontal(cloth_vertex& v);
    // bool _split_vertex_vertical(cloth_vertex& v);
};

#endif