#ifndef ENTITY_H
#define	ENTITY_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <deque>

struct Animator;
class GameObj;
struct Tile;

//holds things needed for graphical representation in SFML
struct Animator
{
    sf::RenderWindow * window;
    sf::Texture * spritesheet;
    sf::IntRect frame;          //clipping information for spritesheet
    sf::RectangleShape shape;   //dimensions on window

    void setFrame(int fx, int fy);  //(1, 0) would be the 2nd frame
    void draw();


};

//represents any entity in the game
class GameObj
{
    public:
        //current path
        std::deque< Tile*> path;

        //scale factor from grid to window
        int scale;

        //graphical representation
        Animator anim;

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
        void update();  //updates path and position
};

//holds GameObj, used to make a grid
struct Tile
{
    std::vector< GameObj * > entities;

    //Position
    int x, y;

    Animator background;

    //Tile stats
    bool walkable;

    //Constructor
    Tile() : walkable(true), x(0), y(0){}

    //Stuff for AStar (reset after it's run)
    int cost;       //cost to move to node
    int hcost;      //weighted cost
    Tile * parent;  //Node that led here

    //Graphics
    void draw();
    void setBackground( sf::Texture * t, int scale );

    //Interactions
    void addEnt( GameObj *);
    void removeEnt(GameObj *);
    void setWalkable( bool w );
    void toggleWalkable();
};


#endif	/* ENTITY_H */
