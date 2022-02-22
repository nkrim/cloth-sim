/* **************************************************************************** *
 * AUTHOR:      Noah Krim
 * ASSIGNMENT:  GUI Cloth Sim
 * CLASS:       CS_08
 * ---------------------------------------------------------------------------- *
 * File: event-handler.h
 *  Header file for EventHandler class
 * **************************************************************************** */

#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

class EventHandler {
public:
    virtual bool addEventHandler(sf::RenderWindow& window, const sf::Event& event) = 0;
    virtual void update(float t=0) = 0;
};

#endif