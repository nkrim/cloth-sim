 /* **************************************************************************** *
 * AUTHOR:      Noah Krim
 * ASSIGNMENT:  GUI Cloth Sim
 * CLASS:       CS_08
 * ---------------------------------------------------------------------------- *
 * File: parrot-menu-controller.h
 *  Header file for ParrotMenuController class
 * **************************************************************************** */

#ifndef PARROT_MENU_CONTROLLER_H
#define PARROT_MENU_CONTROLLER_H

#include <SFML/Graphics.hpp>

#include "../gui/menu/menu-controller.h"

class ParrotMenuController : public MenuController {
private:
    // private data members
    std::map<unsigned,sf::String> _labels;

public:
    // constructors
    ParrotMenuController();

    // inherited from MenuController
    virtual void performMenuAction(unsigned menu_item_id);

    // mutators
    void insertMenuLabel(unsigned id, const sf::String& label);
};

#endif