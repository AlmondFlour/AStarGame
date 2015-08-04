#include "entity.h"
#include <cmath>
#include <iostream>


/*=================
    GameObj
=================*/

//Constructor
GameObj::GameObj( sf::RenderWindow * win, sf::Texture * ss, int w, int h, int scale)
{
    x = 0;
    y = 0;
    this->w = w;
    this->h = h;

    this->scale = scale;

    anim.initilize( win, ss, sf::IntRect( 0, 0, 75, 75), scale, scale );

    movement.walkable = true;
    movement.aquatic = false;
    movement.flying = false;


}
GameObj::GameObj()
{
    x = 0;
    y = 0;
    w = 1;
    h = 1;

    movement.walkable = true;
    movement.aquatic = false;
    movement.flying = false;



    scale = 1;
    anim.initilize( NULL, NULL, sf::IntRect( 0, 0, 75, 75), scale, scale );
}

/*=================
    Graphics
=================*/


void GameObj::setWindow( sf::RenderWindow * win )
{
    anim.setWindow(win);
}
void GameObj::setSprite( sf::Texture * ss )
{
    anim.setTexture(ss);
}
void GameObj::draw()
{
    anim.draw();
}
void GameObj::setFrameDimensions( int w, int h )
{   //Set the clipping area of the texture
    anim.setClipDimensions( w, h );
}
void GameObj::setFrame( int fx, int fy )
{   //x and y are the frame number, not coordinate
    anim.setFrame( fx, fy );
}

/*=================
    Game Interactions
=================*/

void GameObj::move( int x, int y)
{
    this->x += x;
    this->y += y;

    anim.setPosition( x * scale , y * scale );
}
void GameObj::setPosition( int x, int y)
{
    this->x = x;
    this->y = y;

    anim.setPosition( x * scale, y * scale );
}
void GameObj::setDimensions( int w, int h )
{
    this->w = w;
    this->h = h;

    //graphics stuff

    anim.setDimensions( w * scale, h * scale );
}
void GameObj::setMovementType( TerrainType t)
{
    movement = t;
}
void GameObj::update()
{
    //check if there is a path
    if( path.empty() )
        return;

    //update position and path
    move( path.front()->x, path.front()->y );
    path.pop_front();

    //update animation
}

/*=================
    Tile
=================*/
void Tile::draw()
{
    //draw tile background
    background.draw();

    //choose which objects to draw, if any
    for( int n = 0; n < entities.size(); n++ )
    {
        //std::cout << x << ", " << y << "\n";
        //if it starts on this tile
        if( ( x == entities[n]->x ) && (y == entities[n]->y) )
        {
            entities[n]->draw();
        }
    }
}
void Tile::drawTerrain()
{
    background.draw();
}
void Tile::addEnt( GameObj * ent)
{
    entities.push_back( ent );
}
void Tile::removeEnt( GameObj * ent )
{
    entities.erase( remove( entities.begin(), entities.end(), ent), entities.end() );
}
void Tile::setBackground( sf::Texture * t, int scale )
{
    background.setTexture(t);
    background.setClipDimensions( 75, 75 );

    background.setDimensions( scale, scale );
    background.setPosition( x, y );
}
void Tile::setTerrain( TerrainType &t )
{
    terrain = t;
    background.setFrame( t.currentTerrain, 0 );
}

/*=================
    Terrain Type
=================*/
bool TerrainType::isWalkable( TerrainType &destination )
{
    if(    ( walkable && destination.walkable )
        || ( aquatic && destination.aquatic)
        || ( flying && destination.flying ) )
    {
        return true;
    }

    return false;
}

TerrainType TerrainType::operator=(const TerrainType &newTerrain)
{
    cost = newTerrain.cost;
    walkable = newTerrain.walkable;
    aquatic = newTerrain.aquatic;
    flying = newTerrain.flying;
    currentTerrain = newTerrain.currentTerrain;
}
