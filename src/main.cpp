/* **************************************************************************** *
 * AUTHOR:      Noah Krim
 * ASSIGNMENT:  GUI Cloth Sim
 * CLASS:       CS_08
 * ---------------------------------------------------------------------------- *
 * File: main.cpp
 *  Test application for TextBox with MultiText class
 * **************************************************************************** */

#include <SFML/Graphics.hpp>

#include "../libs/nfd/src/include/nfd.h"

#include "system/global-entities.h"
#include "system/mouse-tracker.h"
#include "system/history.h"

#include "gui/number-input/number-input.h"
#include "gui/dropdown/dropdown-menu.h"
#include "gui/text-input/text-input.h"
#include "gui/button/button.h"
#include "gui/menu/menu-bar.h"

#include "menu-controllers/parrot-menu-controller.h"
#include "menu-controllers/bitmask-menu-controller.h"

#include "app/help-text-box.h"

#include "cloth/cloth.h"

#include <string>
#include <iostream>
#include <vector>

using std::cout;
using std::cerr;
using std::endl;

// global constants
const sf::Vector2f WINDOW_SIZE = {2400, 1600};

const std::string FONT_FILE = "resources/fonts/OpenSans-Bold.ttf";
const std::string FONT_FILE_MONO = "resources/fonts/Cousine-Bold.ttf";
const std::string IMG_TEX_FILE = "resources/img/napkin.png";
const unsigned TEXT_SIZE = 36;
const unsigned TEXT_REND_TEX_SIZE = 1080;
const unsigned TEXT_REND_TEX_PADDING = 32;
const unsigned TEXT_REND_TEXT_COLOR = 0x222222FF;
const unsigned TEXT_REND_CHAR_SIZE = 72;

// cloth initial values
const Cloth::size_type INIT_CLOTH_VERTICES = Cloth::DEFAULT_SIDE_VERTEX_COUNT;
const float INIT_CLOTH_SCALE = 2.0f;
const glm::vec3 INIT_LIGHT_DIR = {2.0f,2.0f,-10.0f};
const glm::vec3 INIT_GRAVITY = {0,-9.8f,0};
const glm::vec3 INIT_WIND_FORCE = {0,0,0};

// menubar enums
enum FileIDs {
    Load=0,
    Save
};
enum EditIDs {
    Undo=0,
    Redo
};
enum ViewIDs {
    ResetSim=0,
    ResetGui
};

// inline shortcuts 
#define CLOTH_PAUSE cloth.pause(); pauseButton.setLabel("Resume"); 
#define CLOTH_RESUME cloth.resume(); pauseButton.setLabel("Pause"); 

// main function
int main()
{
    // setup window
    sf::RenderWindow window(
        sf::VideoMode(WINDOW_SIZE.x, WINDOW_SIZE.y),
        "Cloth Simulation", 
        0b0101 // NO fullscreen, close button, NO resize, title bar
        );
    
    // -- SFML v-sync causes weird mouse-movement delays
    // window.setVerticalSyncEnabled(true);
        
    // setup mouse tracker
    Global::mouse_tracker.setRenderWindow(window);

    // setup font
    std::vector<sf::Font*> fonts;
    sf::Font text_font;
    if(!text_font.loadFromFile(FONT_FILE)) {
        cout << "Could not load font file: " << FONT_FILE << endl
             << "Aborting program..." << endl;
        return 1;
    }
    fonts.push_back(&text_font);
    sf::Font text_font_mono;
    if(!text_font_mono.loadFromFile(FONT_FILE_MONO)) {
        cout << "Could not load font file: " << FONT_FILE_MONO << endl
             << "Aborting program..." << endl;
        return 1;
    }
    fonts.push_back(&text_font_mono);

    // setup history
    History history;

    // setup cursors
    sf::Cursor default_cursor; default_cursor.loadFromSystem(sf::Cursor::Arrow);
    sf::Cursor text_cursor; text_cursor.loadFromSystem(sf::Cursor::Text);
    sf::Cursor hand_cursor; hand_cursor.loadFromSystem(sf::Cursor::Hand);
    sf::Cursor cross_cursor; cross_cursor.loadFromSystem(sf::Cursor::Cross);
    window.setMouseCursor(default_cursor);

    // setup component vector
    std::vector<GuiComponent*> components;

    // setup default menu
    MenuList default_menu(sf::String(), text_font, TEXT_SIZE);
    default_menu.setBackgroundFillColor(sf::Color(0xDDDDDDFF));
    default_menu.setBackgroundOutlineColor(sf::Color(0x57595DFF));
    default_menu.setBackgroundOutlineThickness(4.0f);
    default_menu.setItemTextFillColor(sf::Color(0x222222FF));
    default_menu.setItemHighlightFillColor(sf::Color(0xFFFFFFFF));
    default_menu.setItemPadding(20);
    default_menu.setItemTextOffset(-10);
    default_menu.setItemDimensions({800, 60});
    default_menu.setMenuHeaderDimensions({160, 60});

    // setup menu bar
    MenuBar menu_bar;
    // menu_bar.setPosition({4.0f,4.0f});
    components.push_back(&menu_bar);
    Global::mouse_tracker.addClickableComponent(menu_bar);

    // setup file menu
    MenuList file_menu {default_menu};
    file_menu.setMenuLabel(sf::String("File"));
    file_menu.addItem(sf::String("Load Image"), Load);
    file_menu.addItem(sf::String("Save As"), Save);
    // setup parrot menu controller
    BitmaskMenuController file_controller;
    file_menu.setMenuController(&file_controller);
    menu_bar.addMenuList(file_menu);

    // setup edit menu
    MenuList edit_menu {default_menu};
    edit_menu.setMenuLabel(sf::String("Edit"));
    edit_menu.addItem(sf::String("Undo"), Undo);
    edit_menu.addItem(sf::String("Redo"), Redo);
    // setup parrot menu controller
    BitmaskMenuController edit_controller;
    edit_menu.setMenuController(&edit_controller);
    menu_bar.addMenuList(edit_menu);

    // setup view menu
    MenuList view_menu {default_menu};
    view_menu.setMenuLabel(sf::String("View"));
    view_menu.addItem(sf::String("Restart Simulation"), ResetSim);
    view_menu.addItem(sf::String("Reset to Default Settings"), ResetGui);
    // setup parrot menu controller
    BitmaskMenuController view_controller;
    view_menu.setMenuController(&view_controller);
    menu_bar.addMenuList(view_menu);

    // setup text_input
    TextInput text_input(sf::String("Text: "), sf::String("Text on cloth..."), 
                        text_font, {816, 400},
                        TEXT_SIZE, true);
    components.push_back(&text_input);
    Global::mouse_tracker.addClickableComponent(text_input);
    text_input.setPosition({20,150});
    text_input.setTextFillColor(sf::Color(0x222222FF));
    text_input.setPlaceholderFillColor(sf::Color(0x2222227F));
    text_input.setFillColor(sf::Color(0xDDDDDDFF));
    text_input.setOutlineColor(sf::Color(0x57595DFF));
    text_input.setOutlineThickness(4.0f);
    text_input.setCursorColor(sf::Color::Black);
    text_input.setTextOffset({10.0f,10.0f});
    text_input.setHoverCursor(text_cursor);

    // setup dropdown for text alignment
    DropdownMenu text_align({}, text_font, TEXT_SIZE, "Select Text Align...");
    components.push_back(&text_align);
    Global::mouse_tracker.addClickableComponent(text_align);
    text_align.addItem(sf::String("Top Left"), 0x00);
    text_align.addItem(sf::String("Top Center"), 0x01);
    text_align.addItem(sf::String("Top Right"), 0x02);
    text_align.addItem(sf::String("Middle Left"), 0x10);
    text_align.addItem(sf::String("Middle Center"), 0x11);
    text_align.addItem(sf::String("Middle Right"), 0x12);
    text_align.addItem(sf::String("Bottom Left"), 0x20);
    text_align.addItem(sf::String("Bottom Center"), 0x21);
    text_align.addItem(sf::String("Bottom Right"), 0x22);
    text_align.setPosition(text_input.getPosition()+sf::Vector2f{0,text_input.getDimensions().y+50.0f});
    text_align.setBackgroundFillColor(sf::Color(0xDDDDDDFF));
    text_align.setBackgroundOutlineColor(sf::Color(0x57595DFF));
    text_align.setBackgroundOutlineThickness(4.0f);
    text_align.setItemTextFillColor(sf::Color(0x222222FF));
    text_align.setItemHighlightFillColor(sf::Color(0xFFFFFFFF));
    text_align.setItemHighlightOutlineColor(sf::Color(0x000000FF));
    text_align.setItemHighlightOutlineThickness(4.0f);
    text_align.setItemPadding(20);
    text_align.setItemTextOffset(-4.0f);
    text_align.setItemDimensions({430, 50});
    text_align.setShowcasedItemHeight(70);

    // setup dropdown for font
    DropdownMenu font_select({}, text_font, TEXT_SIZE, "Select Text Font...");
    components.push_back(&font_select);
    Global::mouse_tracker.addClickableComponent(font_select);
    font_select.addItem(sf::String("Sans Serif"));
    font_select.addItem(sf::String("Monospace"));
    font_select.setPosition(text_align.getPosition()+sf::Vector2f{480.0f,0});
    font_select.setBackgroundFillColor(sf::Color(0xDDDDDDFF));
    font_select.setBackgroundOutlineColor(sf::Color(0x57595DFF));
    font_select.setBackgroundOutlineThickness(4.0f);
    font_select.setItemTextFillColor(sf::Color(0x222222FF));
    font_select.setItemHighlightFillColor(sf::Color(0xFFFFFFFF));
    font_select.setItemHighlightOutlineColor(sf::Color(0x000000FF));
    font_select.setItemHighlightOutlineThickness(4.0f);
    font_select.setItemPadding(20);
    font_select.setItemTextOffset(-4.0f);
    font_select.setItemDimensions({430, 50});
    font_select.setShowcasedItemHeight(70);

    // setup cloth text size
    NumberInput cloth_text_size(NumberInput::Int, "Text Size: ", text_font, 
                {150, 50}, TEXT_SIZE);
    components.push_back(&cloth_text_size);
    Global::mouse_tracker.addClickableComponent(cloth_text_size);
    cloth_text_size.setPosition(text_align.getPosition()+sf::Vector2f{0,100.0f});
    cloth_text_size.setTextFillColor(sf::Color(0x222222FF));
    cloth_text_size.setPlaceholderFillColor(sf::Color(0x2222227F));
    cloth_text_size.setFillColor(sf::Color(0xDDDDDDFF));
    cloth_text_size.setOutlineColor(sf::Color(0x57595DFF));
    cloth_text_size.setOutlineThickness(4.0f);
    cloth_text_size.setCursorColor(sf::Color::Black);
    cloth_text_size.setTextOffset({10.0f,2.0f});
    cloth_text_size.setHoverCursor(text_cursor);
    cloth_text_size.setMinLabelWidth(280.0f);
    cloth_text_size.setMinIntValue(1);
    cloth_text_size.setIntValue(TEXT_REND_CHAR_SIZE);

    // setup cloth line spacing
    NumberInput cloth_line_spacing(cloth_text_size);
    components.push_back(&cloth_line_spacing);
    Global::mouse_tracker.addClickableComponent(cloth_line_spacing);
    cloth_line_spacing.setPosition(cloth_line_spacing.getPosition()+sf::Vector2f{450,0});
    cloth_line_spacing.setNumberType(NumberInput::Float);
    cloth_line_spacing.setMinFloatValue(1.0f);
    cloth_line_spacing.setFloatValue(TEXT_REND_CHAR_SIZE);
    cloth_line_spacing.setLabel("Line Spacing: ");

    // setup cloth text color
    NumberInput cloth_text_color(cloth_text_size);
    components.push_back(&cloth_text_color);
    Global::mouse_tracker.addClickableComponent(cloth_text_color);
    cloth_text_color.setPosition(cloth_text_color.getPosition()+sf::Vector2f{0,100});
    cloth_text_color.setNumberType(NumberInput::Hex32);
    cloth_text_color.setDimensions(cloth_text_color.getDimensions()+sf::Vector2u{60,0});
    cloth_text_color.setMinLabelWidth(220.0f);
    cloth_text_color.setHexValue(TEXT_REND_TEXT_COLOR);
    cloth_text_color.setLabel("Text Color: ");

     // setup cloth color
    NumberInput cloth_color(cloth_text_color);
    components.push_back(&cloth_color);
    Global::mouse_tracker.addClickableComponent(cloth_color);
    cloth_color.setPosition(cloth_color.getPosition()+sf::Vector2f{450,0});
    cloth_color.setDimensions(cloth_color.getDimensions()+sf::Vector2u{60,0});
    cloth_color.setHexValue(0xFFFFFFFF);
    cloth_color.setLabel("Cloth Color: ");

    // setup dropdown for fixed point arrangement
    DropdownMenu fpa_select({}, text_font, TEXT_SIZE, "Select Cloth Configuration...");
    components.push_back(&fpa_select);
    Global::mouse_tracker.addClickableComponent(fpa_select);
    fpa_select.addItem(sf::String("Curtain"), Cloth::Curtain);
    fpa_select.addItem(sf::String("Flag (Left)"), Cloth::FlagLeft);
    fpa_select.addItem(sf::String("Flat (Right)"), Cloth::FlatRight);
    fpa_select.addItem(sf::String("Four Corners"), Cloth::FourCorners);
    fpa_select.addItem(sf::String("All Top"), Cloth::AllTop);
    fpa_select.addItem(sf::String("All Left"), Cloth::AllLeft);
    fpa_select.addItem(sf::String("All Right"), Cloth::AllRight);
    fpa_select.addItem(sf::String("All Left & Right"), Cloth::AllLeftRight);
    fpa_select.addItem(sf::String("Full Perimeter"), Cloth::AllPerimeter);
    fpa_select.addItem(sf::String("Loose"), Cloth::Loose);
    fpa_select.setPosition(cloth_text_color.getPosition()+sf::Vector2f{0, 100.0f});
    fpa_select.setBackgroundFillColor(sf::Color(0xDDDDDDFF));
    fpa_select.setBackgroundOutlineColor(sf::Color(0x57595DFF));
    fpa_select.setBackgroundOutlineThickness(4.0f);
    fpa_select.setItemTextFillColor(sf::Color(0x222222FF));
    fpa_select.setItemHighlightFillColor(sf::Color(0xFFFFFFFF));
    fpa_select.setItemHighlightOutlineColor(sf::Color(0x000000FF));
    fpa_select.setItemHighlightOutlineThickness(4.0f);
    fpa_select.setItemPadding(20);
    fpa_select.setItemTextOffset(-4.0f);
    fpa_select.setItemDimensions({600, 50});
    fpa_select.setShowcasedItemHeight(70);

    // setup cloth vertices
    NumberInput cloth_vertices(cloth_text_size);
    components.push_back(&cloth_vertices);
    Global::mouse_tracker.addClickableComponent(cloth_vertices);
    cloth_vertices.setPosition(fpa_select.getPosition()+sf::Vector2f{0,150});
    cloth_vertices.setMinIntValue(2);
    cloth_vertices.setMaxIntValue(64);
    cloth_vertices.setIntValue(INIT_CLOTH_VERTICES);
    cloth_vertices.setLabel("Cloth Vertices: ");

    // setup cloth scale
    NumberInput cloth_scale(cloth_vertices);
    components.push_back(&cloth_scale);
    Global::mouse_tracker.addClickableComponent(cloth_scale);
    cloth_scale.setPosition(cloth_vertices.getPosition()+sf::Vector2f{450,0});
    cloth_scale.setNumberType(NumberInput::Float);
    cloth_scale.setMinFloatValue(1.0f);
    cloth_scale.setFloatValue(INIT_CLOTH_SCALE);
    cloth_scale.setLabel("Cloth Scale: ");

    // setup gravity x
    NumberInput gravity_x(cloth_vertices);
    components.push_back(&gravity_x);
    Global::mouse_tracker.addClickableComponent(gravity_x);
    gravity_x.setNumberType(NumberInput::Float);
    gravity_x.setMinFloatValue(std::numeric_limits<float>::lowest());
    gravity_x.setFloatValue(INIT_GRAVITY.x);
    gravity_x.setLabel("Gravity X: ");
    gravity_x.setPosition(gravity_x.getPosition()+sf::Vector2f{0, 100});
    // setup gravity y
    NumberInput gravity_y(gravity_x);
    components.push_back(&gravity_y);
    Global::mouse_tracker.addClickableComponent(gravity_y);
    gravity_y.setFloatValue(INIT_GRAVITY.y);
    gravity_y.setLabel("Gravity Y: ");
    gravity_y.setPosition(gravity_y.getPosition()+sf::Vector2f{0,55});
    // setup gravity z
    NumberInput gravity_z(gravity_y);
    components.push_back(&gravity_z);
    Global::mouse_tracker.addClickableComponent(gravity_z);
    gravity_z.setFloatValue(INIT_GRAVITY.z);
    gravity_z.setLabel("Gravity Z: ");
    gravity_z.setPosition(gravity_z.getPosition()+sf::Vector2f{0,55});

    // setup wind force x
    NumberInput wind_force_x(gravity_z);
    components.push_back(&wind_force_x);
    Global::mouse_tracker.addClickableComponent(wind_force_x);
    wind_force_x.setFloatValue(INIT_WIND_FORCE.x);
    wind_force_x.setLabel("Wind Force X: ");
    wind_force_x.setPosition(wind_force_x.getPosition()+sf::Vector2f{0,100});
    // setup wind force y
    NumberInput wind_force_y(wind_force_x);
    components.push_back(&wind_force_y);
    Global::mouse_tracker.addClickableComponent(wind_force_y);
    wind_force_y.setFloatValue(INIT_WIND_FORCE.y);
    wind_force_y.setLabel("Wind Force Y: ");
    wind_force_y.setPosition(wind_force_y.getPosition()+sf::Vector2f{0,55});
    // setup wind force z
    NumberInput wind_force_z(wind_force_y);
    components.push_back(&wind_force_z);
    Global::mouse_tracker.addClickableComponent(wind_force_z);
    wind_force_z.setFloatValue(INIT_WIND_FORCE.z);
    wind_force_z.setLabel("Wind Force Z: ");
    wind_force_z.setPosition(wind_force_z.getPosition()+sf::Vector2f{0,55});

    // setup light_dir x
    NumberInput light_dir_x(gravity_x);
    components.push_back(&light_dir_x);
    Global::mouse_tracker.addClickableComponent(light_dir_x);
    light_dir_x.setFloatValue(INIT_LIGHT_DIR.x);
    light_dir_x.setLabel("Light Dir X: ");
    light_dir_x.setPosition(light_dir_x.getPosition()+sf::Vector2f{450,0});
    // setup light_dir y
    NumberInput light_dir_y(light_dir_x);
    components.push_back(&light_dir_y);
    Global::mouse_tracker.addClickableComponent(light_dir_y);
    light_dir_y.setFloatValue(INIT_LIGHT_DIR.y);
    light_dir_y.setLabel("Light Dir Y: ");
    light_dir_y.setPosition(light_dir_y.getPosition()+sf::Vector2f{0,55});
    // setup light_dir z
    NumberInput light_dir_z(light_dir_y);
    components.push_back(&light_dir_z);
    Global::mouse_tracker.addClickableComponent(light_dir_z);
    light_dir_z.setFloatValue(INIT_LIGHT_DIR.z);
    light_dir_z.setLabel("Light Dir Z: ");
    light_dir_z.setPosition(light_dir_z.getPosition()+sf::Vector2f{0,55});

    // setup cloth
    unsigned cloth_component_index = components.size();
    Cloth cloth({1400, 1200}, INIT_CLOTH_SCALE);
    components.push_back(&cloth);
    Global::mouse_tracker.addClickableComponent(cloth);
    cloth.setPosition(980, 20);
    cloth.loadImgTexFromFile(IMG_TEX_FILE);
    cloth.init_renderer();
    cloth.setOutlineColor(sf::Color(0x57595DFF));
    cloth.setFocusedOutlineColor(sf::Color(0xFFFFFFFF));
    cloth.setOutlineThickness(4.0f);
    cloth.setHoverCursor(hand_cursor);
    cloth.setClickCursor(cross_cursor);

    // setup pause button
    Button pauseButton({1400, 80}, "Pause", text_font, TEXT_SIZE);
    components.push_back(&pauseButton);
    Global::mouse_tracker.addClickableComponent(pauseButton);
    pauseButton.setPosition(cloth.getPosition()+sf::Vector2f{0, 1220});
    pauseButton.setTextFillColor(sf::Color(0x222222FF));
    pauseButton.setFillColor(sf::Color(0xDDDDDDFF));
    pauseButton.setOutlineColor(sf::Color(0x57595DFF));
    pauseButton.setOutlineThickness(4.0f);
    pauseButton.setHighlightFillColor(sf::Color(0xFFFFFFFF));
    pauseButton.setHighlightOutlineColor(sf::Color(0x000000FF));
    pauseButton.setHighlightOutlineThickness(4.0f);
    pauseButton.setTextOffset(-4.0f);
    pauseButton.setHoverCursor(hand_cursor);

    // setup help text box
    HelpTextBox htp(text_font_mono, TEXT_SIZE);
    components.push_back(&htp);
    Global::mouse_tracker.addClickableComponent(htp);
    htp.setPosition(cloth.getPosition()+sf::Vector2f{0, 1320});
    htp.setSize({1400, 250});
    htp.setText(sf::String("\
Instructions: Click sim view to use keyboard controls\n\
WASD:    Move camera\n\
Arrows:  Look around\n\
Space:   Float up\n\
Shift:   Float down\n\
"),sf::String("\
\n\
L-Click: Grab vertex\n\
R-Click: Tear vertices (hold)\n\
Q:       Pin grabbed vertex\n\
R:       Reset cam & sim\n\
"));
    htp.setTextFillColor(sf::Color(0x222222FF));
    htp.setFillColor(sf::Color(0x888888FF));
    htp.setOutlineColor(sf::Color(0x57595DFF));
    htp.setOutlineThickness(4.0f);
    htp.setTextOffset({20.0f,20.0f});

    // setup text box render texture
    sf::RenderTexture text_rend_tex;
    text_rend_tex.create(TEXT_REND_TEX_SIZE, TEXT_REND_TEX_SIZE);
    // setup text render item list
    ItemList text_rend_items(text_font, TEXT_SIZE);
    text_rend_items.setBackgroundFillColor(sf::Color(0));
    text_rend_items.setItemDimensions(sf::Vector2f{TEXT_REND_TEX_SIZE-2*TEXT_REND_TEX_PADDING, TEXT_SIZE});
    text_rend_items.setItemTextFillColor(sf::Color(0xFFFFFFFF));
    // setup other helpful vars for this process
    sf::String text_rend_prev_text;

    // set focused component to the cloth
    Global::mouse_tracker.setFocusedComponent(cloth);

    // setup frame clock
    sf::Clock clock;

    // main draw loop
    // ==============================
    while (window.isOpen())
    {
        // get frame time
        // ------------------------------
        float t = clock.restart().asSeconds();

        // event step
        // ------------------------------
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // global mouse tracker event handling
            // -----------------------------------
            if(Global::mouse_tracker.addEventHandler(window, event)) {
                history.pushSnapshot(*Global::mouse_tracker.getLastClicked());
            }

            // history event handling
            // ------------------------------
            history.addEventHandler(window, event);

            // component event handlers
            // ------------------------------
            for(auto it=components.begin(); it!=components.end(); ++it) {
                if((*it)->addEventHandler(window, event)) {
                    history.pushSnapshot(**it);
                }
            }
        }

        // button activations
        // ------------------------------
        if(pauseButton.clickedThisFrame()) {
            if(cloth.togglePause())
                pauseButton.setLabel("Resume");
            else
                pauseButton.setLabel("Pause");
        }

        // update step
        // ------------------------------
        for(size_t i=0; i<components.size(); i++) {
            if(i != cloth_component_index)
                components[i]->update(t);
        }

        // perform MenuBar actions - File
        // ------------------------------
        if(file_controller.wasActivated(Load)) {
            bool was_cloth_paused = cloth.isPaused();
            if(!was_cloth_paused)
                CLOTH_PAUSE
            nfdchar_t *out_path = nullptr;
            nfdresult_t nfd_result = NFD_OpenDialog("png,bmp,tga,jpg,jpeg,gif,psd,hdr,pic", nullptr, &out_path);
            if(nfd_result == NFD_OKAY) {
                if(!cloth.loadImgTexFromFile(out_path)) {
                    cerr << "Error: failed to load image file: " << out_path << endl;
                }
            }
            else if(nfd_result == NFD_ERROR) {
                cerr << "Error: " << NFD_GetError() << endl;
            }
            cloth.render_mesh();
            Global::mouse_tracker.setFocusedComponent(pauseButton);
            if(!was_cloth_paused)
                CLOTH_RESUME
        }
        if(file_controller.wasActivated(Save)) {
            bool was_cloth_paused = cloth.isPaused();
            if(!was_cloth_paused)
                CLOTH_PAUSE
            nfdchar_t *out_path = nullptr;
            nfdresult_t nfd_result = NFD_SaveDialog("png,bmp,tga,jpg,jpeg,gif,psd,hdr,pic", nullptr, &out_path);
            if(nfd_result == NFD_OKAY) {
                sf::Image save_img = cloth.getRenderedTexture().copyToImage();
                if(!save_img.saveToFile(out_path)) {
                    cerr << "Error: failed to save image file: " << out_path << endl;
                }
            }
            else if(nfd_result == NFD_ERROR) {
                cerr << "Error: " << NFD_GetError() << endl;
            }
            cloth.render_mesh();
            Global::mouse_tracker.setFocusedComponent(pauseButton);
            if(!was_cloth_paused)
                CLOTH_RESUME
        }
        // reset selection state
        file_controller.clearBitmask();

        // perform MenuBar actions - Edit
        // ------------------------------
        if(edit_controller.wasActivated(Undo))
            history.undo();
        if(edit_controller.wasActivated(Redo))
            history.redo();
        // reset selection state
        edit_controller.clearBitmask();

        // perform MenuBar actions - View
        // ------------------------------
        if(view_controller.wasActivated(ResetSim)) {
            cloth.restart();
        }
        if(view_controller.wasActivated(ResetGui)) {
            text_input.setTextString(sf::String());
            text_align.setSelectedItem(DropdownMenu::PLACEHOLDER_ID);
            font_select.setSelectedItem(DropdownMenu::PLACEHOLDER_ID);
            cloth_text_size.setIntValue(TEXT_REND_CHAR_SIZE);
            cloth_line_spacing.setFloatValue(TEXT_REND_CHAR_SIZE);
            cloth_text_color.setHexValue(TEXT_REND_TEXT_COLOR);
            cloth_color.setHexValue(0xFFFFFFFF);
            fpa_select.setSelectedItem(DropdownMenu::PLACEHOLDER_ID);
            cloth_vertices.setIntValue(INIT_CLOTH_VERTICES);
            cloth_scale.setFloatValue(INIT_CLOTH_SCALE);
            gravity_x.setFloatValue(INIT_GRAVITY.x);
            gravity_y.setFloatValue(INIT_GRAVITY.y);
            gravity_z.setFloatValue(INIT_GRAVITY.z);
            light_dir_x.setFloatValue(INIT_LIGHT_DIR.x);
            light_dir_y.setFloatValue(INIT_LIGHT_DIR.y);
            light_dir_z.setFloatValue(INIT_LIGHT_DIR.z);
            wind_force_x.setFloatValue(INIT_WIND_FORCE.x);
            wind_force_y.setFloatValue(INIT_WIND_FORCE.y);
            wind_force_z.setFloatValue(INIT_WIND_FORCE.z);
            history.clear();
        }
        // reset selection state
        view_controller.clearBitmask();


        // perform UI synchronization with cloth viewer
        // --------------------------------------------
        text_rend_items.setItemLabelAlignment((Item::LabelAlignment)
            (text_align.hasSelection() ? text_align.getSelectedItemID()&0xf : 0));
        text_rend_items.setItemFont(*fonts[font_select.hasSelection() ? font_select.getSelectedItemID() : 0]);
        text_rend_items.setItemCharacterSize(cloth_text_size.getIntValue());
        text_rend_items.setItemDimensions({text_rend_items.getItemDimensions().x, cloth_line_spacing.getFloatValue()});
        text_rend_items.setItemTextFillColor(sf::Color(cloth_text_color.getUintValue()));
        cloth.set_image_color(sf::Color(cloth_color.getUintValue()));
        cloth.set_scale(cloth_scale.getFloatValue());
        cloth.set_light_dir({light_dir_x.getFloatValue(), light_dir_y.getFloatValue(), light_dir_z.getFloatValue()});
        cloth.set_gravity({gravity_x.getFloatValue(), gravity_y.getFloatValue(), gravity_z.getFloatValue()});
        cloth.set_wind_force({wind_force_x.getFloatValue(), wind_force_y.getFloatValue(), wind_force_z.getFloatValue()});
        cloth.set_n_vertices(cloth_vertices.getIntValue());
        cloth.set_fixed_point_arrangement((Cloth::FixedPointArrangement)
            (fpa_select.hasSelection() ? fpa_select.getSelectedItemID() : 0));

        // generate text render texture
        // ------------------------------
        text_rend_tex.clear(sf::Color(0));
        if(text_rend_prev_text != text_input.getText().toString()) {

            // reset and fill text_rend_items with each line of the input text as an element
            // -----------------------------------------------------------------------------
            text_rend_items.clearItems();
            sf::String curr_item_label;
            for(auto it=text_input.getText().begin(); it!=text_input.getText().end(); ++it) {
                if(it->getChar() != '\n')
                    curr_item_label += sf::String(it->getChar());
                else {
                    text_rend_items.addItem(curr_item_label);
                    curr_item_label.clear();
                }
            }
            // flush last hanging line
            if(!curr_item_label.isEmpty()) 
                text_rend_items.addItem(curr_item_label);

            // perform vertical text alignment
            // --------------------------------------------------
            float items_height = text_rend_items.getClickableBounds().height;
            // default -- top vertical alignment  (y position = padding)
            float items_y = TEXT_REND_TEX_PADDING;
            if(text_align.hasSelection()) {
                // centered vertical alignment
                if(text_align.getSelectedItemID() & 0x10)
                    items_y = (TEXT_REND_TEX_SIZE-2*TEXT_REND_TEX_PADDING-items_height)/2.0f + TEXT_REND_TEX_PADDING;
                // bottom vertical alignment
                else if(text_align.getSelectedItemID() & 0x20)
                    items_y = TEXT_REND_TEX_SIZE-TEXT_REND_TEX_PADDING-items_height;
            }
            text_rend_items.setPosition({TEXT_REND_TEX_PADDING, items_y});

            // draw text to render texture
            // ------------------------------
            text_rend_tex.draw(text_rend_items);
            text_rend_tex.display();
        }

        // update cloth
        // ------------------------------
        cloth.set_text_texture(text_rend_tex.getTexture());
        cloth.update(t);

        // mouse tracker update (must be after component updates)
        // ------------------------------------------------------
        Global::mouse_tracker.update(t);

        // draw components 
        // ------------------------------
        window.clear(sf::Color(0xBBBBBBFF));
        for(auto it=components.begin(); it!=components.end(); ++it) {
            // skip _last_clicked
            if(*it != Global::mouse_tracker.getLastClicked())
                window.draw(**it);
        }
        // draw focused component last
        if(Global::mouse_tracker.getLastClicked() != nullptr) {
            window.draw(*Global::mouse_tracker.getLastClicked());
        }
        window.display();
    }

    return 0;
}