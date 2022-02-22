/* **************************************************************************** *
 * AUTHOR:      Noah Krim
 * ASSIGNMENT:  GUI Cloth Sim
 * CLASS:       CS_08
 * ---------------------------------------------------------------------------- *
 * File: mouse-tracker.h
 *  Header file for MouseTracker class
 * **************************************************************************** */

/* ---------------------------------------------------------------------------- *
 * NOTE: MouseTracker's update() function should be called AFTER all 
 *          other EventHandler's that are dependent on MouseTracker.
 * ---------------------------------------------------------------------------- */

#ifndef MOUSE_TRACKER_H
#define MOUSE_TRACKER_H

#include <SFML/Graphics.hpp>
#include <vector>

#include "../gui/component-interface/gui-component.h"

class MouseTracker : public EventHandler {
private:
    // private member variables
    sf::RenderWindow* _window;
    sf::Cursor _default_cursor;
    std::vector<GuiComponent*> _clickable_components;
    GuiComponent* _top_hovering;
    GuiComponent* _last_clicked;
    sf::Vector2f _last_click_position;
    sf::Vector2f _mouse_position;
    sf::Mouse::Button _last_click_button;
    unsigned _consecutive_clicks;
    float _time_since_last_click;
    bool _clicked_this_frame;
    bool _mouse_held;

public:
    // global constants
    static constexpr float CONSECUTIVE_CLICK_INTERVAL = 0.5f;
    // static const unsigned CONSECUTIVE_CLICK_POS_DELTA = 5;

    // static instance of class
    static MouseTracker global_tracker;

    // constructors
    MouseTracker();

    // inherited from EventHandler
    virtual bool addEventHandler(sf::RenderWindow& window, const sf::Event& event);
    virtual void update(float t=0);

    // mutators
    void addClickableComponent(GuiComponent& component);
    void setClickableComponents(const std::vector<GuiComponent*>& clickable_components);
    void setFocusedComponent(GuiComponent& component);
    void setRenderWindow(sf::RenderWindow& window);
    void setWindowMouseCursor(sf::Cursor* cursor);

    // accessors
    const std::vector<GuiComponent*>& getClickableComponents() const;
    const sf::RenderWindow* getRenderWindow() const;

    // global mouse event queries
    const GuiComponent* getLastClicked() const;
    const sf::Vector2f& getLastClickPosition() const;
    const sf::Vector2f& getMousePosition() const;
    sf::Mouse::Button getLastClickButton() const;
    unsigned getConsecutiveClicks() const;
    bool getClickedThisFrame() const;
    bool getMouseHeld() const;

    // clickable-specific mouse event queries
    bool isLastClicked(const GuiComponent& component) const;
    bool isClickedThisFrame(const GuiComponent& component) const;
    bool isMouseHovering(const GuiComponent& component) const;
    sf::Vector2f calcLocalMousePosition(const GuiComponent& component) const;
};

#endif