#ifndef HANDLER_H
#define	HANDLER_H

#include <vector>
#include <algorithm>
#include "entity.h"

class Level
{
    private:
        sf::RenderWindow * window;
        int scale;
        int w, h;

    public:
        bool changesMade;

        std::vector< std::vector< Tile > > tiles;
        std::vector< GameObj* > entities;

        Level( sf::RenderWindow * win, int w, int h, int scale, sf::Texture * bg);

        void addEnt( GameObj * ent );
        void removeEnt( GameObj * ent );
        void moveEnt( GameObj * ent, int x, int y );
        void setEntPos( GameObj * ent, int x, int y );
        void setEntPath( GameObj * ent, int x, int y);

        void update();
        void updatePath( GameObj * obj);
        void updateAllPaths();
        void aStar( GameObj*, Tile * dest );

        void draw();

        //test functions
        void randomizeAll();
        void randomizeTerrain( double prob );
        void moveAll(int x, int y);
};

#endif	/* HANDLER_H */
