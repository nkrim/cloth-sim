 /* **************************************************************************** *
 * AUTHOR:      Noah Krim
 * ASSIGNMENT:  GUI Cloth Sim
 * CLASS:       CS_08
 * ---------------------------------------------------------------------------- *
 * File: bitmask-menu-controller.h
 *  Header file for BitmaskMenuController class
 * **************************************************************************** */

#ifndef BITMASK_MENU_CONTROLLER_H
#define BITMASK_MENU_CONTROLLER_H

#include <SFML/Graphics.hpp>

#include "../gui/menu/menu-controller.h"

class BitmaskMenuController : public MenuController {
private:
    // private data members
    unsigned _bitmask;

public:
    // constructors
    BitmaskMenuController();

    // inherited from MenuController
    virtual void performMenuAction(unsigned menu_item_id);

    // accessors
    bool wasActivated(unsigned menu_item_id) const;

    // mutators
    void clearBitmask();
};

#endif