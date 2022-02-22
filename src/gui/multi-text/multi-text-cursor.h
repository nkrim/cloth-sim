/* **************************************************************************** *
 * AUTHOR:      Noah Krim
 * ASSIGNMENT:  GUI Cloth Sim
 * CLASS:       CS_08
 * ---------------------------------------------------------------------------- *
 * File: multi-text-cursor.h
 *  Header file for MultiTextCursor class
 * **************************************************************************** */

#ifndef MULTI_TEXT_CURSOR_H
#define MULTI_TEXT_CURSOR_H

#include <SFML/Graphics.hpp>

#include "multi-text.h"

class MultiTextCursor : protected sf::Transformable, public sf::Drawable {
private:
    sf::RectangleShape _shape;
    MultiText::iterator _loc;
    MultiText::iterator _selection_anchor;
    MultiText& _text; // reference to text object
    unsigned _index;
    unsigned _selection_anchor_index;
    sf::Vector2f _offset;
    float _width;
    float _blink_interval;
    float _t;
    bool _is_selecting;

protected:
    // inherited from sf::Drawable
    virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;

public:
    // global constants
    static constexpr float DEFAULT_BLINK_INTERVAL = 1.0f;

    // constructor
    MultiTextCursor(MultiText& text);

    // destructor
    virtual ~MultiTextCursor();

    // cursor update methods
    void updatePosition();
    void incrementCursorLocation();
    void decrementCursorLocation(bool iter_safe=false);

    // text manipulation
    void insertAtCursor(sf::Uint32 ch);
    void insertAtCursor(const sf::String& str);
    sf::String eraseAtCursor(unsigned count=1, bool return_erased=false);

    // selection manipulation
    void startSelecting();
    void stopSelecting(bool set_loc_right=true);
    unsigned eraseSelection();

    // selection information
    bool isSelecting() const;
    sf::String getSelection() const;

    // mutators
    void setText(MultiText& text);
    void setCursorWidth(unsigned width);
    void setCursorIndex(unsigned index);
    void setCursorOffset(const sf::Vector2f& offset);
    void setCursorColor(const sf::Color& color);
    void setBlinkInterval(float interval);
    void resetCursorTime();
    void setCursorTime(float t);
    void addCursorTime(float t);

    // accessors
    const MultiText& getText() const;
    unsigned getCursorWidth() const;
    MultiText::iterator getCursorLocation() const;
    unsigned getCursorIndex() const;
    const sf::Vector2f& getCursorOffset() const;
    const sf::Color& getCursorColor() const;
    float getBlinkInterval() const;
    float getCursorTime() const;
};

#endif