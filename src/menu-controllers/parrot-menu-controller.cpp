 /* **************************************************************************** *
 * AUTHOR:      Noah Krim
 * ASSIGNMENT:  GUI Cloth Sim
 * CLASS:       CS_08
 * ---------------------------------------------------------------------------- *
 * File: parrot-menu-controller.cpp
 *  Definition file for ParrotMenuController class
 * **************************************************************************** */

#include "parrot-menu-controller.h"
#include <iostream>

// constructors
ParrotMenuController::ParrotMenuController()
    : _labels{}
{ }

// inherited from MenuController
void ParrotMenuController::performMenuAction(unsigned menu_item_id) {
    sf::String response {"?"};
    if(_labels.count(menu_item_id))
        response = _labels.at(menu_item_id);
    std::cout<< "Parrot: \"" << response.toAnsiString() << "!\"" <<std::endl;
}

// mutators
void ParrotMenuController::insertMenuLabel(unsigned id, const sf::String& label) {
    _labels[id] = label;
}