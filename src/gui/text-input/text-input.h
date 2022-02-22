/* **************************************************************************** *
 * AUTHOR:      Noah Krim
 * ASSIGNMENT:  GUI Cloth Sim
 * CLASS:       CS_08
 * ---------------------------------------------------------------------------- *
 * File: text-input.h
 *  Header file for typable text-input class
 * **************************************************************************** */

#ifndef TEXT_INPUT_H
#define TEXT_INPUT_H

#include <SFML/Graphics.hpp>
#include <array>

#include "../component-interface/gui-component.h"
#include "../../snapshots/string-change-snapshot.h"

#include "../multi-text/multi-text.h"
#include "../multi-text/letter.h"
#include "../multi-text/multi-text-cursor.h"


class TextInput : public GuiComponent {

// data
protected:
    // protected member variables 
    //  (children may want direct access to MultiText objects)
    // MultiText and Cursor
    MultiText _placeholder;
    MultiText _text;
    MultiTextCursor _cursor;
    // Snapshot storage
    StringChangeSnapshot _snapshot;

private:
    // private member variables
    // Rendering tools
    mutable sf::RenderTexture _render_tex;
    sf::Sprite _render_sprite;
    sf::RectangleShape _render_rect;
    // Extra rendering variables
    sf::Vector2f _text_offset;
    // Accessories
    sf::Text _label;
    float _min_label_width;
    bool _multi_line;

public:
    // global static constants
    static constexpr float CURSOR_INTERVAL = 0.5f;
    static constexpr float CURSOR_Y_OFFSET = 2.0f;

// functions
protected:
    // inherited from GuiComponent (sf::Drawable)
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

public:
    // constructors
    TextInput();
    TextInput(const sf::Font& font, const sf::Vector2u& dimensions, 
            unsigned char_size=Letter::DEFAULT_LETTER_SIZE,
            bool multi_line=false);
    TextInput(const sf::String& label, const sf::String& placeholder_str, 
            const sf::Font& font, const sf::Vector2u& dimensions,
            unsigned char_size=Letter::DEFAULT_LETTER_SIZE,
            bool multi_line=false);

    TextInput(const TextInput& o);

    // virtual destructor
    virtual ~TextInput();

    // inhereted from GuiComponent (EventHandler)
    virtual bool addEventHandler(sf::RenderWindow& window, const sf::Event& event);
    virtual void update(float t=0);

    // inherited from GuiComponent (Snapshotable)
    virtual const Snapshot& getSnapshot() const;
    virtual void applySnapshot(Snapshot& snapshot);
    virtual void revertSnapshot(Snapshot& snapshot);

    // inherited from GuiComponent (Clickable)
    virtual sf::FloatRect getClickableBounds() const;
    // virtual bool click(const sf::Vector2f& click_pos);
    // virtual bool hover(const sf::Vector2f& hover_pos);

    // bounds information
    sf::FloatRect getLocalBounds() const;
    sf::FloatRect getGlobalBounds() const;

    // selection information
    // unsigned selectionSize() const;
    // sf::String getSelection() const;

    // mutators
    void setText(const MultiText& text);
    void setTextString(const sf::String& text);
    void setPlaceholder(const MultiText& placeholder);
    void setPlaceholderString(const sf::String& placeholder);
    void setFont(const sf::Font& font);
    void setCharacterSize(unsigned size);
    void setLetterSpacing(float letter_spacing);
    void setLineSpacing(float line_spacing);
    void setDimensions(const sf::Vector2u& dims);
    void setMultiLine(bool multi_line);
    void setLabel(const sf::String& label);
    void setTextFillColor(const sf::Color& text_fill_color);
    void setPlaceholderFillColor(const sf::Color& placeholder_fill_color);
    void setFillColor(const sf::Color& fill_color);
    void setOutlineColor(const sf::Color& outline_color);
    void setOutlineThickness(float outline_thickness);
    void setCursorColor(const sf::Color& cursor_color);
    void setTextOffset(const sf::Vector2f& text_offset);
    void setMinLabelWidth(float min_label_width);

    // accessors
    const MultiText& getText() const;
    const MultiText& getPlaceholder() const;
    const sf::Font* getFont() const;
    unsigned getCharacterSize() const;
    float getLetterSpacing() const;
    float getLineSpacing() const;
    unsigned getTextLength() const;
    sf::Vector2u getDimensions() const;
    bool getMultiLine() const;
    const sf::String& getLabel() const;
    const sf::Color& getTextFillColor() const;
    const sf::Color& getPlaceholderFillColor() const;
    const sf::Color& getFillColor() const;
    const sf::Color& getOutlineColor() const;
    float getOutlineThickness() const;
    const sf::Color& getCursorColor() const;
    const sf::Vector2f& getTextOffset() const;
    float getMinLabelWidth() const;

// private functions
private:

};

#endif 