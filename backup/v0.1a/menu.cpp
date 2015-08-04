#include "menu.h"
#include <iostream>

/*=================
    MenuItem
=================*/

void MenuItem::setup( sf::RenderWindow * win, sf::IntRect bound, sf::Font * f,
         std::string str, sf::Color c1, sf::Color c2)
 {
     window = win;

     toggled = false;

     boundaries = bound;
     text.setString(str );

     defaultColor = c1;
     toggledColor = c2;

     anim.setFillColor( c1);
     anim.setPosition( bound.left, bound.top );
     anim.setSize( sf::Vector2f( bound.width, bound.height));
     anim.setOutlineColor( sf::Color( 0, 0, 0, 255 ) );
     anim.setOutlineThickness( 3 );

     text.setFont( *f);
     text.setString( str );
     text.setPosition( bound.left + ( bound.width / 4 ) , bound.top + ( bound.height / 4 ) );
 }

void MenuItem::setWindow( sf::RenderWindow * win )
{
    window = win;
    //anim.setWindow( win);
}
void MenuItem::setColor( sf::Color c1, sf::Color c2 )
{
    defaultColor = c1;
    toggledColor = c2;

    if( toggled )
    {
        anim.setFillColor( toggledColor);
    }
    else
    {
        anim.setFillColor( defaultColor);
    }
}
void MenuItem::setText( std::string str )
{
    text.setString( str );
}
void MenuItem::setPosition( int x, int y )
{
    boundaries.left = x;
    boundaries.top = y;
    anim.setPosition( x, y );
    text.setPosition( x + ( boundaries.width / 4 ) , y + ( boundaries.height / 4 ) );

}
void MenuItem::toggle()
{
    toggled = !toggled;
    if( toggled )
    {
        anim.setFillColor( toggledColor );
    }
    else
    {
        anim.setFillColor( defaultColor );
    }
}
void MenuItem::draw()
{
    window->draw( anim );
    window->draw( text );
}

/*=================
    Menu
=================*/

Menu::Menu( sf::RenderWindow * win, Camera * cam, sf::Font * f,
           sf::Color bgColor, sf::Color itemColor1, sf::Color itemColor2 )
{
    window = win;
    camera = cam;

    sf::IntRect itemDim;
    itemDim.left += camera->view->getSize().x / 3;
    itemDim.top += camera->view->getSize().y / 4;
    itemDim.width = camera->view->getSize().x / 3;
    itemDim.height = camera->view->getSize().y / 8;

    baseItem.setup( win, itemDim, f, "", itemColor1, itemColor2 );

    font = f;

    //background.setWindow( win );
    background.setPosition( 0, 0 );
    background.setSize( sf::Vector2f( camera->view->getSize().x, camera->view->getSize().y ));
    background.setFillColor( bgColor );
    background.setOutlineColor( sf::Color( 15, 37, 33, 255) );
    background.setOutlineThickness( 5 );
    //background.setDimensions( bgDim.width, bgDim.height );

    inputItem = -1;
    input = "";
    inputPos = 0;
}


int Menu::checkClicked( float x, float y )
{
    sf::FloatRect display = camera->view->getViewport();

    sf::Vector2f adjustedCoords = camera->convertCoords( x, y );

    if( menuItems.empty() )
    {
        return -1;
    }

    for( int n = 0; n < menuItems.size(); n++ )
    {
        if( menuItems[n].boundaries.contains( adjustedCoords.x, adjustedCoords.y ))
        {
            return n;
        }
    }

    return -1;
}
void Menu::setFont( sf::Font * f )
{
    font = f;

    for( int n = 0; n < menuItems.size(); n++)
    {
        menuItems[n].text.setFont( *f );
    }

}
void Menu::addItem( std::string text )
{
    MenuItem newItem = baseItem;
    newItem.setPosition( newItem.boundaries.left, newItem.boundaries.top
                        + ( ( newItem.boundaries.height + 25 ) * menuItems.size() ) ) ;
    newItem.setText( text );

    menuItems.push_back( newItem );
}
void Menu::setItemText( int item, std::string text )
{
    menuItems[item].setText( text );
}
void Menu::setInputItem( int item )
{
    if( ( item >= 0) && (item <= menuItems.size() )  || ( item == -1 ) )
    {
        inputPos = 0;
        inputItem = item;
        input = "";
    }

}
void Menu::inputText( char c )
{
    // Handle ASCII characters only
    if ( c < 128)
    {
        //if delete
        if( c == 8 )
        {
            if( !input.empty() )
            {
                input.erase( input.size() - 1 );
                menuItems[ inputItem ].setText( input );
            }
        }
        else
        {
            input +=  c;
            menuItems[ inputItem ].setText( input );
        }


    }
}
std::string Menu::getInput()
{
    return input;
}
bool Menu::enteringText()
{
    return ( ( inputItem >= 0 ) && ( inputItem <= menuItems.size() ) );

}

void Menu::setBackgroundColor( sf::Color c )
{
    background.setFillColor( c );
}
void Menu::setItemColor( sf::Color c1, sf::Color c2 )
{
    baseItem.setColor( c1, c2 );

    for( int n = 0; n < menuItems.size(); n++)
    {
        menuItems[n].setColor( c1, c2 );
    }
}
void Menu::draw()
{
    window->setView( *camera->view );
    window->draw( background);

    for( int n = 0; n < menuItems.size(); n++ )
    {
        menuItems[n].draw();
    }

    window->setView( window->getDefaultView() );
}
