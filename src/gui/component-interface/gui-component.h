/* **************************************************************************** *
 * AUTHOR:      Noah Krim
 * ASSIGNMENT:  GUI Cloth Sim
 * CLASS:       CS_08
 * ---------------------------------------------------------------------------- *
 * File: gui-component.h
 *  Header file for abstract GuiComponent class
 * **************************************************************************** */

#ifndef GUI_COMPONENT_H
#define GUI_COMPONENT_H

#include <SFML/Graphics.hpp>

#include "event-handler.h"
#include "states.h"
#include "clickable.h"
#include "snapshot-interface.h"

class GuiComponent : public sf::Transformable, public EventHandler, public States, public Clickable, public Snapshotable, public sf::Drawable {
public:
	// virtual destructor
	virtual ~GuiComponent() { }

	// // - inherited from sf::Drawable -
	// virtual void draw(sf::RenderTarget& window, sf::RenderStates states) const = 0;

	// // - inherited from EventHandler -
	// virtual void addEventHandler(sf::RenderWindow& window, const sf::Event& event) = 0;
	// virtual void update(float t=0) = 0;

	// // - inherited from Snapshotable -
	// virtual const Snapshot& getSnapshot() const = 0;
	// virtual void applySnapshot(const Snapshot& snapshot) = 0;
	// virtual void revertSnapshot(const Snapshot& snapshot) = 0;

	// // - inherited form Clickable -
	// virtual sf::FloatRect getClickableBounds() const = 0;
    // // optional virtual functions
    // virtual bool click(const sf::Vector2f& click_pos);
    // virtual bool hover(const sf::Vector2f& hover_pos);
};

#endif