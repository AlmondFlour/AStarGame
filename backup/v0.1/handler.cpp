#include "handler.h"
#include <iostream>
#include <iterator>
#include <fstream>
#include <cctype>

/*=================
    Level
=================*/

//======Constructor/Destructor
Level::Level( sf::RenderWindow * win, int w, int h, int scale, sf::Texture * bg, std::vector<TerrainType> * tlist )
{
    terrainList = tlist;

    changesMade = false;

    window = win;
    this->w = w;
    this->h = h;
    this->scale = scale;

    Tile defaultTile;

    //TODO: allow entering clipping dimensions
    defaultTile.background.initilize( win, bg, sf::IntRect( 0, 0, 75, 75), scale, scale );
    std::vector< Tile > emptyVec;

    //setup grid
    for( int n = 0; n < h; n++)
    {
        tiles.push_back(emptyVec);

        defaultTile.y = n;

        for( int i = 0; i < w; i++ )
        {
            defaultTile.x = i;
            defaultTile.background.setPosition( i*scale, n*scale );
            tiles[n].push_back( defaultTile );

        }
    }
}

//======Utility Functions
void Level::update()
{
    //Handle collision


    for( int n = 0; n < entities.size(); n++ )
    {
        entities[n]->anim.update();

        //update positions
        if( !entities[n]->path.empty())
        {
            setEntPos( entities[n], entities[n]->path.front()->x, entities[n]->path.front()->y);

            entities[n]->path.pop_front();
        }

    }
}
void Level::draw()
{
    for( int n = h - 1; n >= 0; n-- )
    {
        for( int i = w - 1; i >= 0; i-- )
        {
            tiles[n][i].draw();
        }
    }
}
void Level::drawTerrain()
{
    for( int n = h - 1; n >= 0; n-- )
    {
        for( int i = w - 1; i >= 0; i-- )
        {
            tiles[n][i].drawTerrain();
        }
    }
}

//=====Path finding
void Level::aStar( GameObj * obj, Tile * dest )
{
    //erase current path
    obj->path.clear();

    //check if it dest is the start
    if( &tiles[obj->y][obj->x] == dest )
        return;

    //if destination is unwalkable
    if( !obj->movement.isWalkable( dest->terrain ) )
        return;

    //check if obj is in this level

    //lists of tiles to check/ignore
    std::deque<Tile * > openList;
    std::deque< Tile * > closedList;

    //add starting tile
    openList.push_back( &tiles[obj->y][obj->x] );

    tiles[obj->y][obj->x].parent = NULL;

    //loop until arrive at dest (break), or path not found
    while( !openList.empty() )
    {
        //get position of min element
        std::deque<Tile *>::iterator minPos = openList.begin();

        for( std::deque<Tile * >::iterator n = openList.begin(); n != openList.end(); ++n )
        {
            //uncomment to get Dijksta's
            //minPos = n;
            //break;

            if( (*n)->hcost <= (*minPos)->hcost )
                minPos = n;
        }

        Tile * current = *minPos;
        Tile * next = NULL;

        //move to closed list
        closedList.push_back( *minPos );
        openList.erase( minPos );


        /*std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~\n";
        for( std::deque<Tile *>::iterator n = closedList.begin(); n != closedList.end(); ++n )
        {
            std::cout << (*n)->x << ", " << (*n)->y << "\n";
        }*/

        //=====check each direction
        //up
        if( (current->y > 0 ) && ( obj->movement.isWalkable( (next = &tiles[current->y - 1][current->x] )->terrain  ) ) )
        {

            if( next == dest )
            {
                dest->parent = current;
                break;
            }

            int cost = current->cost + next->terrain.cost;

            int hcost = cost + abs(next->x - dest->x )
                             + abs(next->y - dest->y );

            //check if its on closedList
            if( std::find( closedList.begin(), closedList.end(), next ) != closedList.end())
            {
                //ignore it
            }
            //check if it's on openList
            else if( std::find( openList.begin(), openList.end(), next ) != openList.end())
            {
                //if cost is less with this node, change values
                if( cost < next->cost )
                {
                    openList.push_back(next);
                    next->cost = cost;
                    next->hcost = hcost;
                    next->parent = current;
                }
            }
            //if not, add it
            else
            {
                openList.push_back(next);
                next->cost = cost;
                next->hcost = hcost;
                next->parent = current;
            }

        }
        //down
        if( (current->y < h - 1 ) && ( obj->movement.isWalkable( (next = &tiles[current->y + 1][current->x] )->terrain  ) )  )
        {

            if( next == dest )
            {
                dest->parent = current;
                break;
            }

            int cost = current->cost + next->terrain.cost;

            int hcost = cost + abs(next->x - dest->x )
                             + abs(next->y - dest->y );

            //check if its on closedList
            if( std::find( closedList.begin(), closedList.end(), next ) != closedList.end())
            {
                //ignore it
            }
            //check if it's on openList
            else if( std::find( openList.begin(), openList.end(), next ) != openList.end())
            {
                //if cost is less with this node, change values
                if( cost < next->cost )
                {
                    openList.push_back(next);
                    next->cost = cost;
                    next->hcost = hcost;
                    next->parent = current;
                }
            }
            //if not, add it
            else
            {
                openList.push_back(next);
                next->cost = cost;
                next->hcost = hcost;
                next->parent = current;
            }

        }
        //left
        if( (current->x > 0 ) && ( obj->movement.isWalkable( (next = &tiles[current->y][current->x - 1] )->terrain  ) ) )
        {
            if( next == dest )
            {
                dest->parent = current;
                break;
            }

            int cost = current->cost + next->terrain.cost;

            int hcost = cost + abs(next->x - dest->x )
                             + abs(next->y - dest->y );

            //check if its on closedList
            if( std::find( closedList.begin(), closedList.end(), next ) != closedList.end())
            {

            }
            //check if it's on openList
            else if( std::find( openList.begin(), openList.end(), next ) != openList.end())
            {
                //if cost is less with this node, change values
                if( cost < next->cost )
                {
                    openList.push_back(next);
                    next->cost = cost;
                    next->hcost = hcost;
                    next->parent = current;
                }
            }
            //if not, add it
            else
            {
                openList.push_back(next);
                next->cost = cost;
                next->hcost = hcost;
                next->parent = current;
            }

        }
        //right
        if( (current->x < w - 1 ) && ( obj->movement.isWalkable( (next = &tiles[current->y][current->x + 1] )->terrain  ) ) )
        {

            if( next == dest )
            {
                dest->parent = current;
                break;
            }

            int cost = current->cost + next->terrain.cost;

            int hcost = cost + abs(next->x - dest->x )
                             + abs(next->y - dest->y );

            //check if its on closedList
            if( std::find( closedList.begin(), closedList.end(), next ) != closedList.end())
            {

            }
            //check if it's on openList
            else if( std::find( openList.begin(), openList.end(), next ) != openList.end())
            {
                //if cost is less with this node, change values
                if( cost < next->cost )
                {
                    openList.push_back(next);
                    next->cost = cost;
                    next->hcost = hcost;
                    next->parent = current;
                }
            }
            //if not, add it
            else
            {
                openList.push_back(next);
                next->cost = cost;
                next->hcost = hcost;
                next->parent = current;
            }

        }
        //std::cout << std::endl;
    }

    std::deque<Tile * > path;
    Tile * current = dest;

    //get the path
    while( current->parent != NULL )
    {
        path.push_front( current);
        current = current->parent;
    }

    //check if the start is included before updating
    if( (!path.empty()) && (path.front()->parent == &tiles[obj->y][obj->x]) )
    {
        obj->path = path;
    }

    /*
    std::cout << "========================\n";
    for( std::deque<Tile *>::iterator n = obj->path.begin(); n != obj->path.end(); ++n )
    {
        std::cout << (*n)->x << ", " << (*n)->y << "\n";
    }*/

    //wipe aStar data in tiles
    for( int n = 0; n < h; n++ )
    {
        for( int i = 0; i < w; i++)
        {
            tiles[n][i].parent = NULL;
        }
    }

}
void Level::updateAllPaths()
{
    changesMade = false;

    for( int n = 0; n < entities.size(); n++ )
    {
        if( !entities[n]->path.empty() )
        {
            //recalculate path
            aStar( entities[n], entities[n]->path.back() );
        }
    }
}
void Level::updatePath( GameObj * obj )
{
    aStar(obj, obj->path.back() );
}
//=====Other Interactions

void Level::addEnt( GameObj * ent )
{
    tiles[ent->y][ent->x].addEnt( ent );
    entities.push_back( ent);
}
void Level::removeEnt( GameObj * ent )
{
    tiles[ent->y][ent->x].addEnt( ent );
}
void Level::moveEnt( GameObj * ent, int x, int y )
{
    //check if ent is in level

    //check if move is valid
    if(!( ( ent->y + y < h ) && ( ent->y + y >= 0 ) &&
          ( ent->x + x < w ) && ( ent->x + x >= 0 ) ))
    {
        return;
    }

    //remove ent from current position
    tiles[ent->y][ent->x].removeEnt(ent);

    //modify tile attributes

    //add to new position
    tiles[ent->y + y][ent->x + x].addEnt(ent);

    //modify tile attributes

    //move
    ent->move( x, y );
}
void Level::setEntPos( GameObj * ent, int x, int y )
{
    //check if ent is in level

    //check if move is valid
    if(!( ( y < h ) && ( y >= 0 ) &&
          ( x < w ) && ( x >= 0 ) ))
    {
        return;
    }

    //remove ent from current position
    tiles[ent->y][ent->x].removeEnt(ent);

    //modify tile attributes

    //add to new position
    tiles[y][x].addEnt(ent);

    //modify tile attributes

    //move
    ent->setPosition( x, y );
}
void Level::setEntPath( GameObj * ent, int x, int y )
{
    if( (x >= 0) && ( x < w )
       && (y >= 0) && ( y < h ) )
    {
        aStar( ent, &tiles[y][x]);
        //std::cout << "\nADKLSJD\n";
    }
}
void Level::loadTerrain( std::vector< std::vector< int > > & ter )
{
    int currentX = 0;
    int currentY = 0;

    while( (currentY < h ) && ( currentY < ter.size() ) )
    {
        while( ( currentX < w ) && ( currentX < ter[currentY].size() ) )
        {
            //change tile
            tiles[currentY][currentX].setTerrain( (*terrainList)[ ter[currentY][currentX] ] );

            currentX++;
        }

        currentY++;
        currentX = 0;
    }
}

//=====Test Functions
void Level::randomizeAll()
{
    int rx, ry;

    for( int n = 0; n < entities.size(); n++ )
    {
        rx = rand() % w;
        ry = rand() % h;

        int tries = 0;
        Tile * t = &tiles[ry][rx];

        while( (!entities[n]->movement.isWalkable( t->terrain ) ) && (n < 5) )
        {
            rx = rand() % w;
            ry = rand() % h;

            tries++;
            t = &tiles[ry][rx];

        }

        aStar( entities[n], &tiles[ry][rx] );
    }
}
void Level::randomizeTerrain( double prob )
{
    changesMade = true;

    for( int n = 0; n < h; n++ )
    {
        for( int i = 0; i < w; i++ )
        {
            //open
            if( rand() % 100 < prob )
            {
                //normal
                if( rand() % 100 < prob )
                    tiles[n][i].setTerrain( (*terrainList)[0] );
                else
                {
                    //forest
                    if( rand() % 100 < prob )
                        tiles[n][i].setTerrain( (*terrainList)[3] );
                    //water
                    else
                        tiles[n][i].setTerrain( (*terrainList)[2] );
                }
            }
            //closed
            else
            {
                tiles[n][i].setTerrain( (*terrainList)[1] );
            }
        }
    }
}
void Level::moveAll( int x, int y)
{
    //check for valid movement
    if(!( ( y < h ) && ( y >= 0 ) &&
          ( x < w ) && ( x >= 0 ) ))
    {
        return;
    }

    for( int n = 0; n < entities.size(); n++ )
    {
        aStar( entities[n], &tiles[y][x] );
    }
}

bool Level::saveLevel( std::string filename )
{
    std::ofstream outFile;

    outFile.open( filename.c_str() );

    if( ! outFile.is_open() )
    {
        return false;
    }

    outFile << "#Terrain layout for a level" << std::endl;
    outFile << w << " " << h << std::endl;
    for( int n = 0; n < h; n++ )
    {
        for( int i = 0; i < w; i++ )
        {
            outFile << tiles[n][i].terrain.currentTerrain << " ";
        }

        outFile << std::endl;
    }

    outFile.close();
}
bool  Level::loadLevel( std::vector<std::vector<int> > & terrain, std::string filename )
{

    std::ifstream file;
    file.open(filename.c_str());

    int width = 0;
    int height = 0;

    if( !file.good() )
    {
        std::cout << "Invalid filename";
        return false;
    }

    std::string line;
    getline( file, line );

    //skip to height/width declaration
    while( !file.eof() )
    {
        //std::cout << line << std::endl;
        if( !line.empty() && ( line[0] != '#' ) )
        {
            break;
        }

        getline( file, line);
    }

    //get height and width
    while( !file.eof() )
    {
        //std::cout << line << std::endl;

        if( !line.empty() && (line[0] != '#' ) )
        {
            //width
            int pos = line.find( " " );
            width = atoi( line.substr( 0, pos ).c_str() );
            line.erase(0, pos + 1 );

            //height
            height = atoi( line.c_str() );

            break;
        }
    }

	getline(file, line);

    //parse tiles values
    //for( int n = 0; n < width * height ; n++)
    while(!file.eof())
    {
        //row currently being added to
        std::vector<int> row;

        //get position of next value
        size_t pos = 0;

        //parse line
        while( !line.empty()  )
        {
            if( isdigit( line[0] ) )
            {
                pos = line.find_first_not_of("1234567890");

                //Change level terrain
                row.push_back( atoi( line.substr(0, pos).c_str() ) );

                line.erase( 0, pos );
            }
            else
            {
                pos = line.find_first_of("1234567890");

                if( pos == std::string::npos )
                    break;

                line.erase( 0, pos );
            }
        }

        getline( file, line);
        //line.erase(0, pos + 1);

        terrain.push_back( row );
    }
}

void Level::clearMap()
{
    for( int n = 0; n < h; n++)
    {
        for( int i = 0; i < w; i++)
        {
            tiles[n][i].setTerrain( (*terrainList)[0] );
        }
    }
}
