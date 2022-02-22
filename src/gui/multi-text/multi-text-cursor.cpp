/* **************************************************************************** *
 * AUTHOR:      Noah Krim
 * ASSIGNMENT:  GUI Cloth Sim
 * CLASS:       CS_08
 * ---------------------------------------------------------------------------- *
 * File: multi-text-cursor.cpp
 *  Definition file for MultiTextCursor class
 * **************************************************************************** */

#include "multi-text-cursor.h"

#include <cmath>
#include <iostream>

/* ============================================================================ *
 * Constructors
 * ============================================================================ */
MultiTextCursor::MultiTextCursor(MultiText& text)
    : _shape() 
    , _loc()
    , _selection_anchor()
    , _text(text)
    , _index(0)
    , _selection_anchor_index(0)
    , _offset({0,8.0f})
    , _width(1.0f)
    , _blink_interval(DEFAULT_BLINK_INTERVAL)
    , _t(0)
    , _is_selecting(false)
{ 
    _shape.setSize({1.0f, (float)text.getDefaultLetterSize()});
    _loc = text.end();
    updatePosition();
}



/* ============================================================================ *
 * Destructor
 * ============================================================================ */
MultiTextCursor::~MultiTextCursor() { }

// inherited from sf::Drawable
void MultiTextCursor::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if(_t < 0.5*_blink_interval) {
        states.transform *= this->getTransform();
        target.draw(_shape, states);
    }
}



/* ============================================================================ *
 * Update Methods
 * ============================================================================ */
void MultiTextCursor::updatePosition() {
    // find position for cursor
    sf::Vector2f position {0,0};
    if(_index != 0) {
        if(_loc != _text.end()) {
            // step back a character if at newline
            if(_loc->getChar() == '\n') {
                if(_loc == _text.begin())
                    position = {0,0};
                else {
                    --_loc;
                    position = _loc->getPosition();
                    position.x += _loc->getAdvanceOffset();
                    ++_loc;
                }
            }
            else
                position = _loc->getPosition();
        }
        // position at end of _text
        else if(!_text.empty()) {
            const Letter& b = _text.back();
            position = b.getPosition();
            position.x += b.getAdvanceOffset();
        }
    }
    position += _offset;
    this->setPosition(position);
}
void MultiTextCursor::incrementCursorLocation() {
    // increment index
    ++_index;

    // increment _loc, if pointing within list
    if(_loc != _text.end())
        ++_loc;
    // otherwise, point _loc to _index in list
    else
        _loc = _text.iter_at(_index);

    // update cursor
    updatePosition();
    resetCursorTime();
}
void MultiTextCursor::decrementCursorLocation(bool iter_safe) {
    // if _index at 0, exit early
    if(_index == 0)
        return;

    // decrement index
    --_index;

    // decrement _loc, if pointing within list
    // - because decrement will happen after backspace deletion,
    //   _loc iterator may be unsafe to test
    if(iter_safe && _loc != _text.end())
        --_loc;
    // otherwise, point _loc to _index in list
    else
        _loc = _text.iter_at(_index);

    // update cursor
    updatePosition();
    resetCursorTime();
}



/* ============================================================================ *
 * Text Manipulation
 * ============================================================================ */
void MultiTextCursor::insertAtCursor(sf::Uint32 ch) {
    _text.insert(_loc, ch);
    ++_index;
    
    // update cursor
    updatePosition();
    resetCursorTime();
}

void MultiTextCursor::insertAtCursor(const sf::String& str) {
    for(auto it=str.begin(); it!=str.end(); ++it) {
        _text.insert(_loc, *it);
        ++_index;
        // don't move _loc as it should stay at the same node
    }
    
    // update cursor
    updatePosition();
    resetCursorTime();
}
sf::String MultiTextCursor::eraseAtCursor(unsigned count, bool return_erased) {
    if(count > _index)
        count = _index;
    if(count == 0)
        return sf::String();

    // initialize output string
    sf::String out;

    // reduce _index before adjusting count
    _index -= count;

    // get start iterator for erasure
    MultiText::iterator prev_loc = _loc;
    if(_loc == _text.end())
        _loc = _text.iter_at(_index);
    else {
        for(; count>0; --count)
            --_loc;
    }

    // if erased value requested, first copy into out
    if(return_erased) {
        for(auto it=_loc; it!=prev_loc; ++it)
            out += sf::String(it->getChar());
    }

    // erase and reset _loc to the node it was at before call
    _text.erase(_loc, prev_loc);
    _loc = prev_loc;

    // update cursor
    updatePosition();
    resetCursorTime();

    // return output string (filled or not)
    return out;
}



/* ============================================================================ *
 * Selection Manipulation
 * ============================================================================ */
void MultiTextCursor::startSelecting() {
    if(_is_selecting)
        return;

    _selection_anchor = _loc;
    _selection_anchor_index = _index;
    _is_selecting = true;
}
void MultiTextCursor::stopSelecting(bool set_loc_right) {
    if(!_is_selecting)
        return;
    if(_index == _selection_anchor_index) {
        _is_selecting = false;
        return;
    }

    _is_selecting = false;

    if(set_loc_right) {
        if(_index < _selection_anchor_index) {
            _index = _selection_anchor_index;
            _loc = _selection_anchor;
        }
    }
    else {
        if(_index > _selection_anchor_index) {
            _index = _selection_anchor_index;
            _loc = _selection_anchor;
        }
    }
}
unsigned MultiTextCursor::eraseSelection() {
    if(!_is_selecting || _index == _selection_anchor_index)
        return 0;

    unsigned num_erased = 0;

    // cursor is left side of selection
    if(_index < _selection_anchor_index) {
        _loc = _text.erase(_loc, _selection_anchor);
        num_erased = _selection_anchor_index - _index;
        // index remains the same
    }
    // cursor is right side of selection
    else {
        _loc = _text.erase(_selection_anchor, _loc);
        num_erased = _index - _selection_anchor_index;
    }

    // stop selecting (don't call function as it will change _loc)
    _is_selecting = false;

    // return the number of letters erased
    return num_erased;
}



/* ============================================================================ *
 * Selection Information
 * ============================================================================ */
bool MultiTextCursor::isSelecting() const {
    return _is_selecting && _index != _selection_anchor_index;
}
sf::String MultiTextCursor::getSelection() const {
    if(!_is_selecting || _index == _selection_anchor_index)
        return sf::String();

    // initialize output string
    sf::String out;

    // determine bounds for loop
    MultiText::iterator sel_begin, sel_end;
    if(_index < _selection_anchor_index) {
        sel_begin = _loc;
        sel_end = _selection_anchor;
    }
    else {
        sel_begin = _selection_anchor;
        sel_end = _loc;
    }

    // fill output selection string
    for(auto it=sel_begin; it!=sel_end; ++it) {
        out += it->getChar();
    }

    // return output string
    return out;
}



/* ============================================================================ *
 * Mutators
 * ============================================================================ */
void MultiTextCursor::setText(MultiText& text) {
    _text = text;
}
void MultiTextCursor::setCursorWidth(unsigned width) {
    _width = width;
}
void MultiTextCursor::setCursorIndex(unsigned index) {
    _index = std::min(index, _text.size());
    _loc = _text.iter_at(index);

    // update cursor
    updatePosition();
    resetCursorTime();
}
void MultiTextCursor::setCursorOffset(const sf::Vector2f& offset) {
    _offset = offset;
}
void MultiTextCursor::setCursorColor(const sf::Color& color) {
    _shape.setFillColor(color);
}
void MultiTextCursor::setBlinkInterval(float interval) {
    _blink_interval = interval;
}
void MultiTextCursor::resetCursorTime() {
    _t = 0;
}
void MultiTextCursor::setCursorTime(float t) {
    _t = t;
    if(_t >= _blink_interval)
        _t -= _blink_interval;
    // in case _t is still greater, use fmod
    if(_t >= _blink_interval)
        _t = fmod(_t, _blink_interval);
}
void MultiTextCursor::addCursorTime(float t) {
    _t += t;
    if(_t >= _blink_interval)
        _t -= _blink_interval;
    // in case _t is still greater, use fmod
    if(_t >= _blink_interval)
        _t = fmod(_t, _blink_interval);
}



/* ============================================================================ *
 * Accessors
 * ============================================================================ */
const MultiText& MultiTextCursor::getText() const {
    return _text;
}
unsigned MultiTextCursor::getCursorWidth() const {
    return _width;
}
MultiText::iterator MultiTextCursor::getCursorLocation() const {
    return _loc;
}
unsigned MultiTextCursor::getCursorIndex() const {
    return _index;
}
const sf::Vector2f& MultiTextCursor::getCursorOffset() const {
    return _offset;
}
const sf::Color& MultiTextCursor::getCursorColor() const {
    return _shape.getFillColor();
}
float MultiTextCursor::getBlinkInterval() const {
    return _blink_interval;
}
float MultiTextCursor::getCursorTime() const {
    return _t;
}