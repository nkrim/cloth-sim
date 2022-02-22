 /* **************************************************************************** *
 * AUTHOR:      Noah Krim
 * ASSIGNMENT:  GUI Cloth Sim
 * CLASS:       CS_08
 * ---------------------------------------------------------------------------- *
 * File: menu-bar.h
 *  Header file for MenuBar class
 * **************************************************************************** */

#ifndef MENU_BAR_H
#define MENU_BAR_H

#include <SFML/Graphics.hpp>

#include <map>

#include "../component-interface/gui-component.h"
#include "menu-list.h"

class MenuBar : public GuiComponent {
public:
    // typedefs
    typedef std::map<unsigned,MenuList>::iterator iterator;
    typedef std::map<unsigned,MenuList>::const_iterator const_iterator;

private:
    // private data members
    std::map<unsigned,MenuList> _menu_lists;
    IntSnapshot _snapshot;
    bool _open_last_frame;

// functions
protected:
    // inherited from GuiComponent (sf::Drawable)
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

public:
    // constructors
    MenuBar();
    MenuBar(const std::vector<MenuList>& menu_lists);

    // destructor
    virtual ~MenuBar();

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

    // mutators - menu bar
    unsigned addMenuList(const MenuList& menu_list);
    bool addMenuList(const MenuList& menu_list, unsigned id);

    // accessors - menu bar
    bool empty() const;
    unsigned size() const;
    bool contains(unsigned id) const;
    iterator begin();
    const_iterator begin() const;
    iterator end();
    const_iterator end() const;

    // rendering helpers 
    // (made public as the menu lists are not directly mutated by this class)
    void updateMenuListPositions();
};

#endif