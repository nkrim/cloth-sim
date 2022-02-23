 /* **************************************************************************** *
 * AUTHOR:      Noah Krim
 * ASSIGNMENT:  SFML Dropdown
 * CLASS:       CS_08
 * ---------------------------------------------------------------------------- *
 * File: mouse-tracker.cpp
 *  Definition file for MouseTracker class
 * **************************************************************************** */

#include "mouse-tracker.h"
#include <iostream>

// constructors
MouseTracker::MouseTracker()
    : _window{nullptr}
    , _default_cursor{}
    , _clickable_components{}
    , _top_hovering{nullptr}
    , _last_clicked{nullptr}
    , _last_click_position{0,0}
    , _mouse_position{0,0}
    , _last_click_button{sf::Mouse::Left}
    , _consecutive_clicks{0}
    , _time_since_last_click{0}
    , _clicked_this_frame{false}
    , _mouse_held{false}
{ 
    // load default cursor
    _default_cursor.loadFromSystem(sf::Cursor::Arrow);
}

// inherited from EventHandler
bool MouseTracker::addEventHandler(sf::RenderWindow& window, const sf::Event& event) {
    // flag for snapshot return
    bool snapshot_available = false;

    // mouse move
    if(event.type == sf::Event::MouseMoved) {
        _mouse_position = {(float)event.mouseMove.x, (float)event.mouseMove.y};

        // reset _top_hovering (gets cursor from this and shortcuts click test)
        _top_hovering = nullptr;
        // test focused (_last_clicked) component first
        if(_last_clicked != nullptr) { 
            if(!_last_clicked->anyStates(States::Disabled|States::Hidden) 
                && _last_clicked->containsMousePosition(_mouse_position))
            {
                _last_clicked->setState(States::Hovered, true);
                if(_last_clicked->hover(_mouse_position))
                    _top_hovering = _last_clicked;
            }
            else {
                _last_clicked->setState(States::Hovered, false);
                _last_clicked->unhover();
            }
        }
        
        // test for hover
        for(auto it=_clickable_components.begin(); it!=_clickable_components.end(); ++it) {
            // skip _last_clicked
            if((*it) == _last_clicked) { 
                /* do nothing */ 
            }
            // test for valid hover
            else if(_top_hovering == nullptr
                && !(*it)->anyStates(States::Disabled|States::Hidden) 
                && (*it)->containsMousePosition(_mouse_position))
            {
                (*it)->setState(States::Hovered, true);
                // call hover() and set cursor if first success 
                if((*it)->hover(_mouse_position) && _top_hovering == nullptr) {
                    _top_hovering = *it;
                }
            }
            // unhover
            else {
                if((*it)->getState(States::Hovered)) {
                    (*it)->setState(States::Hovered, false);
                    (*it)->unhover();
                }
            }
        }

        // if none hovered, set to default
        if(_top_hovering == nullptr)
            window.setMouseCursor(_default_cursor);
    }

    // mouse button pressed
    else if(event.type == sf::Event::MouseButtonPressed) {
        // set mouse position from event
        _mouse_position = {(float)event.mouseButton.x, (float)event.mouseButton.y};
        // set last_click_position
        _last_click_position = _mouse_position;
        
        // current clicked component
        GuiComponent* curr_clicked = nullptr;
        // test focused component (_last_clicked) first
        if(_last_clicked != nullptr 
            && !_last_clicked->anyStates(States::Disabled|States::Hidden|States::Unclickable) 
            && _last_clicked->containsMousePosition(_mouse_position)) 
        {
            curr_clicked = _last_clicked;
        }

        // test for click against other components
        for(auto it=_clickable_components.begin(); it!=_clickable_components.end(); ++it) {
            if(curr_clicked == nullptr && (*it) != _last_clicked 
                && !(*it)->anyStates(States::Disabled|States::Hidden|States::Unclickable) 
                && (*it)->containsMousePosition(_mouse_position))
            {
                curr_clicked = (*it);
            }
            else {
                (*it)->setState(States::Focused, false);
            }
        }

        // perform click logic
        if(curr_clicked != nullptr) {
            // test for re-click
            if(curr_clicked == _last_clicked
                && _last_click_button == event.mouseButton.button
                && _time_since_last_click <= CONSECUTIVE_CLICK_INTERVAL) 
            {
                ++_consecutive_clicks;
            }
            // no re-click, first click instead
            else {
                _last_clicked = curr_clicked;
                _last_click_button = event.mouseButton.button;
                _consecutive_clicks = 1;
            }
            // update universal information
            _time_since_last_click = 0;
            _clicked_this_frame = true;
            _mouse_held = true;

            // set focused state on
            curr_clicked->setState(States::Focused, true);
            // call component's click function, if true then snapshot is available
            snapshot_available = curr_clicked->click(_mouse_position, event.mouseButton.button);
        }

        // handle case where no GuiComponent has been clicked
        else {
            _last_clicked = nullptr;
            _last_click_button = event.mouseButton.button;
            _consecutive_clicks = 1;
            _time_since_last_click = 0;
            _clicked_this_frame = true;
            _mouse_held = true;
        }
    }

    // mouse button released or lost focus
    else if(event.type == sf::Event::MouseButtonReleased)
    {
        _mouse_held = false;
    }

    // lost focus
    else if(event.type == sf::Event::LostFocus) {
        _mouse_held = false;
        if(_last_clicked != nullptr) {
            _last_clicked->setState(States::Focused, false);
        }
    }

    else if(event.type == sf::Event::GainedFocus) {
        if(_last_clicked != nullptr) {
            _last_clicked->setState(States::Focused, true);
        }
    }

    // has no snapshot to request, so returns false
    return snapshot_available;
}
void MouseTracker::update(float t) {
    // set cursor
    if(_mouse_held && _last_clicked != nullptr)
        setWindowMouseCursor(_last_clicked->getClickCursor());
    else if(_top_hovering != nullptr)
        setWindowMouseCursor(_top_hovering->getHoverCursor());
    else
        setWindowMouseCursor(nullptr);

    // reset some frame-specific variables
    if(_clicked_this_frame)
        _clicked_this_frame = false;
    else
        _time_since_last_click += t;
    // _top_hovering = nullptr;
}



// mutators
void MouseTracker::addClickableComponent(GuiComponent& component) {
    _clickable_components.push_back(&component);
}
void MouseTracker::setClickableComponents(const std::vector<GuiComponent*>& clickable_components) {
    _clickable_components = clickable_components;
}
void MouseTracker::setFocusedComponent(GuiComponent& component) {
    if(_last_clicked == &component)
        return;

    // clear all other focused
    for(auto it=_clickable_components.begin(); it!=_clickable_components.end(); ++it)
        (*it)->setState(States::Focused, false);

    // set up focused component as if it were clicked
    _last_clicked = &component;
    _last_clicked->setState(States::Focused, true);
    _consecutive_clicks = 0;
    _time_since_last_click = 0;
    _clicked_this_frame = true;
    _mouse_held = false;
}
void MouseTracker::setRenderWindow(sf::RenderWindow& window) {
    _window = &window;
}
void MouseTracker::setWindowMouseCursor(sf::Cursor* cursor) {
    if(cursor != nullptr)
        _window->setMouseCursor(*cursor);
    else
        _window->setMouseCursor(_default_cursor);
}



// accessors
const std::vector<GuiComponent*>& MouseTracker::getClickableComponents() const {
    return _clickable_components;
}
const sf::RenderWindow* MouseTracker::getRenderWindow() const {
    return _window;
}



// global mouse event queries
const GuiComponent* MouseTracker::getLastClicked() const {
    return _last_clicked;
}
const sf::Vector2f& MouseTracker::getLastClickPosition() const {
    return _last_click_position;
}
const sf::Vector2f& MouseTracker::getMousePosition() const {
    return _mouse_position;
}
sf::Mouse::Button MouseTracker::getLastClickButton() const {
    return _last_click_button;
}
unsigned MouseTracker::getConsecutiveClicks() const {
    return _consecutive_clicks;
}
bool MouseTracker::getClickedThisFrame() const {
    return _clicked_this_frame;
}
bool MouseTracker::getMouseHeld() const {
    return _mouse_held;
}



// clickable-specific mouse event queries
bool MouseTracker::isLastClicked(const GuiComponent& component) const {
    return _last_clicked == &component;
}
bool MouseTracker::isClickedThisFrame(const GuiComponent& component) const {
    return isLastClicked(component) && _clicked_this_frame;
}
bool MouseTracker::isMouseHovering(const GuiComponent& component) const {
    return component.containsMousePosition(_mouse_position);
}
sf::Vector2f MouseTracker::calcLocalMousePosition(const GuiComponent& component) const {
    return component.getTransform().getInverse().transformPoint(_mouse_position);
}