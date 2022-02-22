/* **************************************************************************** *
 * AUTHOR:      Noah Krim
 * ASSIGNMENT:  SFML Dropdown
 * CLASS:       CS_08
 * ---------------------------------------------------------------------------- *
 * File: dropdown-menu.h
 *  Header file for DropdownMenu class
 * **************************************************************************** */

#ifndef DROPDOWN_MENU_H 
#define DROPDOWN_MENU_H

#include <SFML/Graphics.hpp>

#include "../component-interface/gui-component.h"
#include "../item-list/item-list.h"
#include "../../snapshots/int-snapshot.h"

#include <vector>

class DropdownMenu : public GuiComponent {
public:
    // typedefs
    typedef ItemList::iterator iterator;
    typedef ItemList::const_iterator const_iterator;
    // constants
    static const unsigned PLACEHOLDER_ID = ~0U;

private:
    // private data members
    ItemList _item_list;
    sf::String _placeholder_label;
    Item _showcased_item;
    unsigned _selected_item_id;
    sf::RectangleShape _bg_shape;
    IntSnapshot _snapshot;
    bool _placeholder_enabled;
    bool _open_last_frame;

// functions
protected:
    // inherited from GuiComponent (sf::Drawable)
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

public:
    // constructors
    DropdownMenu();
    DropdownMenu(const std::vector<sf::String>& items,
                 const sf::Font& font,
                 unsigned char_size);
    DropdownMenu(const std::vector<sf::String>& items,
                 const sf::Font& font,
                 unsigned char_size,
                 const sf::String& placeholder);

    // destructor
    virtual ~DropdownMenu();

    // inhereted from GuiComponent (EventHandler)
    virtual bool addEventHandler(sf::RenderWindow& window, const sf::Event& event);
    virtual void update(float t=0);

    // inherited from GuiComponent (Snapshotable)
    virtual const Snapshot& getSnapshot() const;
    virtual void applySnapshot(Snapshot& snapshot);
    virtual void revertSnapshot(Snapshot& snapshot);

    // inherited from GuiComponent (Clickable)
    virtual sf::FloatRect getClickableBounds() const;
    virtual bool click(const sf::Vector2f& click_pos, sf::Mouse::Button mouse_button);
    virtual bool hover(const sf::Vector2f& hover_pos);
    virtual void unhover();

    // mutators - dropdown menu
    bool setSelectedItem(unsigned id);
    bool setSelectedItem(const sf::String& label);
    void setShowcasedItemHeight(float height);
    void setPlaceholderLabel(const sf::String& placeholder);
    void setPlaceholderEnabled(bool enabled);

    // accessors - dropdown menu
    bool hasSelection() const;
    unsigned getSelectedItemID() const;
    const sf::String& getSelectedItemLabel() const;
    float getShowcasedItemHeight() const;
    const sf::String& getPlaceholderLabel() const;
    bool getPlaceholderEnabled() const;

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
    void _updateItemListPosition();
};

#endif