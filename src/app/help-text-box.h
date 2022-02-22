/* **************************************************************************** *
 * AUTHOR:      Noah Krim
 * ASSIGNMENT:  GUI Cloth Sim
 * CLASS:       CS_08
 * ---------------------------------------------------------------------------- *
 * File: help-text-box.h
 *  Header file for HelpTextBox class
 * **************************************************************************** */

#ifndef HELP_TEXT_BOX_H
#define HELP_TEXT_BOX_H

#include <SFML/Graphics.hpp>

#include "../gui/component-interface/gui-component.h"
#include "../snapshots/int-snapshot.h"

class HelpTextBox : public GuiComponent {
private:
    sf::Text _col_1;
    sf::Text _col_2;
    sf::RectangleShape _rect;
    sf::Vector2f _text_offset;
    IntSnapshot _snapshot;

protected:
    // inherited from GuiComponent (sf::Drawable)
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

public:
    // constructors
    HelpTextBox();
    HelpTextBox(const sf::Font& font, unsigned char_size);

    // destructor
    virtual ~HelpTextBox();

    // inherited from GuiComponent (EventHandler)
    bool addEventHandler(sf::RenderWindow& window, const sf::Event& event);
    void update(float t=0);

    // inherited from GuiComponent (Snapshotable)
    virtual const Snapshot& getSnapshot() const;
    virtual void applySnapshot(Snapshot& snapshot);
    virtual void revertSnapshot(Snapshot& snapshot);

    // inherited from GuiComponent (Clickable)
    virtual sf::FloatRect getClickableBounds() const;

    // mutators
    void setFont(const sf::Font& font);
    void setCharSize(unsigned char_size);
    void setSize(const sf::Vector2f& size);
    void setText(const sf::String& col_1, const sf::String& col_2);
    void setTextFillColor(const sf::Color& text_color);
    void setFillColor(const sf::Color& fill_color);
    void setOutlineColor(const sf::Color& outline_color);
    void setOutlineThickness(float outline_thickness);
    void setTextOffset(const sf::Vector2f& text_offset);
};

#endif