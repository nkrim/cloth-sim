/* **************************************************************************** *
 * AUTHOR:      Noah Krim
 * ASSIGNMENT:  SFML Dropdown
 * CLASS:       CS_08
 * ---------------------------------------------------------------------------- *
 * File: dropdown-menu.cpp
 *  Definition file for DropdownMenu class
 * **************************************************************************** */

#include "dropdown-menu.h"
#include <iostream>

/* ============================================================================ *
 * Constructors
 * ============================================================================ */
DropdownMenu::DropdownMenu()
    : _item_list{}
    , _placeholder_label{}
    , _showcased_item{}
    , _selected_item_id{PLACEHOLDER_ID}
    , _bg_shape{}
    , _snapshot{*this}
    , _placeholder_enabled{false}
    , _open_last_frame{false}
{ }
DropdownMenu::DropdownMenu(const std::vector<sf::String>& items,
                           const sf::Font& font,
                           unsigned char_size)
    : _item_list{font, char_size}
    , _placeholder_label{}
    , _showcased_item{sf::String(), font, char_size}
    , _selected_item_id{PLACEHOLDER_ID}
    , _bg_shape{}
    , _snapshot{*this}
    , _placeholder_enabled{false}
    , _open_last_frame{false}
{ 
    for(auto it=items.begin(); it!=items.end(); ++it) {
        _item_list.addItem(*it);
    }
    if(!_item_list.empty()) {
        _selected_item_id = _item_list.begin()->first;
        _showcased_item.setLabel(_item_list.begin()->second.getLabel());
    }
    _updateItemListPosition();
}
DropdownMenu::DropdownMenu(const std::vector<sf::String>& items,
                           const sf::Font& font,
                           unsigned char_size,
                           const sf::String& placeholder)
    : _item_list{font, char_size}
    , _placeholder_label{placeholder}
    , _showcased_item{placeholder, font, char_size}
    , _selected_item_id{PLACEHOLDER_ID}
    , _bg_shape{}
    , _snapshot{*this}
    , _placeholder_enabled{true}
    , _open_last_frame{false}
{
    for(auto it=items.begin(); it!=items.end(); ++it) {
        _item_list.addItem(*it);
    }
    _updateItemListPosition();
}

/* ============================================================================ *
 * Destructor
 * ============================================================================ */
DropdownMenu::~DropdownMenu()
{ }

/* ============================================================================ *
 * Inherited from GuiComponent (sf::Drawable)
 * ============================================================================ */
void DropdownMenu::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(_bg_shape, states);
    if(hasSelection() || _placeholder_enabled)
        target.draw(_showcased_item, states);
    if(getState(States::Focused)) {
        target.draw(_item_list, states);
    }
}

/* ============================================================================ *
 * Inhereted from GuiComponent (EventHandler)
 * ============================================================================ */
bool DropdownMenu::addEventHandler(sf::RenderWindow& window, const sf::Event& event) {
    return false;
}
void DropdownMenu::update(float t)
{ 
    _open_last_frame = getState(States::Focused);
}

/* ============================================================================ *
 * Inherited from GuiComponent (Snapshotable)
 * ============================================================================ */
const Snapshot& DropdownMenu::getSnapshot() const {
    return _snapshot;
}
void DropdownMenu::applySnapshot(Snapshot& snapshot) {
    IntSnapshot* is_ptr = nullptr;
    if((is_ptr = dynamic_cast<IntSnapshot*>(&snapshot)) != nullptr) {
        setSelectedItem(is_ptr->getAfter());
    }
}
void DropdownMenu::revertSnapshot(Snapshot& snapshot) {
    IntSnapshot* is_ptr = nullptr;
    if((is_ptr = dynamic_cast<IntSnapshot*>(&snapshot)) != nullptr) {
        setSelectedItem(is_ptr->getBefore());
    }
}

/* ============================================================================ *
 * Inherited from GuiComponent (Clickable)
 * ============================================================================ */
sf::FloatRect DropdownMenu::getClickableBounds() const {
    sf::Vector2f size = _bg_shape.getSize();
    if(getState(States::Focused))
        size.y += _item_list.getPosition().y + _item_list.getClickableBounds().height;
    return getTransform().transformRect(sf::FloatRect({0,0}, size));
}
bool DropdownMenu::click(const sf::Vector2f& click_pos, sf::Mouse::Button mouse_button) {
    if(!getState(States::Focused) || mouse_button != sf::Mouse::Left)
        return false;
    sf::Vector2f local_click_pos = getInverseTransform().transformPoint(click_pos);

    // test for click in _item_list
    local_click_pos.y -= _item_list.getPosition().y;
    float pos_y_offset = _item_list.getItemAdvanceOffset();
    bool any_clicked = false;
    bool new_selected = false;
    for(auto it=_item_list.begin(); it!=_item_list.end(); ++it) {
        if(!any_clicked && it->second.getClickableBounds().contains(local_click_pos)) {
            any_clicked = true;
            if(_selected_item_id != it->first) {
                new_selected = true; 
                _snapshot = IntSnapshot(*this, _selected_item_id, it->first);
                setSelectedItem(it->first);
            }
            it->second.setState(States::Selected, true);
            // close dropdown on item select
            setState(States::Focused, false);
        }
        else if(_open_last_frame){
            it->second.setState(States::Selected, false);
        }
        local_click_pos.y -= pos_y_offset;
    }

    // ensure dropdown is unfocused if it was open last frame
    if(_open_last_frame)
        setState(States::Focused, false);

    return new_selected;
}
bool DropdownMenu::hover(const sf::Vector2f& hover_pos) {
    sf::Vector2f local_hover_pos = getInverseTransform().transformPoint(hover_pos);
    bool any_hovered = false;
    if(_showcased_item.getClickableBounds().contains(local_hover_pos)) {
        any_hovered = true;
        _showcased_item.setState(States::Hovered, true);
        _item_list.unhover();
    }
    else {
        _showcased_item.setState(States::Hovered, false);
    }
    
    if(!any_hovered && getState(States::Focused)) { 
        any_hovered = _item_list.hover(local_hover_pos);
    }
    return any_hovered;
}
void DropdownMenu::unhover() {
    _showcased_item.setState(States::Hovered, false);
    _item_list.unhover();
}

/* ============================================================================ *
 * Mutators - Dropdown Menu
 * ============================================================================ */
bool DropdownMenu::setSelectedItem(unsigned id) {
    bool found_selected = false;
    if(_placeholder_enabled && id == PLACEHOLDER_ID) {
        found_selected = true;
        _selected_item_id = id;
        _showcased_item.setLabel(_placeholder_label);
    }
    for(auto it=_item_list.begin(); it!=_item_list.end(); ++it) {
        if(it->first == id) {
            found_selected = true;
            _selected_item_id = it->first;
            _showcased_item.setLabel(it->second.getLabel());
            it->second.setState(States::Selected, true);
        }
        else
            it->second.setState(States::Selected, false);
    }
    return found_selected;
}
bool DropdownMenu::setSelectedItem(const sf::String& label) {
    bool found_selected = false;
    for(auto it=_item_list.begin(); it!=_item_list.end(); ++it) {
        if(it->second.getLabel() == label) {
            found_selected = true;
            _selected_item_id = it->first;
            _showcased_item.setLabel(it->second.getLabel());
            it->second.setState(States::Selected, true);
        }
        else
            it->second.setState(States::Selected, false);
    }
    return found_selected;
}
void DropdownMenu::setShowcasedItemHeight(float height) {
    _showcased_item.setDimensions({_showcased_item.getDimensions().x, height});
    _bg_shape.setSize({_bg_shape.getSize().x, height});
    _updateItemListPosition();
}
void DropdownMenu::setPlaceholderLabel(const sf::String& placeholder) {
    _placeholder_label = placeholder;
}
void DropdownMenu::setPlaceholderEnabled(bool enabled) {
    _placeholder_enabled = enabled;
    _selected_item_id = PLACEHOLDER_ID;
}

/* ============================================================================ *
 * Accessors - Dropdown Menu
 * ============================================================================ */
bool DropdownMenu::hasSelection() const {
    return _selected_item_id != PLACEHOLDER_ID && _item_list.getItemByID(_selected_item_id) != nullptr;
}
unsigned DropdownMenu::getSelectedItemID() const {
    if(hasSelection())
        return _selected_item_id;
    return PLACEHOLDER_ID;
}
const sf::String& DropdownMenu::getSelectedItemLabel() const {
    if(hasSelection()) 
        return _item_list.getItemByID(_selected_item_id)->getLabel();
    return _placeholder_label;
}
float DropdownMenu::getShowcasedItemHeight() const {
    return _bg_shape.getSize().y;
}
const sf::String& DropdownMenu::getPlaceholderLabel() const {
    return _placeholder_label;
}
bool DropdownMenu::getPlaceholderEnabled() const {
    return _placeholder_enabled;
}

/* ============================================================================ *
 * Mutators - Item List
 * ============================================================================ */
unsigned DropdownMenu::addItem(const sf::String& label) {
    unsigned id = _item_list.addItem(label);
    if(!_placeholder_enabled && _selected_item_id == PLACEHOLDER_ID)
        _selected_item_id = id;
    return id;
}
bool DropdownMenu::addItem(const sf::String& label, unsigned id) {
    if(!_item_list.addItem(label, id))
        return false;
    if(!_placeholder_enabled && _selected_item_id == PLACEHOLDER_ID)
        _selected_item_id = id;
    return true;
}
bool DropdownMenu::removeItem(const sf::String& label) {
    if(_item_list.removeItem(label)) {
        if(_item_list.empty())
            _selected_item_id = PLACEHOLDER_ID;
        return true;
    }
    return false;
}
bool DropdownMenu::removeItem(unsigned id) {
    if(_item_list.removeItem(id)) {
        if(_item_list.empty())
            _selected_item_id = PLACEHOLDER_ID;
        return true;
    }
    return false;
}

/* ============================================================================ *
 * Accessors - Item List
 * ============================================================================ */
const Item* DropdownMenu::getItemByLabel(const sf::String& label) {
    return _item_list.getItemByLabel(label);
}
const Item* DropdownMenu::getItemByID(unsigned id) {
    return _item_list.getItemByID(id);
}
bool DropdownMenu::empty() const {
    return _item_list.empty();
}
unsigned DropdownMenu::size() const {
    return _item_list.size();
}
DropdownMenu::iterator DropdownMenu::begin() {
    return _item_list.begin();
}
DropdownMenu::const_iterator DropdownMenu::begin() const {
    return _item_list.begin();
}
DropdownMenu::iterator DropdownMenu::end() {
    return _item_list.end();
}
DropdownMenu::const_iterator DropdownMenu::end() const {
    return _item_list.end();
}

/* ============================================================================ *
 * Mutators - Background
 * ============================================================================ */
void DropdownMenu::setBackgroundFillColor(const sf::Color& bg_fill_color) {
    _bg_shape.setFillColor(bg_fill_color);
    _item_list.setBackgroundFillColor(bg_fill_color);
}
void DropdownMenu::setBackgroundOutlineColor(const sf::Color& bg_outline_color) {
    _bg_shape.setOutlineColor(bg_outline_color);
    _item_list.setBackgroundOutlineColor(bg_outline_color);
}
void DropdownMenu::setBackgroundOutlineThickness(float bg_outline_thickness) {
    _bg_shape.setOutlineThickness(bg_outline_thickness);
    _item_list.setBackgroundOutlineThickness(bg_outline_thickness);
    _updateItemListPosition();
}

/* ============================================================================ *
 * Accessors - Background
 * ============================================================================ */
const sf::Color& DropdownMenu::getBackgroundFillColor() const {
    return _bg_shape.getFillColor();
}
const sf::Color& DropdownMenu::getBackgroundOutlineColor() const {
    return _bg_shape.getOutlineColor();
}
float DropdownMenu::getBackgroundOutlineThickness() const {
    return _bg_shape.getOutlineThickness();
}

/* ============================================================================ *
 * Mutators - Items
 * ============================================================================ */
void DropdownMenu::setItemLabelAlignment(Item::LabelAlignment align) {
    _showcased_item.setLabelAlignment(align);
    _item_list.setItemLabelAlignment(align);
}
void DropdownMenu::setItemPadding(float padding) {
    _showcased_item.setPadding(padding);
    _item_list.setItemPadding(padding);
}
void DropdownMenu::setItemTextOffset(float text_offset) {
    _showcased_item.setTextOffset(text_offset);
    _item_list.setItemTextOffset(text_offset);
}
void DropdownMenu::setItemDimensions(const sf::Vector2f& dims) {
    sf::Vector2f showcase_dims = {dims.x, getShowcasedItemHeight()};
    _showcased_item.setDimensions(showcase_dims);
    _bg_shape.setSize(showcase_dims);
    _item_list.setItemDimensions(dims);
}
void DropdownMenu::setItemFont(const sf::Font& font) {
    _showcased_item.setFont(font);
    _item_list.setItemFont(font);
}
void DropdownMenu::setItemCharacterSize(unsigned char_size) {
    _showcased_item.setCharacterSize(char_size);
    _item_list.setItemCharacterSize(char_size);
}
void DropdownMenu::setItemTextFillColor(const sf::Color& text_fill_color) {
    _showcased_item.setTextFillColor(text_fill_color);
    _item_list.setItemTextFillColor(text_fill_color);
}
void DropdownMenu::setItemTextOutlineColor(const sf::Color& text_outline_color) {
    _showcased_item.setTextOutlineColor(text_outline_color);
    _item_list.setItemTextOutlineColor(text_outline_color);
}
void DropdownMenu::setItemTextOutlineThickness(float text_outline_thickness) {
    _showcased_item.setTextOutlineThickness(text_outline_thickness);
    _item_list.setItemTextOutlineThickness(text_outline_thickness);
}
void DropdownMenu::setItemHighlightFillColor(const sf::Color& highlight_fill_color) {
    _showcased_item.setHighlightFillColor(highlight_fill_color);
    _item_list.setItemHighlightFillColor(highlight_fill_color);
}
void DropdownMenu::setItemHighlightOutlineColor(const sf::Color& highlight_outline_color) {
    _showcased_item.setHighlightOutlineColor(highlight_outline_color);
    _item_list.setItemHighlightOutlineColor(highlight_outline_color);
}
void DropdownMenu::setItemHighlightOutlineThickness(float highlight_outline_thickness) {
    _showcased_item.setHighlightOutlineThickness(highlight_outline_thickness);
    _item_list.setItemHighlightOutlineThickness(highlight_outline_thickness);
}

/* ============================================================================ *
 * Accessors - Items
 * ============================================================================ */
Item::LabelAlignment DropdownMenu::getItemLabelAlignment() const {
    return _item_list.getItemLabelAlignment();
}
float DropdownMenu::getItemPadding() const {
    return _item_list.getItemPadding();
}
float DropdownMenu::getItemTextOffset() const {
    return _item_list.getItemTextOffset();
}
const sf::Vector2f& DropdownMenu::getItemDimensions() const {
    return _item_list.getItemDimensions();
}
const sf::Font* DropdownMenu::getItemFont() const {
    return _item_list.getItemFont();
}
unsigned DropdownMenu::getItemCharacterSize() const {
    return _item_list.getItemCharacterSize();
}
const sf::Color& DropdownMenu::getItemTextFillColor() const {
    return _item_list.getItemTextFillColor();
}
const sf::Color& DropdownMenu::getItemTextOutlineColor() const {
    return _item_list.getItemTextOutlineColor();
}
float DropdownMenu::getItemTextOutlineThickness() const {
    return _item_list.getItemTextOutlineThickness();
}
const sf::Color& DropdownMenu::getItemHighlightFillColor() const {
    return _item_list.getItemHighlightFillColor();
}
const sf::Color& DropdownMenu::getItemHighlightOutlineColor() const {
    return _item_list.getItemHighlightOutlineColor();
}
float DropdownMenu::getItemHighlightOutlineThickness() const {
    return _item_list.getItemHighlightOutlineThickness();
}

/* ============================================================================ *
 * Private Functions
 * ============================================================================ */
void DropdownMenu::_updateItemListPosition() {
    float y = getShowcasedItemHeight() + getBackgroundOutlineThickness();
    _item_list.setPosition(0, y);
}