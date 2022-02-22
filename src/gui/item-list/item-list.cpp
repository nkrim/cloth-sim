/* **************************************************************************** *
 * AUTHOR:      Noah Krim
 * ASSIGNMENT:  SFML Dropdown
 * CLASS:       CS_08
 * ---------------------------------------------------------------------------- *
 * File: item-list.cpp
 *  Definition file for Item and ItemList classes
 * **************************************************************************** */

#include "item-list.h"

/* ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ *
 *
 * Item Class Definition
 *
 * ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ */

/* ============================================================================ *
 * Contructors
 * ============================================================================ */
Item::Item()
    : _label{}
    , _highlight_shape{}
    , _padding{0}
    , _text_offset{0}
    , _align{Item::Center}
{ }
Item::Item(const sf::String& label)
    :Item{}
{ 
    _label.setString(label);
}
Item::Item(const sf::String& label, const sf::Font& font, unsigned char_size)
    : _label{label, font, char_size}
    , _highlight_shape{}
    , _padding{}
    , _text_offset{0}
    , _align{Item::Center}
{ 
    _updateLabelPosition();
}

/* ============================================================================ *
 * Destructor
 * ============================================================================ */
Item::~Item()
{ }

/* ============================================================================ *
 * Inherited from Drawable
 * ============================================================================ */
void Item::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    // apply transform
    states.transform *= getTransform();
    if(anyStates(States::Hovered|States::Selected))
        target.draw(_highlight_shape, states);
    target.draw(_label, states);
}

/* ============================================================================ *
 * Inherited from Clickable
 * ============================================================================ */
sf::FloatRect Item::getClickableBounds() const {
    return getTransform().transformRect(sf::FloatRect{{0,0}, {_highlight_shape.getSize().x, getAdvanceOffset()}});
}

/* ============================================================================ *
 * Mutators
 * ============================================================================ */
void Item::setLabel(const sf::String& label) {
    _label.setString(label);
    _updateLabelPosition();
}
void Item::setLabelAlignment(Item::LabelAlignment align) {
    _align = align;
    _updateLabelPosition();
}
void Item::setPadding(float padding) {
    _padding = padding;
    _updateLabelPosition();
}
void Item::setTextOffset(float text_offset) {
    _text_offset = text_offset;
    _updateLabelPosition();
}
void Item::setDimensions(const sf::Vector2f& dims) {
    _highlight_shape.setSize(dims);
    _updateLabelPosition();
}
void Item::setFont(const sf::Font& font) {
    _label.setFont(font);
    _updateLabelPosition();
}
void Item::setCharacterSize(unsigned char_size) {
    _label.setCharacterSize(char_size);
    _updateLabelPosition();
}
void Item::setTextFillColor(const sf::Color& text_fill_color) {
    _label.setFillColor(text_fill_color);
}
void Item::setTextOutlineColor(const sf::Color& text_outline_color) {
    _label.setOutlineColor(text_outline_color);
}
void Item::setTextOutlineThickness(float text_outline_thickness) {
    _label.setOutlineThickness(text_outline_thickness);
    _updateLabelPosition();
}
void Item::setHighlightFillColor(const sf::Color& highlight_fill_color) {
    _highlight_shape.setFillColor(highlight_fill_color);
}
void Item::setHighlightOutlineColor(const sf::Color& highlight_outline_color) {
    _highlight_shape.setOutlineColor(highlight_outline_color);
}
void Item::setHighlightOutlineThickness(float highlight_outline_thickness) {
    _highlight_shape.setOutlineThickness(highlight_outline_thickness);
}

/* ============================================================================ *
 * Accessors
 * ============================================================================ */
float Item::getAdvanceOffset() const {
    return _highlight_shape.getSize().y + _highlight_shape.getOutlineThickness();
}
const sf::String& Item::getLabel() const {
    return _label.getString();
}
Item::LabelAlignment Item::getLabelAlignment() const {
    return _align;
}
float Item::getPadding() const {
    return _padding;
}
float Item::getTextOffset() const {
    return _text_offset;
}
const sf::Vector2f& Item::getDimensions() const {
    return _highlight_shape.getSize();
}
const sf::Font* Item::getFont() const {
    return _label.getFont();
}
unsigned Item::getCharacterSize() const {
    return _label.getCharacterSize();
}
const sf::Color& Item::getTextFillColor() const {
    return _label.getFillColor();
}
const sf::Color& Item::getTextOutlineColor() const {
    return _label.getOutlineColor();
}
float Item::getTextOutlineThickness() const {
    return _label.getOutlineThickness();
}
const sf::Color& Item::getHighlightFillColor() const {
    return _highlight_shape.getFillColor();
}
const sf::Color& Item::getHighlightOutlineColor() const {
    return _highlight_shape.getOutlineColor();
}
float Item::getHighlightOutlineThickness() const {
    return _highlight_shape.getOutlineThickness();
}

/* ============================================================================ *
 * Private Functions
 * ============================================================================ */
void Item::_updateLabelPosition() {
    sf::FloatRect label_bounds = _label.getLocalBounds();
    sf::Vector2f dimensions = _highlight_shape.getSize();
    sf::Vector2f position = {0,0};
    position.y = (dimensions.y - _label.getCharacterSize())/2 + _text_offset;
    switch(_align) {
        case Item::Left:
            position.x = _padding;
            break;
        case Item::Center:
            position.x = (dimensions.x - label_bounds.width)/2;
            break;
        case Item::Right:
            position.x = dimensions.x - _padding - label_bounds.width;
            break;
    }
    _label.setPosition(position);
}






/* ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ *
 *
 * ItemList Class Definition
 *
 * ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ */

/* ============================================================================ *
 * Contructors
 * ============================================================================ */
ItemList::ItemList()
    : _items{}
    , _default_item{}
    , _background_shape{}
{ }

ItemList::ItemList(const sf::Font& font, unsigned char_size)
    : _items{}
    , _default_item{sf::String(), font, char_size}
    , _background_shape{}
{ }

/* ============================================================================ *
 * Destructor
 * ============================================================================ */
ItemList::~ItemList()
{ }

/* ============================================================================ *
 * Inherited from GuiComponent (sf::Drawable)
 * ============================================================================ */
void ItemList::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(_background_shape, states);

    sf::Transform states_transform_base = states.transform;
    sf::Transform item_y_translate;
    for(auto it=_items.begin(); it!=_items.end(); ++it) {
        states.transform = states_transform_base * item_y_translate;
        target.draw(it->second, states);
        item_y_translate.translate(0, getItemAdvanceOffset());
    }
}

/* ============================================================================ *
 * Inherited from GuiComponent (Clickable)
 * ============================================================================ */
sf::FloatRect ItemList::getClickableBounds() const {
    return getTransform().transformRect(sf::FloatRect({0,0}, _background_shape.getSize()));
}
bool ItemList::click(const sf::Vector2f& click_pos, sf::Mouse::Button mouse_button) {
    return false;
}
bool ItemList::hover(const sf::Vector2f& hover_pos) {
    sf::Vector2f local_hover_pos = getInverseTransform().transformPoint(hover_pos);
    float advance_offset = getItemAdvanceOffset();
    bool any_hovering = false;
    for(auto it=_items.begin(); it!=_items.end(); ++it) {
        if(!any_hovering && it->second.containsMousePosition(local_hover_pos)) {
            any_hovering = true;
            it->second.setState(States::Hovered, true);
        }
        else {
            it->second.setState(States::Hovered, false);
        }
        local_hover_pos.y -= advance_offset;
    }
    return any_hovering;
}
void ItemList::unhover() {
    for(auto it=_items.begin(); it!=_items.end(); ++it) {
        it->second.setState(States::Hovered, false);
    }
}

/* ============================================================================ *
 * Mutators - Item List
 * ============================================================================ */
unsigned ItemList::addItem(const sf::String& label) {
    Item to_add = _default_item;
    to_add.setLabel(label);
    unsigned to_add_id = 0;
    if(!_items.empty())
        to_add_id = _items.rbegin()->first + 1;
    _items[to_add_id] = to_add;
    _updateSize();
    return to_add_id;
}
bool ItemList::addItem(const sf::String& label, unsigned id) {
    if(_items.count(id))
        return false;
    Item to_add = _default_item;
    to_add.setLabel(label);
    _items[id] = to_add;
    _updateSize();
    return true;
}
bool ItemList::removeItem(const sf::String& label) {
    for(auto it=_items.begin(); it!=_items.end(); ++it) {
        if(it->second.getLabel() == label) {
            _items.erase(it);
            _updateSize();
            return true;
        }
    }
    return false;
}
bool ItemList::removeItem(unsigned id) {
    if(_items.erase(id)) {
        _updateSize();
        return true;
    }
    return false;
}
void ItemList::clearItems() {
    _items.clear();
}

/* ============================================================================ *
 * Accessors - Item List
 * ============================================================================ */
Item* ItemList::getItemByLabel(const sf::String& label) {
    for(auto it=_items.begin(); it!=_items.end(); ++it) {
        if(it->second.getLabel() == label) {
            return &(it->second);
        }
    }
    return nullptr;
}
Item* ItemList::getItemByID(unsigned id) {
    if(!_items.count(id))
        return nullptr;
    return &(_items[id]);
}
const Item* ItemList::getItemByLabel(const sf::String& label) const {
    for(auto it=_items.begin(); it!=_items.end(); ++it) {
        if(it->second.getLabel() == label) {
            return &(it->second);
        }
    }
    return nullptr;
}
const Item* ItemList::getItemByID(unsigned id) const {
    if(!_items.count(id))
        return nullptr;
    return &(_items.at(id));
}
bool ItemList::empty() const {
    return _items.empty();
}
unsigned ItemList::size() const {
    return _items.size();
}
ItemList::iterator ItemList::begin() {
    return _items.begin();
}
ItemList::const_iterator ItemList::begin() const {
    return _items.cbegin();
}
ItemList::iterator ItemList::end() {
    return _items.end();
}
ItemList::const_iterator ItemList::end() const {
    return _items.cend();
}

/* ============================================================================ *
 * Mutators - Background
 * ============================================================================ */
void ItemList::setBackgroundFillColor(const sf::Color& bg_fill_color) {
    _background_shape.setFillColor(bg_fill_color);
}
void ItemList::setBackgroundOutlineColor(const sf::Color& bg_outline_color) {
    _background_shape.setOutlineColor(bg_outline_color);
}
void ItemList::setBackgroundOutlineThickness(float bg_outline_thickness) {
    _background_shape.setOutlineThickness(bg_outline_thickness);
}

/* ============================================================================ *
 * Accessors - Background
 * ============================================================================ */
const sf::Color& ItemList::getBackgroundFillColor() const {
    return _background_shape.getFillColor();
}
const sf::Color& ItemList::getBackgroundOutlineColor() const {
    return _background_shape.getOutlineColor();
}
float ItemList::getBackgroundOutlineThickness() const {
    return _background_shape.getOutlineThickness();
}

/* ============================================================================ *
 * Mutators - Items
 * ============================================================================ */
void ItemList::setItemLabelAlignment(Item::LabelAlignment align) {
    _default_item.setLabelAlignment(align);
    for(auto it=_items.begin(); it!=_items.end(); ++it) {
        it->second.setLabelAlignment(align);
    }
} 
void ItemList::setItemPadding(float padding) {
    _default_item.setPadding(padding);
    for(auto it=_items.begin(); it!=_items.end(); ++it) {
        it->second.setPadding(padding);
    }
}
void ItemList::setItemTextOffset(float text_offset) {
    _default_item.setTextOffset(text_offset);
    for(auto it=_items.begin(); it!=_items.end(); ++it) {
        it->second.setTextOffset(text_offset);
    }
}
void ItemList::setItemDimensions(const sf::Vector2f& dims) {
    _default_item.setDimensions(dims);
    for(auto it=_items.begin(); it!=_items.end(); ++it) {
        it->second.setDimensions(dims);
    }
    _updateSize();
}
void ItemList::setItemFont(const sf::Font& font) {
    _default_item.setFont(font);
    for(auto it=_items.begin(); it!=_items.end(); ++it) {
        it->second.setFont(font);
    }
}
void ItemList::setItemCharacterSize(unsigned char_size) {
    _default_item.setCharacterSize(char_size);
    for(auto it=_items.begin(); it!=_items.end(); ++it) {
        it->second.setCharacterSize(char_size);
    }
}
void ItemList::setItemTextFillColor(const sf::Color& text_fill_color) {
    _default_item.setTextFillColor(text_fill_color);
    for(auto it=_items.begin(); it!=_items.end(); ++it) {
        it->second.setTextFillColor(text_fill_color);
    }
}
void ItemList::setItemTextOutlineColor(const sf::Color& text_outline_color) {
    _default_item.setTextOutlineColor(text_outline_color);
    for(auto it=_items.begin(); it!=_items.end(); ++it) {
        it->second.setTextOutlineColor(text_outline_color);
    }
}
void ItemList::setItemTextOutlineThickness(float text_outline_thickness) {
    _default_item.setTextOutlineThickness(text_outline_thickness);
    for(auto it=_items.begin(); it!=_items.end(); ++it) {
        it->second.setTextOutlineThickness(text_outline_thickness);
    }
    _updateSize();
}
void ItemList::setItemHighlightFillColor(const sf::Color& highlight_fill_color) {
    _default_item.setHighlightFillColor(highlight_fill_color);
    for(auto it=_items.begin(); it!=_items.end(); ++it) {
        it->second.setHighlightFillColor(highlight_fill_color);
    }
}
void ItemList::setItemHighlightOutlineColor(const sf::Color& highlight_outline_color) {
    _default_item.setHighlightOutlineColor(highlight_outline_color);
    for(auto it=_items.begin(); it!=_items.end(); ++it) {
        it->second.setHighlightOutlineColor(highlight_outline_color);
    }
}
void ItemList::setItemHighlightOutlineThickness(float highlight_outline_thickness) {
    _default_item.setHighlightOutlineThickness(highlight_outline_thickness);
    for(auto it=_items.begin(); it!=_items.end(); ++it) {
        it->second.setHighlightOutlineThickness(highlight_outline_thickness);
    }
}

/* ============================================================================ *
 * Accessors - Items
 * ============================================================================ */
float ItemList::getItemAdvanceOffset() const {
    return _default_item.getAdvanceOffset();
}
Item::LabelAlignment ItemList::getItemLabelAlignment() const {
    return _default_item.getLabelAlignment();
}
float ItemList::getItemPadding() const {
    return _default_item.getPadding();
}
float ItemList::getItemTextOffset() const {
    return _default_item.getTextOffset();
}
const sf::Vector2f& ItemList::getItemDimensions() const {
    return _default_item.getDimensions();
}
const sf::Font* ItemList::getItemFont() const {
    return _default_item.getFont();
}
unsigned ItemList::getItemCharacterSize() const {
    return _default_item.getCharacterSize();
}
const sf::Color& ItemList::getItemTextFillColor() const {
    return _default_item.getTextFillColor();
}
const sf::Color& ItemList::getItemTextOutlineColor() const {
    return _default_item.getTextOutlineColor();
}
float ItemList::getItemTextOutlineThickness() const {
    return _default_item.getTextOutlineThickness();
}
const sf::Color& ItemList::getItemHighlightFillColor() const {
    return _default_item.getHighlightFillColor();
}
const sf::Color& ItemList::getItemHighlightOutlineColor() const {
    return _default_item.getHighlightOutlineColor();
}
float ItemList::getItemHighlightOutlineThickness() const {
    return _default_item.getHighlightOutlineThickness();
}

/* ============================================================================ *
 * Private Functions
 * ============================================================================ */
void ItemList::_updateSize() {
    sf::Vector2f dims = {_default_item.getDimensions().x, getItemAdvanceOffset()};
    dims.y *= _items.size();
    dims.y -= _default_item.getHighlightOutlineThickness();
    _background_shape.setSize(dims);
}