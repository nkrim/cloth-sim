 /* **************************************************************************** *
 * AUTHOR:      Noah Krim
 * ASSIGNMENT:  GUI Cloth Sim
 * CLASS:       CS_08
 * ---------------------------------------------------------------------------- *
 * File: menu-bar.cpp
 *  Definition file for MenuBar class
 * **************************************************************************** */

#include "menu-bar.h"
#include <iostream>

/* ============================================================================ *
 * Constructors
 * ============================================================================ */
MenuBar::MenuBar()
    : _menu_lists{}
    , _snapshot{*this}
    , _open_last_frame{false}
{ }
MenuBar::MenuBar(const std::vector<MenuList>& menu_lists) 
    :MenuBar{}
{
    for(auto it=menu_lists.begin(); it!=menu_lists.end(); ++it) {
        addMenuList(*it);
    }
}

/* ============================================================================ *
 * Destructor
 * ============================================================================ */
MenuBar::~MenuBar()
{ }

/* ============================================================================ *
 * Inherited from GuiComponent (sf::Drawable)
 * ============================================================================ */
void MenuBar::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    const MenuList* draw_last = nullptr;
    for(auto it=_menu_lists.begin(); it!=_menu_lists.end(); ++it) {
        if(it->second.getState(States::Focused)) {
            if(draw_last != nullptr)
                target.draw(*draw_last, states);
            draw_last = &(it->second);
        }
        else {
            target.draw(it->second, states);
            // std::cout<<it->first<<std::endl;
        }
    }
    if(draw_last != nullptr)
        target.draw(*draw_last, states);
}

/* ============================================================================ *
 * Inhereted from GuiComponent (EventHandler)
 * ============================================================================ */
bool MenuBar::addEventHandler(sf::RenderWindow& window, const sf::Event& event) {
    return false;
}
void MenuBar::update(float t) {
    if(_open_last_frame && !getState(States::Focused)) {
        for(auto it=_menu_lists.begin(); it!=_menu_lists.end(); ++it)
            it->second.setStates(States::Hovered|States::Focused, false);
    }
    _open_last_frame = getState(States::Focused);

    // update menu lists
    for(auto it=_menu_lists.begin(); it!=_menu_lists.end(); ++it)
        it->second.update(t);
}

/* ============================================================================ *
 * Inherited from GuiComponent (Snapshotable)
 * ============================================================================ */
const Snapshot& MenuBar::getSnapshot() const {
    return _snapshot;
}
void MenuBar::applySnapshot(Snapshot& snapshot) {
    /* do nothing */
}
void MenuBar::revertSnapshot(Snapshot& snapshot) {
    /* do nothing */
}

/* ============================================================================ *
 * Inherited from GuiComponent (Clickable)
 * ============================================================================ */
sf::FloatRect MenuBar::getClickableBounds() const {
    sf::FloatRect out ({0,0}, {0,0});
    if(_menu_lists.empty())
        return out;
    for(auto it=_menu_lists.begin(); it!=_menu_lists.end(); ++it) {
        sf::FloatRect it_rect = it->second.getClickableBounds();
        out.height = std::max(out.height, it_rect.top + it_rect.height);
    }
    sf::FloatRect last_rect = _menu_lists.rbegin()->second.getClickableBounds();
    out.width = last_rect.left + last_rect.width; 
    return getTransform().transformRect(out);
}
bool MenuBar::containsMousePosition(const sf::Vector2f& mouse_pos) const {
    sf::Vector2f local_mouse_pos = getInverseTransform().transformPoint(mouse_pos);
    for(auto it=_menu_lists.begin(); it!=_menu_lists.end(); ++it) {
        if(it->second.containsMousePosition(local_mouse_pos))
            return true;
    }
    return false;
}
bool MenuBar::click(const sf::Vector2f& click_pos, sf::Mouse::Button mouse_button) {
    if(!getState(States::Focused) || mouse_button != sf::Mouse::Left)
        return false;
    sf::Vector2f local_click_pos = getInverseTransform().transformPoint(click_pos);
    bool any_clicked = false;
    for(auto it=_menu_lists.begin(); it!=_menu_lists.end(); ++it) {
        if(!any_clicked && it->second.containsMousePosition(local_click_pos)) {
            any_clicked = true;
            it->second.setState(States::Focused, true);
            it->second.click(local_click_pos, mouse_button);
        }
        else
            it->second.setState(States::Focused, false);
    }
    if(_open_last_frame || !any_clicked)
        setState(States::Focused, false);
    return false;
}
bool MenuBar::hover(const sf::Vector2f& hover_pos) {
    sf::Vector2f local_hover_pos = getInverseTransform().transformPoint(hover_pos);
    bool any_hovered = false;
    for(auto it=_menu_lists.begin(); it!=_menu_lists.end(); ++it) {
        if(it->second.containsMousePosition(local_hover_pos)) {
            any_hovered = true;
            it->second.setStates(States::Hovered, true);
            if(getState(States::Focused))
                it->second.setStates(States::Focused, true);
            it->second.hover(local_hover_pos);
        }
        else {
            it->second.setStates(States::Hovered|States::Focused, false);
            it->second.unhover();
        }
    }
    return any_hovered;
}
void MenuBar::unhover() {
    for(auto it=_menu_lists.begin(); it!=_menu_lists.end(); ++it) {
        it->second.setState(States::Hovered, false);
        it->second.unhover();
    }
}

/* ============================================================================ *
 * Mutators - Menu Bar
 * ============================================================================ */
unsigned MenuBar::addMenuList(const MenuList& menu_list) {
    unsigned to_add_id = 0;
    if(!_menu_lists.empty())
        to_add_id = _menu_lists.rbegin()->first + 1;
    _menu_lists[to_add_id] = menu_list;
    updateMenuListPositions();
    return to_add_id;
}
bool MenuBar::addMenuList(const MenuList& menu_list, unsigned id) {
    if(_menu_lists.count(id))
        return false;
    _menu_lists[id] = menu_list;
    updateMenuListPositions();
    return true;
}

/* ============================================================================ *
 * Accessors - Menu Bar
 * ============================================================================ */
bool MenuBar::empty() const {
    return _menu_lists.empty();
}
unsigned MenuBar::size() const {
    return _menu_lists.size();
}
bool MenuBar::contains(unsigned id) const {
    return _menu_lists.count(id);
}
MenuBar::iterator MenuBar::begin() {
    return _menu_lists.begin();
}
MenuBar::const_iterator MenuBar::begin() const {
    return _menu_lists.cbegin();
}
MenuBar::iterator MenuBar::end() {
    return _menu_lists.end();
}
MenuBar::const_iterator MenuBar::end() const {
    return _menu_lists.cend();
}

/* ============================================================================ *
 * Rendering Helpers
 *  (made public as the menu lists are not directly mutated by this class)
 * ============================================================================ */
void MenuBar::updateMenuListPositions() {
    sf::Vector2f pos = {0,0};
    for(auto it=_menu_lists.begin(); it!=_menu_lists.end(); ++it) {
        it->second.setPosition(pos);
        pos.x += it->second.getMenuHeaderDimensions().x + it->second.getBackgroundOutlineThickness();
    }
}
