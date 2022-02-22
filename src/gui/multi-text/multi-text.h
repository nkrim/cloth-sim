/* **************************************************************************** *
 * AUTHOR:      Noah Krim
 * ASSIGNMENT:  GUI Cloth Sim
 * CLASS:       CS_08
 * ---------------------------------------------------------------------------- *
 * File: multi-text.h
 *  Header file for MultiText class
 * **************************************************************************** */

#ifndef MULTI_TEXT_H
#define MULTI_TEXT_H

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

#include "../../data-structures/linked-list.h"
#include "letter.h"


class MultiText : public sf::Transformable, public sf::Drawable {
public:
	// iterator typedefs
	typedef LinkedList<Letter>::iterator iterator;
	typedef LinkedList<Letter>::reverse_iterator reverse_iterator;
	typedef LinkedList<Letter>::const_iterator const_iterator;
	typedef LinkedList<Letter>::const_reverse_iterator const_reverse_iterator;

private:
	// private members
	LinkedList<Letter> _list;
	Letter _model_letter;
	float _line_spacing;

protected:
	// overloaded draw
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

public:
	// constructors
	MultiText();
	MultiText(const sf::String& str, 
				const sf::Font& font, 
				unsigned letter_size=Letter::DEFAULT_LETTER_SIZE);
	MultiText(const sf::String& str, const Letter& model_letter);
	MultiText(const sf::String& str, const Letter& model_letter, float line_spacing);

	// properties
	bool empty() const;
	unsigned size() const;

	// rendering
	void align_letter_position(iterator it);
	void align_letter_position(reverse_iterator it);
	void align_all_letter_positions();
	void align_all_letter_positions(iterator start);

	// copying
	// blank_copy(): copies everything but the LinkedList
	MultiText blank_copy() const;
	MultiText substr(const_iterator first) const;
	MultiText substr(const_iterator first, const_iterator last_exclusive) const;

	// insertion
	MultiText& operator+=(sf::Uint32 ch);
	MultiText& operator+=(const Letter& ltr);
	MultiText& operator+=(const sf::String& str);
	MultiText& operator+=(const MultiText& mt);
	void push_front(sf::Uint32 ch);
	void push_front(const Letter& ltr);
	void push_back(sf::Uint32 ch);
	void push_back(const Letter& ltr);
	iterator insert(iterator pos, sf::Uint32 ch);
	iterator insert(iterator pos, const Letter& ltr);

	// removal
	bool pop_back();
	bool pop_front();
	iterator erase(iterator pos, unsigned count=1);
	iterator erase(iterator first, iterator last_exclusive);
	void substr_inplace(iterator first);
	void substr_inplace(iterator first, iterator last_exclusive);
	void clear();

	// lookup
	Letter& at(unsigned index);
	const Letter& at(unsigned index) const;
	iterator iter_at(unsigned index);
	const_iterator iter_at(unsigned index) const;
	Letter& front();
	const Letter& front() const;
	Letter& back();
	const Letter& back() const;
	iterator find(sf::Uint32 ch);
	const_iterator find(sf::Uint32 ch) const;
	iterator find(sf::Uint32 ch, iterator start);
	const_iterator find(sf::Uint32 ch, const_iterator start) const;
	bool contains(sf::Uint32 ch) const;
	unsigned count(sf::Uint32 ch) const;

	// mutators - overall
	void setString(const sf::String& str);
	void setLineSpacing(float line_spacing);
	void setAllFont(const sf::Font& font);
	void setAllFillColor(const sf::Color& fill_color);
	void setAllOutlineColor(const sf::Color& outline_color);
	void setAllOutlineThickness(float outline_thickness);
	void setAllLetterSize(unsigned letter_size);
	void setAllLetterSpacing(float letter_spacing);

	// mutators - defaults
	void setDefaultFont(const sf::Font& font);
	void setDefaultFillColor(const sf::Color& fill_color);
	void setDefaultOutlineColor(const sf::Color& outline_color);
	void setDefaultOutlineThickness(float outline_thickness);
	void setDefaultLetterSize(unsigned letter_size);
	void setDefaultLetterSpacing(float letter_spacing);

	// accessors
	float getLineSpacing() const;
	const sf::Font* getDefaultFont() const;
	const sf::Color& getDefaultFillColor() const;
	const sf::Color& getDefaultOutlineColor() const;
	float getDefaultOutlineThickness() const;
	unsigned getDefaultLetterSize() const;
	float getDefaultLetterSpacing() const;
	Letter createDefaultLetter() const;

	// conversion 
	sf::String toString() const;

	// io
	friend std::ostream& operator<<(std::ostream& os, const MultiText& mt);

	// iterators
	iterator begin();
	const_iterator begin() const;
	iterator end();
	const_iterator end() const;
	reverse_iterator rbegin();
	const_reverse_iterator rbegin() const;
	reverse_iterator rend();
	const_reverse_iterator rend() const;
	const_iterator cbegin() const;
	const_iterator cend() const;
	const_reverse_iterator crbegin() const;
	const_reverse_iterator crend() const;

};

#endif