#ifndef MENU_H
#define	MENU_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "animation.h"
#include "camera.h"


struct MenuItem
{
    //Anim anim;
    sf::RectangleShape anim;

    sf::RenderWindow * window;
    sf::Text text;
    sf::Color defaultColor;
    sf::Color toggledColor;
    sf::IntRect boundaries;
    bool toggled;

    MenuItem() : toggled(false) {}


    MenuItem (const MenuItem &other) : window( other.window ),
                                       anim(other.anim),
                                       text(other.text),
                                       defaultColor( other.defaultColor),
                                       toggledColor( other.toggledColor),
                                       boundaries( other.boundaries),
                                       toggled( other.toggled ) {};

    void setup( sf::RenderWindow * win, sf::IntRect bound, sf::Font * f,
                 std::string str = "",
                 sf::Color c1 = sf::Color( 166, 100, 75, 255),
                 sf::Color c2 = sf::Color( 65, 119, 26, 255 ));

    void setWindow( sf::RenderWindow * win );
    void setColor( sf::Color c1, sf::Color c2 );
    //void setTexture( sf::Texture t );
    void setText( std::string str );
    void setPosition( int x, int y );
    void toggle();
    void draw();
};

class Menu
{
    private:
        sf::RenderWindow * window;
        sf::Font * font;
        Camera * camera;

        sf::RectangleShape background;
        std::vector< MenuItem > menuItems;
        MenuItem baseItem; //used to create new menu items

        int inputItem; //notes which item is the active input item, -1 if none
        std::string input;
        int inputPos;  //notes the current text entry position

    public:

        Menu( sf::RenderWindow * win, Camera * cam, sf::Font * f,
                sf::Color bgColor = sf::Color( 30, 75, 66, 255 ),
                sf::Color itemColor1 = sf::Color( 166, 100, 75, 255 ),
                sf::Color itemColor2 = sf::Color( 65, 119, 26, 255 ) );

        void addItem( std::string text );
        int checkClicked( float x, float y );
        void setFont( sf::Font * f );
        void setItemText( int item, std::string text );
        void setInputItem( int item );
        void setPosition( int x, int y);
        //void setBackgroundDimensions( sf::IntRect dim );
        void setButtonDimensions( sf::IntRect dim ); //sets first item
        void inputText( char c );
        std::string getInput();
        bool enteringText();

        void draw();
        void setBackgroundColor( sf::Color c1 );
        void setBackgroundTexture( sf::Texture * text );
        void setItemTexture( sf::Texture * text );
        void setItemColor( sf::Color c1, sf::Color c2);


};

#endif	/* MENU_H */
