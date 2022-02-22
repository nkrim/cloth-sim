/* **************************************************************************** *
 * AUTHOR:      Noah Krim
 * ASSIGNMENT:  GUI Cloth Sim
 * CLASS:       CS_08
 * ---------------------------------------------------------------------------- *
 * File: button.h
 *  Header file for Button class
 * **************************************************************************** */

#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>

#include "../component-interface/gui-component.h"
#include "../item-list/item-list.h"
#include "../../snapshots/int-snapshot.h"

class Button : public GuiComponent {
private:
    Item _item;
    IntSnapshot _snapshot;
    sf::RectangleShape _rect;
    bool _clicked_this_frame;

protected:
    // inherited from GuiComponent (sf::Drawable)
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

public:
    // constructors
    Button();
    Button(const sf::Vector2f& size,
           const sf::String& label);
    Button(const sf::Vector2f& size,
           const sf::String& label, 
           const sf::Font& font, 
           unsigned char_size);

	// destructor
	virtual ~Button();

	// inherited from GuiComponent (EventHandler)
	bool addEventHandler(sf::RenderWindow& window, const sf::Event& event);
	void update(float t=0);

	// inherited from GuiComponent (Snapshotable)
	virtual const Snapshot& getSnapshot() const;
	virtual void applySnapshot(Snapshot& snapshot);
	virtual void revertSnapshot(Snapshot& snapshot);

	// inherited from GuiComponent (Clickable)
	virtual sf::FloatRect getClickableBounds() const;
    virtual bool click(const sf::Vector2f& click_pos, sf::Mouse::Button mouse_button);
	virtual bool hover(const sf::Vector2f& hover_pos);
    virtual void unhover();

    // button functions
    // void onButtonPress() = 0;
    bool clickedThisFrame() const;

	// mutators
    void setLabel(const sf::String& label);
    void setLabelAlignment(Item::LabelAlignment align);
    void setPadding(float padding);
    void setTextOffset(float text_offset);
    void setDimensions(const sf::Vector2f& dims);
    void setFont(const sf::Font& font);
    void setCharacterSize(unsigned char_size);
    void setTextFillColor(const sf::Color& text_fill_color);
    void setTextOutlineColor(const sf::Color& text_outline_color);
    void setTextOutlineThickness(float text_outline_thickness);
    void setHighlightFillColor(const sf::Color& highlight_fill_color);
    void setHighlightOutlineColor(const sf::Color& highlight_outline_color);
    void setHighlightOutlineThickness(float highlight_outline_thickness);
    void setFillColor(const sf::Color& fill_color);
    void setOutlineColor(const sf::Color& outline_color);
    void setOutlineThickness(float outline_thickness);

    // accessors
    float getAdvanceOffset() const;
    const sf::String& getLabel() const;
    Item::LabelAlignment getLabelAlignment() const;
    float getPadding() const;
    float getTextOffset() const;
    const sf::Vector2f& getDimensions() const;
    const sf::Font* getFont() const;
    unsigned getCharacterSize() const;
    const sf::Color& getTextFillColor() const;
    const sf::Color& getTextOutlineColor() const;
    float getTextOutlineThickness() const;
    const sf::Color& getHighlightFillColor() const;
    const sf::Color& getHighlightOutlineColor() const;
    float getHighlightOutlineThickness() const;
    const sf::Color& getFillColor() const;
    const sf::Color& getOutlineColor() const;
    float getOutlineThickness() const;
};

#endif