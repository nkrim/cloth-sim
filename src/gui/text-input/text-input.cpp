/* **************************************************************************** *
 * AUTHOR:      Noah Krim
 * ASSIGNMENT:  GUI Cloth Sim
 * CLASS:       CS_08
 * ---------------------------------------------------------------------------- *
 * File: text-input.cpp
 *  Defnition file for typable text-input class
 * **************************************************************************** */

#include "text-input.h"

#include "../../system/global-entities.h"
#include "../../system/key-shortcuts.h"
#include "../../system/mouse-tracker.h"
#include <iostream>

/* ============================================================================ *
 * Constructor
 * ============================================================================ */
TextInput::TextInput() 
    : _placeholder{}
    , _text{}
    , _cursor{_text}
    , _snapshot{*this}
    , _render_tex{}
    , _render_sprite{}
    , _render_rect{}
    , _text_offset{0,0}
    , _label{}
    , _min_label_width(0)
    , _multi_line{false}
{ 
    _placeholder.setDefaultFillColor(sf::Color(0xffffff7f));
    _render_rect.setFillColor(sf::Color::Transparent);
}

TextInput::TextInput(const sf::Font& font, const sf::Vector2u& dimensions, 
                     unsigned char_size, bool multi_line)
    :TextInput{sf::String(), sf::String(), font, dimensions, char_size, multi_line}
{ }

TextInput::TextInput(const sf::String& label, const sf::String& placeholder_str, 
                     const sf::Font& font, const sf::Vector2u& dimensions,
                     unsigned char_size, bool multi_line)
    : _placeholder{placeholder_str, font, char_size}
    , _text{sf::String(), font, char_size}
    , _cursor{_text}
    , _snapshot{*this}
    , _render_tex{}
    , _render_sprite{}
    , _render_rect{(sf::Vector2f)dimensions}
    , _text_offset{0,0}
    , _label{label, font, char_size}
    , _min_label_width(0)
    , _multi_line{multi_line}
{ 
    // initialize texture and sprite
    _render_tex.create(dimensions.x, dimensions.y);
    _render_sprite.setTexture(_render_tex.getTexture(), true);
    _render_sprite.setPosition({_label.getLocalBounds().width, 0});
    _render_rect.setPosition(_render_sprite.getPosition());

    // set some defaults that do not fit in constructors
    _placeholder.setAllFillColor(sf::Color(0xffffff7f));
    _render_rect.setFillColor(sf::Color::Transparent);
}

TextInput::TextInput(const TextInput& o)
    :GuiComponent(o)
    , _placeholder{o._placeholder}
    , _text{o._text}
    , _cursor{_text}
    , _snapshot{*this}
    , _render_tex{}
    , _render_sprite{o._render_sprite}
    , _render_rect{o._render_rect}
    , _text_offset{o._text_offset}
    , _label{o._label}
    , _min_label_width(o._min_label_width)
    , _multi_line{o._multi_line}
{
    // initialize texture and sprite
    _render_tex.create(o._render_tex.getSize().x, o._render_tex.getSize().y);
    _render_sprite.setTexture(_render_tex.getTexture(), true);

    // transfer some values from non-copyable members
    _cursor.setCursorColor(o.getCursorColor());
}



/* ============================================================================ *
 * Virtual Destructor
 * ============================================================================ */
TextInput::~TextInput() 
{ }



/* ============================================================================ *
 * inhereted from GuiComponent (Draw)
 * ============================================================================ */
void TextInput::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if(getState(States::Hidden))
        return;

    // draw contents to render texture (do not use given states as this is all local)
    // -------------------------------
    // clear texture
    _render_tex.clear(_render_rect.getFillColor());

    // draw text, or placeholder if no text
    sf::Transform text_transform;
    text_transform.translate(_text_offset);
    if(!_text.empty()) {
        _render_tex.draw(_text, text_transform);
    }
    else if(!_placeholder.empty()) {
        _render_tex.draw(_placeholder, text_transform);
    }
    // draw cursor
    if(getState(States::Focused))
        _render_tex.draw(_cursor, text_transform);

    // finalize render texture drawing
    _render_tex.display();

    // draw to final target
    // --------------------
    // apply transform to states
    states.transform *= getTransform();

    // draw label
    target.draw(_label, states);

    // draw fill and outline
    target.draw(_render_rect, states);

    // draw render texture to target
    target.draw(_render_sprite, states);
}



/* ============================================================================ *
 * inhereted from GuiComponent (EventHandler)
 * ============================================================================ */
bool TextInput::addEventHandler(sf::RenderWindow& window, const sf::Event& event) {
    if(!getState(States::Focused))
        return false;

    // flag for determining whether the _text object has changed
    bool text_changed = false;

    // initialize variables for snapshot values
    sf::String inserted, erased;
    unsigned cursor_index = _cursor.getCursorIndex();

    // text entered
    if(event.type == sf::Event::TextEntered) {
        // set text_changed
        text_changed = true;

        // ignore control characters
        if(!iscntrl(event.text.unicode)) {
            // insert into text
            _cursor.insertAtCursor(event.text.unicode);
            // set snapshot inserted value
            inserted += sf::String(event.text.unicode);
        }
        // TAB key redefinition
        else if(event.text.unicode == '\t') {
            // insert into text
            _cursor.insertAtCursor('\t');
            // set snapshot inserted value
            inserted += sf::String(event.text.unicode);
        }
        // ENTER key redefinition
        else if(_multi_line && event.text.unicode == '\n') {
            // insert into text
            _cursor.insertAtCursor('\n');
            // set snapshot inserted value
            inserted += sf::String(event.text.unicode);
        }
        // no change to str, unset text_changed 
        else
            text_changed = false;
    }

    // other keypresses
    else if(event.type == sf::Event::KeyPressed) {
        // auto modifiers = KeyShortcuts::getKeyModifiersPressed();

        // backspace pressed
        if( event.key.code == sf::Keyboard::Backspace
            && !_text.empty() && _cursor.getCursorIndex() > 0 ) 
        {
            // erase at cursor and append to erased for snapshot
            erased += _cursor.eraseAtCursor(1, true);
            // adjust cursor index for snapshot
            cursor_index = _cursor.getCursorIndex();
            // mark text_changed so snapshot will be filled and requested after
            text_changed = true;
        }
        // left arrow
        else if( event.key.code == sf::Keyboard::Left
           && !_text.empty() && _cursor.getCursorIndex() > 0 )
        {
            _cursor.decrementCursorLocation(true);
        }
        // right arrow
        else if( event.key.code == sf::Keyboard::Right
            && !_text.empty() && _cursor.getCursorIndex() < _text.size() )
        {
            _cursor.incrementCursorLocation();
        }
    }

    // actions for when text has changed
    if(text_changed) {
        // set new snapshot for current change
        _snapshot = StringChangeSnapshot(
            *this, inserted, erased, cursor_index
        );        
    }

    // return true if text has changed
    return text_changed;
}

void TextInput::update(float t) {
    // if not focused, don't update
    if(!getState(States::Focused))
        return;

    // check if just selected, then reset cursor timer
    if(Global::mouse_tracker.isClickedThisFrame(*this)
        && Global::mouse_tracker.getConsecutiveClicks() <= 1)
    {
        _cursor.resetCursorTime();
    }
    // otherwise, if selected, add cursor time
    else
        _cursor.addCursorTime(t);
}



/* ============================================================================ *
 * inhereted from GuiComponent (Snapshotable)
 * ============================================================================ */
const Snapshot& TextInput::getSnapshot() const {
    return _snapshot;
}

void TextInput::applySnapshot(Snapshot& snapshot) {
    // Initialize casting pointers to nullptr
    StringChangeSnapshot* scs_ptr = nullptr;
    
    // try to cast to StringChangeSnapshot*
    if((scs_ptr=dynamic_cast<StringChangeSnapshot*>(&snapshot)) != nullptr) {
        // grab position iterator
        unsigned start_index = scs_ptr->getStartIndex();
        MultiText::iterator pos = _text.iter_at(start_index);
        const sf::String& inserted = scs_ptr->getInserted();
        const sf::String& erased = scs_ptr->getErased();

        // removal
        if(!erased.isEmpty()) {    
            pos = _text.erase(pos, erased.getSize());
        }
        // insertion
        if(!inserted.isEmpty()) {    
            for(auto it=inserted.begin(); it!=inserted.end(); ++it) {
                pos = _text.insert(pos, *it);
                ++pos;
            }
        }

        // select this from global_tracker
        Global::mouse_tracker.setFocusedComponent(*this);

        // update cursor
        _cursor.setCursorIndex(start_index + inserted.getSize());
        _cursor.updatePosition();
    }

    // invalid snapshot
    else {
        throw InvalidSnapshotException();
    }
}

void TextInput::revertSnapshot(Snapshot& snapshot) {
    // Initialize casting pointers to nullptr
    StringChangeSnapshot* scs_ptr = nullptr;
    
    // try to cast to StringChangeSnapshot*
    if((scs_ptr=dynamic_cast<StringChangeSnapshot*>(&snapshot)) != nullptr) {
        // grab position iterator
        unsigned start_index = scs_ptr->getStartIndex();
        MultiText::iterator pos = _text.iter_at(start_index);
        const sf::String& inserted = scs_ptr->getInserted();
        const sf::String& erased = scs_ptr->getErased();

        // insertion (revert, so remove)
        if(!inserted.isEmpty()) {
            pos = _text.erase(pos, inserted.getSize());
        }
        // removal (revert, so insert)
        if(!erased.isEmpty()) {
            for(auto it=erased.begin(); it!=erased.end(); ++it) {
                pos = _text.insert(pos, *it);
                ++pos;
            }
        }

        // select this from global_tracker
        Global::mouse_tracker.setFocusedComponent(*this);

        // update cursor
        _cursor.setCursorIndex(start_index + erased.getSize());
        _cursor.updatePosition();
    }

    // invalid snapshot
    else {
        throw InvalidSnapshotException();
    }
}



/* ============================================================================ *
 * inhereted from GuiComponent (Clickable)
 * ============================================================================ */
sf::FloatRect TextInput::getClickableBounds() const {
    return getGlobalBounds();
}

// bool TextInput::click(const sf::Vector2f& click_pos) {
//     return true;
// }

// bool TextInput::hover(const sf::Vector2f& hover_pos) {
//     return true;
// }



/* ============================================================================ *
 * Bounds Information
 * ============================================================================ */
sf::FloatRect TextInput::getLocalBounds() const {
    return sf::FloatRect(_render_sprite.getPosition(), (sf::Vector2f)getDimensions());
}
sf::FloatRect TextInput::getGlobalBounds() const {
    return getTransform().transformRect(getLocalBounds());
}



/* ============================================================================ *
 * Mutators
 * ============================================================================ */
void TextInput::setText(const MultiText& text) {
    _text = text;
    _cursor.setCursorIndex(_cursor.getCursorIndex());
    _cursor.updatePosition();
}
void TextInput::setTextString(const sf::String& text_str) {
    _text.setString(text_str);
    _cursor.setCursorIndex(_cursor.getCursorIndex());
    _cursor.updatePosition();
}
void TextInput::setPlaceholder(const MultiText& placeholder) {
    _placeholder = placeholder;
    _cursor.updatePosition();
}
void TextInput::setPlaceholderString(const sf::String& placeholder_str) {
    _placeholder.setString(placeholder_str);
    _cursor.updatePosition();
}
void TextInput::setFont(const sf::Font& font) {
    _text.setAllFont(font);
    _placeholder.setAllFont(font);
    _label.setFont(font);
    _cursor.updatePosition();
}
void TextInput::setCharacterSize(unsigned size) {
    _text.setAllLetterSize(size);
    _placeholder.setAllLetterSize(size);
    _cursor.updatePosition();
}
void TextInput::setLetterSpacing(float letter_spacing) {
    _text.setAllLetterSpacing(letter_spacing);
    _placeholder.setAllLetterSpacing(letter_spacing);
    _cursor.updatePosition();
}
void TextInput::setLineSpacing(float line_spacing) {
    _text.setLineSpacing(line_spacing);
    _placeholder.setLineSpacing(line_spacing);
    _cursor.updatePosition();
}
void TextInput::setDimensions(const sf::Vector2u& dims) {
    _render_tex.create(dims.x, dims.y);
    _render_sprite.setTexture(_render_tex.getTexture(), true);
    _render_rect.setSize((sf::Vector2f)dims);
}
void TextInput::setMultiLine(bool multi_line) {
    _multi_line = multi_line;
}
void TextInput::setLabel(const sf::String& label) {
    _label.setString(label);
    _label.setPosition(
        { std::max(0.0f, _min_label_width-_label.getLocalBounds().width)
        , _text_offset.y}
    );
    _render_sprite.setPosition({_label.getPosition().x + _label.getLocalBounds().width, 0});
    _render_rect.setPosition(_render_sprite.getPosition());
}
void TextInput::setTextFillColor(const sf::Color& text_fill_color) {
    _text.setAllFillColor(text_fill_color);
    _label.setFillColor(text_fill_color);
}
void TextInput::setPlaceholderFillColor(const sf::Color& placeholder_fill_color) {
    _placeholder.setAllFillColor(placeholder_fill_color);
}
void TextInput::setFillColor(const sf::Color& fill_color) {
    _render_rect.setFillColor(fill_color);
}
void TextInput::setOutlineColor(const sf::Color& outline_color) {
    _render_rect.setOutlineColor(outline_color);
}
void TextInput::setOutlineThickness(float outline_thickness) {
    _render_rect.setOutlineThickness(outline_thickness);
}
void TextInput::setCursorColor(const sf::Color& cursor_color) {
    _cursor.setCursorColor(cursor_color);
}
void TextInput::setTextOffset(const sf::Vector2f& text_offset) {
    _text_offset = text_offset;
    _label.setPosition({0, _text_offset.y});
}
void TextInput::setMinLabelWidth(float min_label_width) {
    _min_label_width = min_label_width;
    _label.setPosition(
        { std::max(0.0f, _min_label_width-_label.getLocalBounds().width)
        , _text_offset.y}
    );
    _render_sprite.setPosition({_label.getPosition().x + _label.getLocalBounds().width, 0});
    _render_rect.setPosition(_render_sprite.getPosition());
}



/* ============================================================================ *
 * Accessors
 * ============================================================================ */
const MultiText& TextInput::getText() const {
    return _text;
}
const MultiText& TextInput::getPlaceholder() const {
    return _placeholder;
}
const sf::Font* TextInput::getFont() const {
    return _text.getDefaultFont();
}
unsigned TextInput::getCharacterSize() const {
    return _text.getDefaultLetterSize();
}
float TextInput::getLetterSpacing() const {
    return _text.getDefaultLetterSpacing();
}
float TextInput::getLineSpacing() const {
    return _text.getLineSpacing();
}
unsigned TextInput::getTextLength() const {
    return _text.size();
}
sf::Vector2u TextInput::getDimensions() const {
    return _render_tex.getSize();
}
bool TextInput::getMultiLine() const {
    return _multi_line;
}
const sf::String& TextInput::getLabel() const {
    return _label.getString();
}
const sf::Color& TextInput::getTextFillColor() const {
    return _text.getDefaultFillColor();   
}
const sf::Color& TextInput::getPlaceholderFillColor() const {
    return _placeholder.getDefaultFillColor();
}
const sf::Color& TextInput::getFillColor() const {
    return _render_rect.getFillColor();
}
const sf::Color& TextInput::getOutlineColor() const {
    return _render_rect.getOutlineColor();
}
float TextInput::getOutlineThickness() const {
    return _render_rect.getOutlineThickness();
}
const sf::Color& TextInput::getCursorColor() const {
    return _cursor.getCursorColor();
}
const sf::Vector2f& TextInput::getTextOffset() const {
    return _text_offset;
}
float TextInput::getMinLabelWidth() const {
    return _min_label_width;
}



/* ============================================================================ *
 * Private Functions
 * ============================================================================ */
