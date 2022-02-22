 /* **************************************************************************** *
 * AUTHOR:      Noah Krim
 * ASSIGNMENT:  GUI Cloth Sim
 * CLASS:       CS_08
 * ---------------------------------------------------------------------------- *
 * File: bitmask-menu-controller.cpp
 *  Definition file for BitmaskMenuController class
 * **************************************************************************** */

#include "bitmask-menu-controller.h"

// constructors
BitmaskMenuController::BitmaskMenuController()
    : _bitmask(0)
{ }

// inherited from MenuController
void BitmaskMenuController::performMenuAction(unsigned menu_item_id) {
    _bitmask |= 1<<menu_item_id;
}

// accessors
bool BitmaskMenuController::wasActivated(unsigned menu_item_id) const {
    return _bitmask & (1<<menu_item_id);
}

// mutators
void BitmaskMenuController::clearBitmask() {
    _bitmask ^= _bitmask;
}