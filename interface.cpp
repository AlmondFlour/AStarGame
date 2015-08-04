#include "interface.h"
#include <sstream>

GameInterface::GameInterface( Camera * cam, sf::Font * f )
    :   camera( cam ), font( f ), healthText( sf::Text( "\?\?\?/\?\?\?", *f, 15 )),
        background( sf::Vector2f( 794, 200 )), displayObj( NULL ), nameText( "\?\?\?", *f, 15 ),
        healthBar( sf::Vector2f( 100, 20)), maxHealthBar( sf::Vector2f( 100, 20 ) )//,
        //portrait( sf::Vector2f( 94, 154 ))
{
    portrait.setWindow( camera->window );
    portrait.setDimensions( 94, 154 );
    portrait.setTexture( NULL, true );

    background.setFillColor( sf::Color( 75, 75, 100, 255 ) );
    healthBar.setFillColor( sf::Color( 50, 230, 50, 255 ) );
    maxHealthBar.setFillColor( sf::Color( 230, 50, 50, 255 ) );
    portrait.setColor( sf::Color::Transparent);

    background.setOutlineColor( sf::Color( 25, 25, 25, 255 ) );
    background.setOutlineThickness( 5 );
    portrait.setOutlineThickness( 3 );
    portrait.setOutlineColor( sf::Color( 50, 66, 22 ) );

    background.setPosition( 3, 3 );
    nameText.setPosition( 6, 6 );
    healthBar.setPosition( 3, 180 );
    maxHealthBar.setPosition( 3, 180 );
    portrait.setPosition( 6, 26 );
    healthText.setPosition( 20, 180);
}

void GameInterface::selectUnit( GameObj * obj )
{
    displayObj = obj;

    if( obj == NULL )
    {
        portrait.setTexture( NULL );
        portrait.setColor( sf::Color::Transparent);
        healthText.setString( "\?\?\?/\?\?\?" );
        nameText.setString( "\?\?\?" );
        healthBar.setSize( sf::Vector2f( maxHealthBar.getSize().x, maxHealthBar.getSize().y ) );
        return;
    }

    std::ostringstream healthDisplay;
    healthDisplay << obj->stats.health << "/" << obj->stats.maxHealth;
    healthText.setString( healthDisplay.str() );
    nameText.setString( obj->stats.name );

    if( obj->stats.maxHealth == 0 )
    {
        healthBar.setSize( sf::Vector2f( 1, maxHealthBar.getSize().y ));
    }
    else
    {
        healthBar.setSize( sf::Vector2f(
            ( (float)obj->stats.health / obj->stats.maxHealth ) * maxHealthBar.getSize().x, maxHealthBar.getSize().y ) );
    }

    portrait.setTexture( obj->stats.anim.getTexture() );
    portrait.setClipDimensions( obj->stats.anim.getClipDimensions().width, obj->stats.anim.getClipDimensions().height);
    portrait.setColor( obj->stats.anim.getFillColor() );
}


void GameInterface::update()
{
    if( displayObj == NULL )
    {
        return;
    }

    std::ostringstream healthDisplay;
    healthDisplay << displayObj->stats.health << "/" << displayObj->stats.maxHealth;
    healthText.setString( healthDisplay.str() );
}
void GameInterface::draw()
{
    sf::View originalView = camera->window->getView();
    camera->window->setView( *camera->view);

    camera->window->draw( background );
    //camera->window->draw( portrait );
    portrait.draw();
    camera->window->draw( maxHealthBar );
    camera->window->draw( healthBar );
    camera->window->draw( nameText );
    camera->window->draw( healthText );

    camera->window->setView( originalView );
}

