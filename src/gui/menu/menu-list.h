 /* **************************************************************************** *
 * AUTHOR:      Noah Krim
 * ASSIGNMENT:  GUI Cloth Sim
 * CLASS:       CS_08
 * ---------------------------------------------------------------------------- *
 * File: menu-list.h
 *  Header file for MenuList class
 * **************************************************************************** */

#ifndef MENU_LIST_H
#define MENU_LIST_H

#include <SFML/Graphics.hpp>

#include "../component-interface/gui-component.h"
#include "menu-controller.h"
#include "../item-list/item-list.h"
#include "../../snapshots/int-snapshot.h"

class MenuList : public GuiComponent {
public:
    // typedefs
    typedef ItemList::iterator iterator;
    typedef ItemList::const_iterator const_iterator;

private:
    // private data members
    Item _menu_header;
    ItemList _menu_items;
    sf::RectangleShape _bg_shape;
    IntSnapshot _snapshot;
    MenuController* _controller;
    bool _open_last_frame;

// functions
protected:
    // inherited from GuiComponent (sf::Drawable)
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

public:
    // constructors
    MenuList();
    MenuList(const sf::String& menu_label,
             const sf::Font& font,
             unsigned char_size);
    MenuList(const sf::String& menu_label,
             const std::vector<sf::String>& menu_items,
             MenuController* controller,
             const sf::Font& font,
             unsigned char_size);

    // destructor
    virtual ~MenuList();

    // inhereted from GuiComponent (EventHandler)
    virtual bool addEventHandler(sf::RenderWindow& window, const sf::Event& event);
    virtual void update(float t=0);

    // inherited from GuiComponent (Snapshotable)
    virtual const Snapshot& getSnapshot() const;
    virtual void applySnapshot(Snapshot& snapshot);
    virtual void revertSnapshot(Snapshot& snapshot);

    // inherited from GuiComponent (Clickable)
    virtual sf::FloatRect getClickableBounds() const;
    virtual bool containsMousePosition(const sf::Vector2f& mouse_pos) const;
    virtual bool click(const sf::Vector2f& click_pos, sf::Mouse::Button mouse_button);
    virtual bool hover(const sf::Vector2f& hover_pos);
    virtual void unhover();

    // mutators - menu list
    bool selectMenuItem(unsigned id);
    void setMenuLabel(const sf::String& menu_label);
    void setMenuHeaderDimensions(const sf::Vector2f& dims);
    void setMenuController(MenuController* controller);

    // accessors - menu list
    const sf::String& getMenuLabel() const;
    const sf::Vector2f& getMenuHeaderDimensions() const;
    const MenuController* getMenuController() const;

    // mutators - item list
    unsigned addItem(const sf::String& label);
    bool addItem(const sf::String& label, unsigned id);
    bool removeItem(const sf::String& label);
    bool removeItem(unsigned id);

    // accessors - item list
    const Item* getItemByLabel(const sf::String& label);
    const Item* getItemByID(unsigned id);
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
    void _updateMenuItemsPosition();
};

#endif