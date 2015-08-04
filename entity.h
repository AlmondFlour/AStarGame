#ifndef ENTITY_H
#define	ENTITY_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <deque>
#include "animation.h"
#include "handler.h"

#ifndef LEVEL_COMPILED
    class GameObj;
    struct Tile;
    struct TerrainType;
    struct UnitStats;
    struct Ability;
    class Level;
#endif

enum Target{ SINGLE = 0, AREA = 1, NO_TARGET = 2 };


//used for movement with entities and for terrain in tiles
struct TerrainType
{
    Level * level;

    Anim anim;

    bool walkable;
    bool aquatic;
    bool flying;

    int cost;
    int currentTerrain;

    //Constructor
    TerrainType() : walkable(true), aquatic(false), flying(false), cost(1), currentTerrain(0) {}
    //Copy Constructor
    TerrainType( const TerrainType &other) : walkable(other.walkable),
                                             aquatic(other.aquatic),
                                             flying(other.flying),
                                             cost(other.cost),
                                             currentTerrain(other.currentTerrain),
                                             anim( other.anim ){};

    bool isWalkable( TerrainType &destination );
    void operator=(const TerrainType &newTerrain);
};

//Holds stats for gameObjs
struct UnitStats
{
    GameObj * owner;

    Anim anim;

    std::string name;
    std::string description;

    int health;
    int maxHealth;
    float moveSpeed;
    TerrainType movement;

    std::vector< Ability > abilities;

    UnitStats() : anim( NULL, NULL, sf::IntRect( 0, 0, 75, 75 ), 75, 75 )
                , movement(), owner(NULL), name("No Name"), description("No description"){}

    UnitStats( const Anim & a, TerrainType moveType ):
    health(1), maxHealth(1), moveSpeed(1),
    movement(), anim( a ), owner(NULL){}

    UnitStats( const UnitStats &other ) :
        health( other.health ), maxHealth( other.maxHealth ),
        moveSpeed(1), movement( other.movement), anim( other.anim ),
        abilities( other.abilities ), owner( other.owner),
        name( other.name ), description( other.description ){}

    void damage( int d );
    void addAbility( const Ability & a );
};

//represents any entity in the game
class GameObj
{
    public:
        Level * level;

        //current path
        std::deque< Tile*> path;

        //Stats
        UnitStats stats;

        //size of a tile when drawing
        int scale;

        //grid dimensions (x and y are top left)
        int x, y, w, h;

        //=====constructors/destructors
        GameObj( Level * lev, UnitStats & stat, int width, int height, int s );

        GameObj() : x(0), y(0), w(1), h(1), scale(1), level(NULL) {}

        //=====Graphical Interactions
        void draw();

        //=====game interactions
        void move( int x, int y);
        void setPosition( int x, int y );
        void setDimensions( int w, int h );
        void setMovementType( TerrainType & t);
        void update();  //updates path and position
};

#define GAMEOBJ_COMPILED

//holds GameObj, used to make a grid
struct Tile
{
    std::vector< GameObj * > entities;

    //Position
    int x, y;

    //Tile stats
    TerrainType terrain;

    //Constructor
    Tile() : x(0), y(0), cost(1), hcost(1), parent(NULL) {}

    Tile( const Tile & other ) : x(other.x), y(other.y),
        terrain(other.terrain), cost(0), hcost(0), parent(NULL){}

    //Stuff for AStar (reset after it's run)
    int cost;       //cost to move to node
    int hcost;      //weighted cost
    Tile * parent;  //Node that led here

    Tile operator=(const Tile &newTile){ std::cout << "LSLKS\n";}

    //Graphics
    void draw();
    void drawTerrain();

    //Interactions

    void addEnt( GameObj *);
    void removeEnt(GameObj *);
    void setTerrain( TerrainType t, int scaleX , int scaleY );
};

//base class ( might use Unions? )
struct Ability
{
    GameObj * owner;

    int damage;
    int cost;
    int range;

    Target targetType;
    Effect effect;

    Ability( Effect e = Effect( Anim(), sf::milliseconds( 100 ) ), GameObj * own = NULL ) :
        damage( 0 ), cost( 0 ), range( 1 ), targetType( NO_TARGET ),
        effect( e ), owner( own ){}

    Ability( const Ability & other ) :
        damage( other.damage ), cost(other.cost), range( other.range ),
        targetType( other.targetType), effect( other.effect), owner(other.owner) {}

    virtual bool cast( int x, int y );
    virtual bool cast( GameObj * targ );
    virtual bool cast();
};

#endif	/* ENTITY_H */
