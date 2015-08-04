#ifndef CAMERA_H
#define	CAMERA_H

#include <SFML/Graphics.hpp>
#include "entity.h"
#include "handler.h"

struct Camera
{
    sf::RenderWindow * window;
    sf::View * view;
    Level * level;
    GameObj * followObj;

    sf::Vector2f baseZoom;
    int minZoom;
    int zoom;
    int maxZoom;
    float zoomFactor;

    sf::Vector2f velocity;
    sf::Vector2f acceleration;

    //=====Functions

    Camera( sf::RenderWindow * win, sf::View * v, Level * l = NULL ) :
        view(v), level( l ), followObj( NULL ), window( win ),
        minZoom( 0 ), zoom( 10 ), maxZoom( 20 ), zoomFactor( 0.05 ),
        baseZoom( view->getSize() ),
        velocity( sf::Vector2f( 0, 0 ) ), acceleration( sf::Vector2f( 0, 0 ) ) {}

    sf::Vector2f convertCoords( int x, int y );
    void resetZoom();
    void setZoom( int z ); //pos = in, neg = out

    void update();
};

#endif	/* CAMERA_H */
