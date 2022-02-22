/* **************************************************************************** *
 * AUTHOR:      Noah Krim
 * ASSIGNMENT:  GUI Cloth Sim
 * CLASS:       CS_08
 * ---------------------------------------------------------------------------- *
 * File: help-text-box.cpp
 *  Definition file for HelpTextBox class
 * **************************************************************************** */

#include "help-text-box.h"

// constructors
HelpTextBox::HelpTextBox()
    : _col_1()
    , _col_2()
    , _rect()
    , _text_offset()
    , _snapshot(*this)
{ }
HelpTextBox::HelpTextBox(const sf::Font& font, unsigned char_size)
    : _col_1(sf::String(), font, char_size)
    , _col_2(sf::String(), font, char_size)
    , _rect()
    , _text_offset()
    , _snapshot(*this)
{ }

// destructor
HelpTextBox::~HelpTextBox()
{ }

// inherited from GuiComponent (sf::Drawable)
void HelpTextBox::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(_rect, states);
    target.draw(_col_1, states);
    target.draw(_col_2, states);
}

// inherited from GuiComponent (EventHandler)
bool HelpTextBox::addEventHandler(sf::RenderWindow& window, const sf::Event& event) {
    return false;
}
void HelpTextBox::update(float t) {
    /* do nothing */
}

// inherited from GuiComponent (Snapshotable)
const Snapshot& HelpTextBox::getSnapshot() const {
    return _snapshot;
}
void HelpTextBox::applySnapshot(Snapshot& snapshot) {
    /* do nothing */
}
void HelpTextBox::revertSnapshot(Snapshot& snapshot) {
    /* do nothing */
}

// inherited from GuiComponent (Clickable)
sf::FloatRect HelpTextBox::getClickableBounds() const {
    return getTransform().transformRect(_rect.getLocalBounds());
}

// mutators
void HelpTextBox::setFont(const sf::Font& font) {
    _col_1.setFont(font);
    _col_2.setFont(font);
}
void HelpTextBox::setCharSize(unsigned char_size) {
    _col_1.setCharacterSize(char_size);
    _col_2.setCharacterSize(char_size);
}
void HelpTextBox::setSize(const sf::Vector2f& size) {
    _rect.setSize(size);
    _col_2.setPosition(sf::Vector2f{_rect.getSize().x/2.0f, _text_offset.y});
}
void HelpTextBox::setText(const sf::String& col_1, const sf::String& col_2) {
    _col_1.setString(col_1);
    _col_2.setString(col_2);
}
void HelpTextBox::setTextFillColor(const sf::Color& text_color) {
    _col_1.setFillColor(text_color);
    _col_2.setFillColor(text_color);
}
void HelpTextBox::setFillColor(const sf::Color& fill_color) {
    _rect.setFillColor(fill_color);
}
void HelpTextBox::setOutlineColor(const sf::Color& outline_color) {
    _rect.setOutlineColor(outline_color);
}
void HelpTextBox::setOutlineThickness(float outline_thickness) {
    _rect.setOutlineThickness(outline_thickness);
}
void HelpTextBox::setTextOffset(const sf::Vector2f& text_offset) {
    _text_offset = text_offset;
    _col_1.setPosition(_text_offset);
    _col_2.setPosition(sf::Vector2f{_rect.getSize().x/2.0f, _text_offset.y});
}