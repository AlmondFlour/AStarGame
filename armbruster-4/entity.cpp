#include "entity.h"
#include <cmath>
#include <iostream>

/*=================
    Animator
=================*/
//sets the current texture frame
//takes in the row and column
void Animator::setFrame(int fx, int fy)
{
    frame.left = fx * frame.width;
    frame.top = fy * frame.height;

    shape.setTextureRect( frame );
}
void Animator::draw()
{
    window->draw( shape );
}


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

    anim.window = win;
    anim.spritesheet = ss;

    anim.frame.width = w * scale;
    anim.frame.height = h * scale;
    anim.frame.left = 0;
    anim.frame.top = 0;

    anim.shape.setSize( sf::Vector2f( w * scale, h * scale) );
    anim.shape.setTexture( ss );
    anim.shape.setTextureRect( anim.frame );
}
GameObj::GameObj()
{
    x = 0;
    y = 0;
    w = 1;
    h = 1;

    anim.window = NULL;
    anim.spritesheet = NULL;

    anim.frame.width = 0;
    anim.frame.height = 0;
    anim.frame.left = 0;
    anim.frame.top = 0;

    scale = 1;

    anim.shape.setSize( sf::Vector2f( 0, 0 ) );
    //anim.shape.setTexture( anim.spritesheet );
    anim.shape.setTextureRect( anim.frame );
}

/*=================
    Graphics
=================*/


void GameObj::setWindow( sf::RenderWindow * win )
{
    anim.window = win;
}
void GameObj::setSprite( sf::Texture * ss )
{
    anim.spritesheet = ss;
    anim.shape.setTexture( ss );
}
void GameObj::draw()
{
    anim.draw();
}
void GameObj::setFrameDimensions( int w, int h )
{   //Set the clipping area of the texture
    anim.frame.width = w;
    anim.frame.height = h;

    anim.shape.setTextureRect( anim.frame );
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

    anim.shape.move( x * scale, y * scale );
}
void GameObj::setPosition( int x, int y)
{
    this->x = x;
    this->y = y;

    anim.shape.setPosition( x * scale, y * scale );
}
void GameObj::setDimensions( int w, int h )
{
    this->w = w;
    this->h = h;

    //graphics stuff

    anim.frame.width = w * scale;
    anim.frame.height = h * scale;

    anim.shape.setSize( sf::Vector2f( anim.frame.width, anim.frame.height ) );
    anim.shape.setTextureRect( anim.frame );
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
    background.spritesheet = t;
    background.frame.height = 75;
    background.frame.width = 75;
    background.frame.top = 0;
    background.frame.left = 0;

    background.shape.setSize( sf::Vector2f(scale,scale));
    background.shape.setPosition( x, y );
    background.shape.setTexture( t );
    background.shape.setTextureRect( background.frame );
}

void Tile::setWalkable( bool w )
{
    if( w )
    {
        walkable = true;
        background.setFrame( 0, 0 );
    }
    else
    {
        walkable = false;
        background.setFrame( 1, 0);
    }

}
void Tile::toggleWalkable()
{
    walkable = !walkable;

    background.setFrame( 1 * !walkable, 0 );
}
