/* **************************************************************************** *
 * AUTHOR:      Noah Krim
 * ASSIGNMENT:  GUI Cloth Sim
 * CLASS:       CS_08
 * ---------------------------------------------------------------------------- *
 * File: item-list.h
 *  Header file for Item and ItemList classes
 * **************************************************************************** */

#ifndef ITEM_LIST_H
#define ITEM_LIST_H

#include <SFML/Graphics.hpp>

#include "../component-interface/gui-component.h"

#include <vector>

class Item : public sf::Transformable, public Clickable, public States, public sf::Drawable {
public:
    // public enums
    enum LabelAlignment {
        Left=0,
        Center,
        Right
    };

private:
    // private data members
    sf::Text _label;
    sf::RectangleShape _highlight_shape;
    float _padding;
    float _text_offset;
    LabelAlignment _align;

protected:
    // inherited from sf::Drawable
    virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;

public:
    // constructors
    Item();
    Item(const sf::String& label);
    Item(const sf::String& label, const sf::Font& font, unsigned char_size);

    // destructor
    virtual ~Item();

    // inherited from Clickable
    virtual sf::FloatRect getClickableBounds() const;
    // virtual bool click(const sf::Vector2f& click_pos);
    // virtual bool hover(const sf::Vector2f& hover_pos);

    // mutators
    void setLabel(const sf::String& label);
    void setLabelAlignment(LabelAlignment align);
    void setPadding(float padding);
    void setTextOffset(float text_offset);
    void setDimensions(const sf::Vector2f& dims);
    void setFont(const sf::Font& font);
    void setCharacterSize(unsigned char_size);
    void setTextFillColor(const sf::Color& text_fill_color);
    void setTextOutlineColor(const sf::Color& text_outline_color);
    void setTextOutlineThickness(float text_outline_thickness);
    void setHighlightFillColor(const sf::Color& highlight_fill_color);
    void setHighlightOutlineColor(const sf::Color& highlight_outline_color);
    void setHighlightOutlineThickness(float highlight_outline_thickness);

    // accessors
    float getAdvanceOffset() const;
    const sf::String& getLabel() const;
    LabelAlignment getLabelAlignment() const;
    float getPadding() const;
    float getTextOffset() const;
    const sf::Vector2f& getDimensions() const;
    const sf::Font* getFont() const;
    unsigned getCharacterSize() const;
    const sf::Color& getTextFillColor() const;
    const sf::Color& getTextOutlineColor() const;
    float getTextOutlineThickness() const;
    const sf::Color& getHighlightFillColor() const;
    const sf::Color& getHighlightOutlineColor() const;
    float getHighlightOutlineThickness() const;

private:
    // private functions
    void _updateLabelPosition();
};





class ItemList : public sf::Transformable, public Clickable, public sf::Drawable {
public:
    // typedefs
    typedef std::map<unsigned,Item>::iterator iterator;
    typedef std::map<unsigned,Item>::const_iterator const_iterator;

private:
    // private data members
    std::map<unsigned,Item> _items;
    Item _default_item;
    sf::RectangleShape _background_shape;

protected:
    // inherited from GuiComponent (sf::Drawable)
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

public:
    // constructors
    ItemList();
    ItemList(const sf::Font& font, unsigned char_size);

    // destructor
    virtual ~ItemList();

    // inherited from GuiComponent (Clickable)
    virtual sf::FloatRect getClickableBounds() const;
    virtual bool click(const sf::Vector2f& click_pos, sf::Mouse::Button mouse_button);
    virtual bool hover(const sf::Vector2f& hover_pos);
    virtual void unhover();

    // mutators - item list
    unsigned addItem(const sf::String& label);
    bool addItem(const sf::String& label, unsigned id);
    bool removeItem(const sf::String& label);
    bool removeItem(unsigned id);
    void clearItems();

    // accessors - item list
    Item* getItemByLabel(const sf::String& label);
    Item* getItemByID(unsigned id);
    const Item* getItemByLabel(const sf::String& label) const;
    const Item* getItemByID(unsigned id) const;
    bool empty() const;
    unsigned size() const;
    iterator begin();
    const_iterator begin() const;
    iterator end();
    const_iterator end() const;

    // mutators - background
    void setBackgroundFillColor(const sf::Color& bg_fill_color);
    void setBackgroundOutlineColor(const sf::Color& bg_outline_color);
    void setBackgroundOutlineThickness(float bg_outline_thickness);

    // accessors - background
    const sf::Color& getBackgroundFillColor() const;
    const sf::Color& getBackgroundOutlineColor() const;
    float getBackgroundOutlineThickness() const;

    // mutators - items
    void setItemLabelAlignment(Item::LabelAlignment align);
    void setItemPadding(float padding);
    void setItemTextOffset(float text_offset);
    void setItemDimensions(const sf::Vector2f& dims);
    void setItemFont(const sf::Font& font);
    void setItemCharacterSize(unsigned char_size);
    void setItemTextFillColor(const sf::Color& text_fill_color);
    void setItemTextOutlineColor(const sf::Color& text_outline_color);
    void setItemTextOutlineThickness(float text_outline_thickness);
    void setItemHighlightFillColor(const sf::Color& highlight_fill_color);
    void setItemHighlightOutlineColor(const sf::Color& highlight_outline_color);
    void setItemHighlightOutlineThickness(float highlight_outline_thickness);

    // accessors - items
    float getItemAdvanceOffset() const;
    Item::LabelAlignment getItemLabelAlignment() const;
    float getItemPadding() const;
    float getItemTextOffset() const;
    const sf::Vector2f& getItemDimensions() const;
    const sf::Font* getItemFont() const;
    unsigned getItemCharacterSize() const;
    const sf::Color& getItemTextFillColor() const;
    const sf::Color& getItemTextOutlineColor() const;
    float getItemTextOutlineThickness() const;
    const sf::Color& getItemHighlightFillColor() const;
    const sf::Color& getItemHighlightOutlineColor() const;
    float getItemHighlightOutlineThickness() const;

private:
    // private functions
    void _updateSize();
};

#endif