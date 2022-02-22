/* **************************************************************************** *
 * AUTHOR:      Noah Krim
 * ASSIGNMENT:  GUI Cloth Sim
 * CLASS:       CS_08
 * ---------------------------------------------------------------------------- *
 * File: multi-text.cpp
 *  Definition file for MultiText class
 * **************************************************************************** */

#include "multi-text.h"

	// // private members
	// LinkedList<Letter> _list;
	// float _line_spacing;
	// // private members - letter defaults
	// Font* _font;
	// Color _fill_color;
	// Color _outline_color;
	// float _outline_thickness;
	// unsigned _letter_size;

/* ============================================================================ *
 * Contructors
 * ============================================================================ */
MultiText::MultiText() 
	:MultiText{sf::String{}, Letter{}}
{ }

MultiText::MultiText(const sf::String& str, const sf::Font& font, unsigned int letter_size)
	:MultiText{str, Letter{'\0', font, letter_size}, (float)letter_size}
{ }

MultiText::MultiText(const sf::String& str, const Letter& model_letter)
	:MultiText{str, model_letter, (float)model_letter.getCharacterSize()}
{ }

MultiText::MultiText(const sf::String& str, const Letter& model_letter, float line_spacing)
	: _list{}
	, _model_letter{model_letter}
	, _line_spacing{line_spacing}
{
	// copy characters from string
	for(auto it=str.begin(); it!=str.end(); ++it) {
		_model_letter.setChar(*it);
		_list.insert_tail(_model_letter);
	}
	align_all_letter_positions();
}



/* ============================================================================ *
 * Overloaded draw
 * ============================================================================ */
void MultiText::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= this->getTransform();
	for(auto it=_list.begin(); it!=_list.end(); ++it)
		target.draw(*it, states);
}



/* ============================================================================ *
 * Properties
 * ============================================================================ */
bool MultiText::empty() const {
	return _list.empty();
}

unsigned MultiText::size() const {
	return _list.size();
}



/* ============================================================================ *
 * Properties
 * ============================================================================ */
void MultiText::align_letter_position(iterator pos) {
	// invalid iterator
	if(pos == end())
		return;

	sf::Vector2f position {0,0};

	// first letter should be at {0,0}
	if(pos == begin()) {
		if(pos->getChar() == '\n')
			position.y += _line_spacing;
		pos->setPosition(position);
		return;
	}
	
	// iterator back to previous letter, while saving this pos
	iterator curr = pos--;
	position = pos->getPosition();
	// check if current is newline
	if(curr->getChar() == '\n') {
		position.x = this->getPosition().x;
		position.y += _line_spacing;
	}
	else
		position.x += pos->getAdvanceOffset();
	// set the position
	curr->setPosition(position);
}

void MultiText::align_all_letter_positions() {
	align_all_letter_positions(_list.begin());
}

void MultiText::align_all_letter_positions(iterator start) {
	if(start == _list.end())
		return;

	// initialize looping variables
	iterator curr = start;

	// use single-letter align for start
	align_letter_position(curr);
	sf::Vector2f prev_position = curr->getPosition();
	float prev_offset = curr->getAdvanceOffset();

	// continue on aligning positions until end of list
	for(++curr; curr!=_list.end(); ++curr) {
		// check if current is newline
		if(curr->getChar() == '\n') {
			prev_position.x = 0;
			prev_position.y += _line_spacing;
			prev_offset = 0;
		}
		else {
			prev_position.x += prev_offset;
		}
		// set position
		curr->setPosition(prev_position);
		// set prev_offset
		prev_offset = curr->getAdvanceOffset();
	}
}



/* ============================================================================ *
 * Copying
 * ============================================================================ */
// blank_copy(): copies everything but the LinkedList
MultiText MultiText::blank_copy() const {
	return MultiText("", _model_letter, _line_spacing);
}

MultiText MultiText::substr(const_iterator first) const {
	return substr(first, end());
}

MultiText MultiText::substr(const_iterator first, const_iterator last_exclusive) const {
	MultiText out = blank_copy();
	while(first != last_exclusive) {
		out._list.insert_tail(*first);
		++first;
	}
	return out;
}



/* ============================================================================ *
 * Insertion
 * ============================================================================ */
MultiText& MultiText::operator+=(sf::Uint32 ch) {
	_model_letter.setChar(ch);
	push_back(_model_letter);
	return *this;
}

MultiText& MultiText::operator+=(const Letter& ltr) {
	push_back(ltr);
	return *this;
}

MultiText& MultiText::operator+=(const sf::String& str) {
	for(auto it=str.begin(); it!=str.end(); ++it)
		push_back(*it);
	return *this;
}

MultiText& MultiText::operator+=(const MultiText& mt) {
	for(auto it=mt._list.begin(); it!=mt._list.end(); ++it)
		push_back(*it);
	return *this;
}

void MultiText::push_front(sf::Uint32 ch) {
	_model_letter.setChar(ch);
	align_all_letter_positions();
}

void MultiText::push_front(const Letter& ltr) {
	_list.insert_head(ltr);
	align_all_letter_positions();
}

void MultiText::push_back(sf::Uint32 ch) {
	_model_letter.setChar(ch);
	_list.insert_tail(_model_letter);
	align_letter_position(_list.tail_iter());
}

void MultiText::push_back(const Letter& ltr) {
	_list.insert_tail(ltr);
	align_letter_position(_list.tail_iter());
}

MultiText::iterator MultiText::insert(iterator pos, sf::Uint32 ch) {
	_model_letter.setChar(ch);
	auto it = _list.insert(pos, _model_letter);
	align_all_letter_positions(it);
	return it;
}

MultiText::iterator MultiText::insert(iterator pos, const Letter& ltr) {
	auto it = _list.insert(pos, ltr);
	align_all_letter_positions(it);
	return it;
}



/* ============================================================================ *
 * Removal
 * ============================================================================ */
bool MultiText::pop_back() {
	return _list.remove_tail();
}

bool MultiText::pop_front() {
	if(_list.remove_head()) {
		align_all_letter_positions();
		return true;
	}
	return false;
}

MultiText::iterator MultiText::erase(iterator pos, unsigned count) {
	auto it = _list.remove(pos, count);
	align_all_letter_positions(it);
	return it;
}

MultiText::iterator MultiText::erase(iterator first, iterator last_exclusive) {
	auto it = _list.remove(first, last_exclusive);
	align_all_letter_positions(it);
	return it;
}

void MultiText::substr_inplace(iterator first) {
	substr_inplace(first, end());
	align_all_letter_positions();
}

void MultiText::substr_inplace(iterator first, iterator last_exclusive) {
	_list.remove(begin(), first);
	_list.remove(last_exclusive, end());
	align_all_letter_positions();
}

void MultiText::clear() {
	_list.clear();
}



/* ============================================================================ *
 * Lookup
 * ============================================================================ */
Letter& MultiText::at(unsigned index) {
	return _list.at(index);
}
const Letter& MultiText::at(unsigned index) const {
	return _list.at(index);
}

MultiText::iterator MultiText::iter_at(unsigned index) {
	return _list.iter_at(index);
}
MultiText::const_iterator MultiText::iter_at(unsigned index) const {
	return _list.iter_at(index);
}

Letter& MultiText::front() {
	return _list.front();
}
const Letter& MultiText::front() const {
	return _list.front();
}

Letter& MultiText::back() {
	return _list.back();
}
const Letter& MultiText::back() const {
	return _list.back();
}

MultiText::iterator MultiText::find(sf::Uint32 ch) {
	_model_letter.setChar(ch);
	return _list.find(_model_letter);
}

MultiText::const_iterator MultiText::find(sf::Uint32 ch) const {
	return _list.find(Letter(ch));
}


MultiText::iterator MultiText::find(sf::Uint32 ch, iterator start) {
	_model_letter.setChar(ch);
	return _list.find(_model_letter, start);
}

MultiText::const_iterator MultiText::find(sf::Uint32 ch, const_iterator start) const {
	return _list.find(Letter(ch), start);
}

bool MultiText::contains(sf::Uint32 ch) const {
	return _list.contains(Letter(ch));
}

unsigned MultiText::count(sf::Uint32 ch) const {
	return _list.count(Letter(ch));
}



/* ============================================================================ *
 * Mutators
 * ============================================================================ */
void MultiText::setString(const sf::String& str) {
	_list.clear();
	for(auto it=str.begin(); it!=str.end(); ++it) {
		_model_letter.setChar(*it);
		_list.insert_tail(_model_letter);
	}
	align_all_letter_positions();
}

void MultiText::setLineSpacing(float line_spacing) {
	_line_spacing = line_spacing;
}

void MultiText::setAllFont(const sf::Font& font) {
	_model_letter.setFont(font);
	for(auto it=_list.begin(); it!=_list.end(); ++it)
		it->setFont(font);
	align_all_letter_positions();
}

void MultiText::setAllFillColor(const sf::Color& fill_color) {
	_model_letter.setFillColor(fill_color);
	for(auto it=_list.begin(); it!=_list.end(); ++it)
		it->setFillColor(fill_color);
}

void MultiText::setAllOutlineColor(const sf::Color& outline_color) {
	_model_letter.setOutlineColor(outline_color);
	for(auto it=_list.begin(); it!=_list.end(); ++it)
		it->setOutlineColor(outline_color);
}

void MultiText::setAllOutlineThickness(float outline_thickness) {
	_model_letter.setOutlineThickness(outline_thickness);
	for(auto it=_list.begin(); it!=_list.end(); ++it)
		it->setOutlineThickness(outline_thickness);
	align_all_letter_positions();
}

void MultiText::setAllLetterSize(unsigned letter_size) {
	_model_letter.setCharacterSize(letter_size);
	for(auto it=_list.begin(); it!=_list.end(); ++it)
		it->setCharacterSize(letter_size);
	align_all_letter_positions();
}

void MultiText::setAllLetterSpacing(float letter_spacing) {
	_model_letter.setLetterSpacing(letter_spacing);
	for(auto it=_list.begin(); it!=_list.end(); ++it)
		it->setLetterSpacing(letter_spacing);
	align_all_letter_positions();
}



/* ============================================================================ *
 * Mutators - Defaults
 * ============================================================================ */
void MultiText::setDefaultFont(const sf::Font& font) {
	_model_letter.setFont(font);
}

void MultiText::setDefaultFillColor(const sf::Color& fill_color) {
	_model_letter.setFillColor(fill_color);
}

void MultiText::setDefaultOutlineColor(const sf::Color& outline_color) {
	_model_letter.setOutlineColor(outline_color);
}

void MultiText::setDefaultOutlineThickness(float outline_thickness) {
	_model_letter.setOutlineThickness(outline_thickness);
}

void MultiText::setDefaultLetterSize(unsigned letter_size) {
	_model_letter.setCharacterSize(letter_size);
}

void MultiText::setDefaultLetterSpacing(float letter_spacing) {
	_model_letter.setLetterSpacing(letter_spacing);
}



/* ============================================================================ *
 * Accessors
 * ============================================================================ */
float MultiText::getLineSpacing() const {
	return _line_spacing;
}

const sf::Font* MultiText::getDefaultFont() const {
	return _model_letter.getFont();
}

const sf::Color& MultiText::getDefaultFillColor() const {
	return _model_letter.getFillColor();
}

const sf::Color& MultiText::getDefaultOutlineColor() const {
	return _model_letter.getOutlineColor();
}

float MultiText::getDefaultOutlineThickness() const {
	return _model_letter.getOutlineThickness();
}

unsigned MultiText::getDefaultLetterSize() const {
	return _model_letter.getCharacterSize();
}

float MultiText::getDefaultLetterSpacing() const {
	return _model_letter.getLetterSpacing();
}

Letter MultiText::createDefaultLetter() const {
	Letter out = _model_letter;

	// _model_letter's char is changed within funcitons to prevent unnecessary copying, 
	//  so out's char is reset to prevent unwanted introspection
	out.setChar('\0');	
	return out;
}



/* ============================================================================ *
 * Conversions
 * ============================================================================ */
sf::String MultiText::toString() const {
	sf::String out;
	sf::String ch;
	for(auto it=_list.begin(); it!=_list.end(); ++it) {
		ch = sf::String(it->getChar());
		out += ch;
	}
	return out;
}



/* ============================================================================ *
 * Mutators - Defaults
 * ============================================================================ */
std::ostream& operator<<(std::ostream& os, const MultiText& mt) {
	for(auto it=mt._list.begin(); it!=mt._list.end(); ++it) {
		if((char)*it == '\n')
			os << std::endl;
		else 
			os << *it;
	}
	return os;
}



/* ============================================================================ *
 * Iterators
 * ============================================================================ */
MultiText::iterator MultiText::begin() 							{ return _list.begin(); }
MultiText::const_iterator MultiText::begin() const 				{ return _list.begin(); }
MultiText::iterator MultiText::end()							{ return _list.end(); }
MultiText::const_iterator MultiText::end() const 				{ return _list.end(); }
MultiText::reverse_iterator MultiText::rbegin() 				{ return _list.rbegin(); }
MultiText::const_reverse_iterator MultiText::rbegin() const 	{ return _list.rbegin(); }
MultiText::reverse_iterator MultiText::rend()					{ return _list.rend(); }
MultiText::const_reverse_iterator MultiText::rend() const 		{ return _list.rend(); }
MultiText::const_iterator MultiText::cbegin() const 			{ return _list.cbegin(); }
MultiText::const_iterator MultiText::cend() const 				{ return _list.cend(); }
MultiText::const_reverse_iterator MultiText::crbegin() const 	{ return _list.crbegin(); }
MultiText::const_reverse_iterator MultiText::crend() const 		{ return _list.crend(); }