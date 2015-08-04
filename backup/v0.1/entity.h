#ifndef ENTITY_H
#define	ENTITY_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <deque>
#include "animation.h"


class GameObj;
struct Tile;
struct TerrainType;
struct Ability;
struct UnitStats;

#include "units.h"

//used for movement with entities and for terrain in tiles
struct TerrainType
{
    bool walkable;
    bool aquatic;
    bool flying;

    int cost;
    int currentTerrain;

    //Constructor
    TerrainType() : walkable(true), aquatic(false), flying(true), cost(1), currentTerrain(0) {}
    //Copy Constructor
    TerrainType( const TerrainType &other) : walkable(other.walkable),
                                             aquatic(other.aquatic),
                                             flying(other.flying),
                                             cost(other.cost),
                                             currentTerrain(other.currentTerrain){};

    bool isWalkable( TerrainType &destination );
    TerrainType operator=(const TerrainType &newTerrain);
};

//represents any entity in the game
class GameObj
{
    public:
        //current path
        std::deque< Tile*> path;

        //Stats
        UnitStats stats;

        //size of a tile when drawing
        int scale;

        //grid dimensions (x and y are top left)
        int x, y, w, h;

        //=====constructors/destructors
        GameObj( sf::RenderWindow * win, sf::Texture * ss, int w, int h, int scale );
        GameObj();

        //=====Graphical Interactions
        void draw();
        void setWindow( sf::RenderWindow * win );
        void setSprite( sf::Texture * ss );
        void setDimensions( int w, int h );
        void setScale( int s );
        void setFrameDimensions( int w, int h);
        void setFrame( int fx, int fy );

        //=====game interactions
        void move( int x, int y);
        void setPosition( int x, int y );
        void setMovementType( TerrainType t);
        void update();  //updates path and position
};

//holds GameObj, used to make a grid
struct Tile
{
    std::vector< GameObj * > entities;

    //Position
    int x, y;

    Anim background;

    //Tile stats
    TerrainType terrain;

    //Constructor
    Tile() : x(0), y(0)
    {
        background.initilize( NULL, NULL, sf::IntRect( 0, 0, 75, 75), 1, 1 );
    }

    //Stuff for AStar (reset after it's run)
    int cost;       //cost to move to node
    int hcost;      //weighted cost
    Tile * parent;  //Node that led here

    //Graphics
    void draw();
    void drawTerrain();
    void setBackground( sf::Texture * t, int scale );

    //Interactions
    void addEnt( GameObj *);
    void removeEnt(GameObj *);
    void setTerrain( TerrainType &t  );
};
#endif	/* ENTITY_H */
