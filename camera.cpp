#include "camera.h"
#include <iostream>

sf::Vector2f Camera::convertCoords( int x, int y )
{
    sf::Vector2f coord( x, y );

    sf::FloatRect display = view->getViewport();

    sf::Vector2f topleftCorner = view->getCenter();
    topleftCorner.x -= view->getSize().x / 2;
    topleftCorner.y -= view->getSize().y / 2;

    coord.x = ( ( ( coord.x - display.left * window->getSize().x ) / display.width  )
                * ( view->getSize().x / window->getSize().x ) ) + topleftCorner.x;
    coord.y = ( ( ( coord.y - display.top * window->getSize().y  ) / display.height )
                * ( view->getSize().y / window->getSize().y ) ) + topleftCorner.y;

    return coord;
}
void Camera::resetZoom()
{
    sf::Vector2f center = view->getCenter();
    view->reset( sf::FloatRect( 0, 0, baseZoom.x, baseZoom.y ) );
    view->setCenter( center );

    zoom = ( maxZoom - minZoom ) / 2;
}
void Camera::setZoom( int z )
{
    float zoomFact;

    if( z > 0 )
    {
        zoomFact = 1 - zoomFactor;
    }
    else
    {
        zoomFact = 1 + zoomFactor;
    }


    for( int n = 0; n < abs(z); n++ )
    {
        view->zoom( zoomFact );
    }
}

void Camera::update()
{
    if( followObj != NULL )
    {
        Tile * t = &level->tiles[followObj->y][followObj->x];

        view->setCenter( sf::Vector2f( t->x, t->y ) );
    }


    if( acceleration.x != 0 )
    {
        velocity.x /= 1.25;
        velocity.x += acceleration.x;
    }
    else if ( velocity.x != 0 )
    {
        velocity.x /= 4;
        if( velocity.x < 1)
        {
            velocity.x = 0;
        }
    }

    if( acceleration.y != 0 )
    {
        velocity.y /= 1.25;
        velocity.y += acceleration.y;
    }
    else if ( velocity.y != 0 )
    {
        velocity.y /= 4;
        if( velocity.y < 1)
        {
            velocity.y = 0;
        }
    }

    view->move( velocity );

}
