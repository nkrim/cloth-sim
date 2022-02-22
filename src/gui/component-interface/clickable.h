/* **************************************************************************** *
 * AUTHOR:      Noah Krim
 * ASSIGNMENT:  GUI Cloth Sim
 * CLASS:       CS_08
 * ---------------------------------------------------------------------------- *
 * File: clickable.h
 *  Header file for Clickable class
 * **************************************************************************** */

#ifndef CLICKABLE_H
#define CLICKABLE_H

#include <SFML/Graphics.hpp>

#include "states.h"

class Clickable {
private:
    sf::Cursor* _click_cursor;
    sf::Cursor* _hover_cursor;

public:
    // virtual functions
    virtual sf::FloatRect getClickableBounds() const = 0;
    virtual bool containsMousePosition(const sf::Vector2f& mouse_pos) const {
        return getClickableBounds().contains(mouse_pos);
    }
    // optional virtual functions
    // - click() 
    // - return signal of true indicates that a snapshot is ready for history
    virtual bool click(const sf::Vector2f& click_pos, sf::Mouse::Button mouse_button) { 
        return false;
    }
    // virtual void clickOut(const sf::Vector2f& click_pos)
    // { }
    // - hover() 
    // - return signal of true indicates that this Clickable's hover_cursor should be used
    virtual bool hover(const sf::Vector2f& hover_pos) {
        return true;
    }
    virtual void unhover() 
    { }

    // clickable constructor
    Clickable() 
        : _click_cursor{nullptr}, _hover_cursor{nullptr}
    { }

    // hover cursor type getter/setter
    void setClickCursor(sf::Cursor& click_cursor) {
        _click_cursor = &click_cursor;
    }
    sf::Cursor* getClickCursor() {
        return _click_cursor;
    }
    void setHoverCursor(sf::Cursor& hover_cursor) {
        _hover_cursor = &hover_cursor;
    }
    sf::Cursor* getHoverCursor() {
        return _hover_cursor;
    }
};

#endif