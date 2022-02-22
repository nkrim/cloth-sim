/* **************************************************************************** *
 * AUTHOR:      Noah Krim
 * ASSIGNMENT:  GUI Cloth Sim
 * CLASS:       CS_08
 * ---------------------------------------------------------------------------- *
 * File: button.cpp
 *  Definition file for abstract Button class
 * **************************************************************************** */

#include "button.h"

// constructors
Button::Button()
    : _item()
    , _snapshot(*this)
    , _rect()
    , _clicked_this_frame(false)
{ }
Button::Button( const sf::Vector2f& size,
                const sf::String& label)
    : _item(label)
    , _snapshot(*this)
    , _rect(size)
    , _clicked_this_frame(false)
{ 
    _item.setDimensions(size);
}
Button::Button( const sf::Vector2f& size,
                const sf::String& label, 
                const sf::Font& font, 
                unsigned char_size)
    : _item(label, font, char_size)
    , _snapshot(*this)
    , _rect(size)
    , _clicked_this_frame(false)
{
    _item.setDimensions(size);
}

// destructor
Button::~Button()
{ }

// inherited from GuiComponent (sf::Drawable)
void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(_rect, states);
    target.draw(_item, states);
}

// inherited from GuiComponent (EventHandler)
bool Button::addEventHandler(sf::RenderWindow& window, const sf::Event& event) {
    return false;
}
void Button::update(float t) {
    _clicked_this_frame = false;
}

// inherited from GuiComponent (Snapshotable)
const Snapshot& Button::getSnapshot() const {
    return _snapshot;
}
void Button::applySnapshot(Snapshot& snapshot) 
{ }
void Button::revertSnapshot(Snapshot& snapshot) 
{ }

// inherited from GuiComponent (Clickable)
sf::FloatRect Button::getClickableBounds() const {
    return getTransform().transformRect(_item.getClickableBounds());
}
bool Button::click(const sf::Vector2f& click_pos, sf::Mouse::Button mouse_button) {
    _clicked_this_frame = true;
    return false;
}
bool Button::hover(const sf::Vector2f& hover_pos) {
    _item.setState(States::Hovered, true);
    return true;
}
void Button::unhover() {
    _item.setState(States::Hovered, false);
}

// button functions
bool Button::clickedThisFrame() const {
    return _clicked_this_frame;
}

// mutators
void Button::setLabel(const sf::String& label) {
    _item.setLabel(label);
}
void Button::setLabelAlignment(Item::LabelAlignment align) {
    _item.setLabelAlignment(align);
}
void Button::setPadding(float padding) {
    _item.setPadding(padding);
}
void Button::setTextOffset(float text_offset) {
    _item.setTextOffset(text_offset);
}
void Button::setDimensions(const sf::Vector2f& dims) {
    _item.setDimensions(dims);
}
void Button::setFont(const sf::Font& font) {
    _item.setFont(font);
}
void Button::setCharacterSize(unsigned char_size) {
    _item.setCharacterSize(char_size);
}
void Button::setTextFillColor(const sf::Color& text_fill_color) {
    _item.setTextFillColor(text_fill_color);
}
void Button::setTextOutlineColor(const sf::Color& text_outline_color) {
    _item.setTextOutlineColor(text_outline_color);
}
void Button::setTextOutlineThickness(float text_outline_thickness) {
    _item.setTextOutlineThickness(text_outline_thickness);
}
void Button::setHighlightFillColor(const sf::Color& highlight_fill_color) {
    _item.setHighlightFillColor(highlight_fill_color);
}
void Button::setHighlightOutlineColor(const sf::Color& highlight_outline_color) {
    _item.setHighlightOutlineColor(highlight_outline_color);
}
void Button::setHighlightOutlineThickness(float highlight_outline_thickness) {
    _item.setHighlightOutlineThickness(highlight_outline_thickness);
}
void Button::setFillColor(const sf::Color& fill_color) {
    _rect.setFillColor(fill_color);
}
void Button::setOutlineColor(const sf::Color& outline_color) {
    _rect.setOutlineColor(outline_color);
}
void Button::setOutlineThickness(float outline_thickness) {
    _rect.setOutlineThickness(outline_thickness);
}

// accessors
float Button::getAdvanceOffset() const {
    return _item.getAdvanceOffset();
}
const sf::String& Button::getLabel() const {
    return _item.getLabel();
}
Item::LabelAlignment Button::getLabelAlignment() const {
    return _item.getLabelAlignment();
}
float Button::getPadding() const {
    return _item.getPadding();
}
float Button::getTextOffset() const {
    return _item.getTextOffset();
}
const sf::Vector2f& Button::getDimensions() const {
    return _item.getDimensions();
}
const sf::Font* Button::getFont() const {
    return _item.getFont();
}
unsigned Button::getCharacterSize() const {
    return _item.getCharacterSize();
}
const sf::Color& Button::getTextFillColor() const {
    return _item.getTextFillColor();
}
const sf::Color& Button::getTextOutlineColor() const {
    return _item.getTextOutlineColor();
}
float Button::getTextOutlineThickness() const {
    return _item.getTextOutlineThickness();
}
const sf::Color& Button::getHighlightFillColor() const {
    return _item.getHighlightFillColor();
}
const sf::Color& Button::getHighlightOutlineColor() const {
    return _item.getHighlightOutlineColor();
}
float Button::getHighlightOutlineThickness() const {
    return _item.getHighlightOutlineThickness();
}
const sf::Color& Button::getFillColor() const {
    return _rect.getFillColor();
}
const sf::Color& Button::getOutlineColor() const {
    return _rect.getOutlineColor();
}
float Button::getOutlineThickness() const {
    return _rect.getOutlineThickness();
}
