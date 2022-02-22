/* **************************************************************************** *
 * AUTHOR:      Noah Krim
 * ASSIGNMENT:  GUI Cloth Sim
 * CLASS:       CS_08
 * ---------------------------------------------------------------------------- *
 * File: history.h
 *  Header file for History class
 * **************************************************************************** */

#ifndef HISTORY_H
#define HISTORY_H

#include <SFML/Graphics.hpp>

#include "../data-structures/linked-list.h"

#include "../gui/component-interface/gui-component.h"
#include "../gui/component-interface/snapshot-interface.h"

class History : public EventHandler {
private:
    LinkedList<Snapshot*> _stack;
    LinkedList<Snapshot*>::iterator _stack_top;

public:
    // constructors
    History();

    // destructor
    virtual ~History();

    // inherited from EventHandler
    virtual bool addEventHandler(sf::RenderWindow& window, const sf::Event& event);
    virtual void update(float t=0);

    // history endpoints
    unsigned size();
    void pushSnapshot(const Snapshotable& component);
    bool undo();
    bool redo();
    Snapshot* nextUndo();
    Snapshot* nextRedo();
    void clear();
    void clearRedos();
};

#endif 