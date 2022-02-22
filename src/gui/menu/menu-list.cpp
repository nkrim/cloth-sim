 /* **************************************************************************** *
 * AUTHOR:      Noah Krim
 * ASSIGNMENT:  GUI Cloth Sim
 * CLASS:       CS_08
 * ---------------------------------------------------------------------------- *
 * File: menu-list.cpp
 *  Definition file for MenuList class
 * **************************************************************************** */

#include "menu-list.h"
#include <iostream>

/* ============================================================================ *
 * Constructors
 * ============================================================================ */
MenuList::MenuList()
    : _menu_header{}
    , _menu_items{}
    , _bg_shape{}
    , _snapshot{*this}
    , _controller{nullptr}
    , _open_last_frame{false}
{ 
    setItemLabelAlignment(Item::Left);
}
MenuList::MenuList(const sf::String& menu_label,
                           const sf::Font& font,
                           unsigned char_size)
    : _menu_header{menu_label, font, char_size}
    , _menu_items{font, char_size}
    , _bg_shape{}
    , _snapshot{*this}
    , _controller{nullptr}
    , _open_last_frame{false}
{ 
    setItemLabelAlignment(Item::Left);
}
MenuList::MenuList(const sf::String& menu_label,
             const std::vector<sf::String>& menu_items,
             MenuController* controller,
             const sf::Font& font,
             unsigned char_size)
    : _menu_header{menu_label, font, char_size}
    , _menu_items{font, char_size}
    , _bg_shape{}
    , _snapshot{*this}
    , _controller{controller}
    , _open_last_frame{false}
{
    setItemLabelAlignment(Item::Left);
    for(auto it=menu_items.begin(); it!=menu_items.end(); ++it) {
        _menu_items.addItem(*it);
    }
}

/* ============================================================================ *
 * Destructor
 * ============================================================================ */
MenuList::~MenuList()
{ }

/* ============================================================================ *
 * Inherited from GuiComponent (sf::Drawable)
 * ============================================================================ */
void MenuList::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(_bg_shape, states);
    target.draw(_menu_header, states);
    if(getState(States::Focused)) {
        target.draw(_menu_items, states);
    }
}

/* ============================================================================ *
 * Inhereted from GuiComponent (EventHandler)
 * ============================================================================ */
bool MenuList::addEventHandler(sf::RenderWindow& window, const sf::Event& event) {
    return false;
}
void MenuList::update(float t)
{ 
    _open_last_frame = getState(States::Focused);
    if(_open_last_frame)
        _menu_header.setState(States::Selected, true);
    else
        _menu_header.setState(States::Selected, false);
}

/* ============================================================================ *
 * Inherited from GuiComponent (Snapshotable)
 * ============================================================================ */
const Snapshot& MenuList::getSnapshot() const {
    return _snapshot;
}
void MenuList::applySnapshot(Snapshot& snapshot) {
    /* do nothing (yet) */
}
void MenuList::revertSnapshot(Snapshot& snapshot) {
    /* do nothing (yet) */
}

/* ============================================================================ *
 * Inherited from GuiComponent (Clickable)
 * ============================================================================ */
sf::FloatRect MenuList::getClickableBounds() const {
    if(getState(States::Focused))
        return getTransform().transformRect(_menu_items.getClickableBounds());
    return getTransform().transformRect(sf::FloatRect({0,0}, _bg_shape.getSize()));
    
}
bool MenuList::containsMousePosition(const sf::Vector2f& mouse_pos) const {
    sf::Vector2f local_mouse_pos = getInverseTransform().transformPoint(mouse_pos);
    if(_menu_header.containsMousePosition(local_mouse_pos))
        return true;
    if(getState(States::Focused)) {
        local_mouse_pos.y -= _menu_items.getPosition().y;
        float pos_y_offset = _menu_items.getItemDimensions().y;
        for(auto it=_menu_items.begin(); it!=_menu_items.end(); ++it) {
            if(it->second.containsMousePosition(local_mouse_pos)) {
                return true;
            }
            local_mouse_pos.y -= pos_y_offset;
        }
    }
    return false;
}
bool MenuList::click(const sf::Vector2f& click_pos, sf::Mouse::Button mouse_button) {
    if(!getState(States::Focused) || mouse_button != sf::Mouse::Left)
        return false;
    sf::Vector2f local_click_pos = getInverseTransform().transformPoint(click_pos);

    // test for click in _menu_header
    if(_open_last_frame && _menu_header.containsMousePosition(local_click_pos)) {
        setState(States::Focused, false);
    }

    // test for click in _menu_items
    local_click_pos.y -= _menu_items.getPosition().y;
    float pos_y_offset = _menu_items.getItemDimensions().y;
    bool any_clicked = false;
    for(auto it=_menu_items.begin(); !any_clicked && it!=_menu_items.end(); ++it) {
        if(it->second.getClickableBounds().contains(local_click_pos)) {
            any_clicked = true;
            if(_controller != nullptr)
                _controller->performMenuAction(it->first);
            setState(States::Focused, false);
        }
        local_click_pos.y -= pos_y_offset;
    }

    // ensure dropdown is unfocused if it was open last frame
    if(_open_last_frame)
        setState(States::Focused, false);
    
    return false;
}
bool MenuList::hover(const sf::Vector2f& hover_pos) {
    sf::Vector2f local_hover_pos = getInverseTransform().transformPoint(hover_pos);
    bool any_hovered = false;
    if(getState(States::Focused)) {
        any_hovered = _menu_items.hover(local_hover_pos);
    }

    if(_menu_header.getClickableBounds().contains(local_hover_pos)) {
        any_hovered = true;
        _menu_header.setState(States::Hovered, true);
    }
    else
        _menu_header.setState(States::Hovered, false);

    return any_hovered;
}
void MenuList::unhover() {
    _menu_header.setState(States::Hovered, false);
    _menu_items.unhover();
}

/* ============================================================================ *
 * Mutators - Dropdown Menu
 * ============================================================================ */
bool MenuList::selectMenuItem(unsigned id) {
    if(_controller != nullptr && _menu_items.getItemByID(id) != nullptr) {
        _controller->performMenuAction(id);
        return true;
    }
    return false;
}
void MenuList::setMenuLabel(const sf::String& menu_label) {
    _menu_header.setLabel(menu_label);
}
void MenuList::setMenuHeaderDimensions(const sf::Vector2f& dims) {
    _menu_header.setDimensions(dims);
    _bg_shape.setSize(dims);
    _updateMenuItemsPosition();
}
void MenuList::setMenuController(MenuController* controller) {
    _controller = controller;
}

/* ============================================================================ *
 * Accessors - Dropdown Menu
 * ============================================================================ */
const sf::String& MenuList::getMenuLabel() const {
    return _menu_header.getLabel();
}
const sf::Vector2f& MenuList::getMenuHeaderDimensions() const {
    return _bg_shape.getSize();
}
const MenuController* MenuList::getMenuController() const {
    return _controller;
}

/* ============================================================================ *
 * Mutators - Item List
 * ============================================================================ */
unsigned MenuList::addItem(const sf::String& label) {
    unsigned id = _menu_items.addItem(label);
    return id;
}
bool MenuList::addItem(const sf::String& label, unsigned id) {
    if(!_menu_items.addItem(label, id))
        return false;
    return true;
}
bool MenuList::removeItem(const sf::String& label) {
    return _menu_items.removeItem(label);
}
bool MenuList::removeItem(unsigned id) {
    return _menu_items.removeItem(id);
}

/* ============================================================================ *
 * Accessors - Item List
 * ============================================================================ */
const Item* MenuList::getItemByLabel(const sf::String& label) {
    return _menu_items.getItemByLabel(label);
}
const Item* MenuList::getItemByID(unsigned id) {
    return _menu_items.getItemByID(id);
}
bool MenuList::empty() const {
    return _menu_items.empty();
}
unsigned MenuList::size() const {
    return _menu_items.size();
}
MenuList::iterator MenuList::begin() {
    return _menu_items.begin();
}
MenuList::const_iterator MenuList::begin() const {
    return _menu_items.begin();
}
MenuList::iterator MenuList::end() {
    return _menu_items.end();
}
MenuList::const_iterator MenuList::end() const {
    return _menu_items.end();
}

/* ============================================================================ *
 * Mutators - Background
 * ============================================================================ */
void MenuList::setBackgroundFillColor(const sf::Color& bg_fill_color) {
    _bg_shape.setFillColor(bg_fill_color);
    _menu_items.setBackgroundFillColor(bg_fill_color);
}
void MenuList::setBackgroundOutlineColor(const sf::Color& bg_outline_color) {
    _bg_shape.setOutlineColor(bg_outline_color);
    _menu_items.setBackgroundOutlineColor(bg_outline_color);
}
void MenuList::setBackgroundOutlineThickness(float bg_outline_thickness) {
    _bg_shape.setOutlineThickness(bg_outline_thickness);
    _menu_items.setBackgroundOutlineThickness(bg_outline_thickness);
    _updateMenuItemsPosition();
}

/* ============================================================================ *
 * Accessors - Background
 * ============================================================================ */
const sf::Color& MenuList::getBackgroundFillColor() const {
    return _bg_shape.getFillColor();
}
const sf::Color& MenuList::getBackgroundOutlineColor() const {
    return _bg_shape.getOutlineColor();
}
float MenuList::getBackgroundOutlineThickness() const {
    return _bg_shape.getOutlineThickness();
}

/* ============================================================================ *
 * Mutators - Items
 * ============================================================================ */
void MenuList::setItemLabelAlignment(Item::LabelAlignment align) {
    _menu_header.setLabelAlignment(align);
    _menu_items.setItemLabelAlignment(align);
}
void MenuList::setItemPadding(float padding) {
    _menu_header.setPadding(padding);
    _menu_items.setItemPadding(padding);
}
void MenuList::setItemTextOffset(float text_offset) {
    _menu_header.setTextOffset(text_offset);
    _menu_items.setItemTextOffset(text_offset);
}
void MenuList::setItemDimensions(const sf::Vector2f& dims) {
    _menu_items.setItemDimensions(dims);
}
void MenuList::setItemFont(const sf::Font& font) {
    _menu_header.setFont(font);
    _menu_items.setItemFont(font);
}
void MenuList::setItemCharacterSize(unsigned char_size) {
    _menu_header.setCharacterSize(char_size);
    _menu_items.setItemCharacterSize(char_size);
}
void MenuList::setItemTextFillColor(const sf::Color& text_fill_color) {
    _menu_header.setTextFillColor(text_fill_color);
    _menu_items.setItemTextFillColor(text_fill_color);
}
void MenuList::setItemTextOutlineColor(const sf::Color& text_outline_color) {
    _menu_header.setTextOutlineColor(text_outline_color);
    _menu_items.setItemTextOutlineColor(text_outline_color);
}
void MenuList::setItemTextOutlineThickness(float text_outline_thickness) {
    _menu_header.setTextOutlineThickness(text_outline_thickness);
    _menu_items.setItemTextOutlineThickness(text_outline_thickness);
}
void MenuList::setItemHighlightFillColor(const sf::Color& highlight_fill_color) {
    _menu_header.setHighlightFillColor(highlight_fill_color);
    _menu_items.setItemHighlightFillColor(highlight_fill_color);
}
void MenuList::setItemHighlightOutlineColor(const sf::Color& highlight_outline_color) {
    _menu_header.setHighlightOutlineColor(highlight_outline_color);
    _menu_items.setItemHighlightOutlineColor(highlight_outline_color);
}
void MenuList::setItemHighlightOutlineThickness(float highlight_outline_thickness) {
    _menu_header.setHighlightOutlineThickness(highlight_outline_thickness);
    _menu_items.setItemHighlightOutlineThickness(highlight_outline_thickness);
}

/* ============================================================================ *
 * Accessors - Items
 * ============================================================================ */
Item::LabelAlignment MenuList::getItemLabelAlignment() const {
    return _menu_items.getItemLabelAlignment();
}
float MenuList::getItemPadding() const {
    return _menu_items.getItemPadding();
}
float MenuList::getItemTextOffset() const {
    return _menu_items.getItemTextOffset();
}
const sf::Vector2f& MenuList::getItemDimensions() const {
    return _menu_items.getItemDimensions();
}
const sf::Font* MenuList::getItemFont() const {
    return _menu_items.getItemFont();
}
unsigned MenuList::getItemCharacterSize() const {
    return _menu_items.getItemCharacterSize();
}
const sf::Color& MenuList::getItemTextFillColor() const {
    return _menu_items.getItemTextFillColor();
}
const sf::Color& MenuList::getItemTextOutlineColor() const {
    return _menu_items.getItemTextOutlineColor();
}
float MenuList::getItemTextOutlineThickness() const {
    return _menu_items.getItemTextOutlineThickness();
}
const sf::Color& MenuList::getItemHighlightFillColor() const {
    return _menu_items.getItemHighlightFillColor();
}
const sf::Color& MenuList::getItemHighlightOutlineColor() const {
    return _menu_items.getItemHighlightOutlineColor();
}
float MenuList::getItemHighlightOutlineThickness() const {
    return _menu_items.getItemHighlightOutlineThickness();
}

/* ============================================================================ *
 * Private Functions
 * ============================================================================ */
void MenuList::_updateMenuItemsPosition() {
    float y = getMenuHeaderDimensions().y + getBackgroundOutlineThickness();
    _menu_items.setPosition(0, y);
}