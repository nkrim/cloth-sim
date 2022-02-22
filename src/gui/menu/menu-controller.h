 /* **************************************************************************** *
 * AUTHOR:      Noah Krim
 * ASSIGNMENT:  GUI Cloth Sim
 * CLASS:       CS_08
 * ---------------------------------------------------------------------------- *
 * File: menu-controller.h
 *  Header file for abstract MenuController class
 * **************************************************************************** */

#ifndef MENU_CONTROLLER_H
#define MENU_CONTROLLER_H

class MenuController {
public:
    virtual void performMenuAction(unsigned menu_item_id) = 0;
};

#endif