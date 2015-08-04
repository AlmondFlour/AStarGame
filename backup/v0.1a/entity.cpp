#include "entity.h"
#include <cmath>
#include <iostream>


/*=================
    GameObj
=================*/

GameObj::GameObj( Level * lev, UnitStats & stat, int width, int height, int s )
         : level(lev), stats( stat), x(0), y(0), w( width), h(height), scale(s)
{
    stats.owner = this;
}

void GameObj::draw()
{
    stats.anim.draw();
}

/*=================
    Game Interactions
=================*/

void GameObj::move( int x, int y)
{
    this->x += x;
    this->y += y;

    stats.anim.setPosition( x * scale , y * scale );
}
void GameObj::setPosition( int x, int y)
{
    this->x = x;
    this->y = y;

    stats.anim.setPosition( x * scale, y * scale );
}
void GameObj::setDimensions( int w, int h )
{
    this->w = w;
    this->h = h;

    //graphics stuff

    stats.anim.setDimensions( w * scale, h * scale );
}
void GameObj::setMovementType( TerrainType & t)
{
    stats.movement = t;
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
    terrain.anim.draw();

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
    terrain.anim.draw();
}
void Tile::addEnt( GameObj * ent)
{
    entities.push_back( ent );
}
void Tile::removeEnt( GameObj * ent )
{
    entities.erase( remove( entities.begin(), entities.end(), ent), entities.end() );
}
void Tile::setTerrain( TerrainType t, int scaleX, int scaleY )
{
    terrain = t;
    terrain.anim.setPosition( x * scaleX, y * scaleY );
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

void TerrainType::operator=(const TerrainType &newTerrain)
{
    anim = newTerrain.anim;
    cost = newTerrain.cost;
    walkable = newTerrain.walkable;
    aquatic = newTerrain.aquatic;
    flying = newTerrain.flying;
    currentTerrain = newTerrain.currentTerrain;
}

/*=================
    Unit Stats
=================*/
void UnitStats::addAbility( const Ability & a )
{
    abilities.push_back( a );
    abilities[abilities.size() - 1].owner = owner;
}

void UnitStats::damage( int d )
{
    health -= d;

    if( health <= 0 )
    {
        //kill unit
    }
}

/*=================
    Ability
=================*/
bool Ability::cast()
{
    if( owner == NULL )
    {
        return false;
    }

    owner->level->addEffect( &effect );
}

bool Ability::cast( GameObj * targ )
{
    if( owner == NULL )
    {
        return false;
    }


    owner->level->addEffect( &effect, targ->x, targ->y );
}

bool Ability::cast( int x, int y )
{
    if( owner == NULL )
    {
        return false;
    }

    if( range >= abs( owner->x - x) + abs( owner->y - y) )
        owner->level->addEffect( &effect, x, y );
}
