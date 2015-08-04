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

        std::vector< TerrainType > * terrainList;

    public:
        bool changesMade;

        std::vector< std::vector< Tile > > tiles;
        std::vector< GameObj* > entities;

        Level( sf::RenderWindow * win, int w, int h, int scale, sf::Texture * bg, std::vector<TerrainType> * tlist);

        void addEnt( GameObj * ent );
        void removeEnt( GameObj * ent );
        void moveEnt( GameObj * ent, int x, int y );
        void setEntPos( GameObj * ent, int x, int y );
        void setEntPath( GameObj * ent, int x, int y);
        void loadTerrain( std::vector< std::vector< int > > & ter );
        bool saveLevel( std::string filename );
        bool loadLevel( std::vector<std::vector<int> > & terrain, std::string filename );

        void update();
        void updatePath( GameObj * obj);
        void updateAllPaths();
        void aStar( GameObj*, Tile * dest );

        void draw();
        void drawTerrain();

        //test functions
        void clearMap();
        void randomizeAll();
        void randomizeTerrain( double prob );
        void moveAll(int x, int y);
};

#endif	/* HANDLER_H */
