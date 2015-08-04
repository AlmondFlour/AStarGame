#include <SFML/Graphics.hpp>
#include <iostream>
#include "handler.h"
#include "entity.h"
#include "time.h"
#include "menu.h"
#include <vector>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 660;
const int MAX_FPS = 60;
const int SCALE = 20;
const int TILES_X = 40;
const int TILES_Y = 30;
const int MAX_ENTITIES = 100;
const int WALKABLE_PROB = 80;

using namespace std;

//List of possible terrain for tiles
vector<TerrainType> terrainList;

void initializeTerrain();

int main()
{
    srand(time(NULL) );

    //create window
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "AStar Game");
    window.setFramerateLimit( MAX_FPS );

    //Define terrain
    initializeTerrain();

    //====Text
    sf::Font font;
    if(!font.loadFromFile( "Choktoff.ttf"))
        return -3;

    sf::Text instruct1("Use WASD or right click to move,left click to toggle terrain", font, 14);
    sf::Text instruct2("'y' to add 1 slime, shift+right click to move all", font, 14);
    sf::Text instruct3("'r' to randomize terrain, space to randomize movements", font, 14);
    sf::Text instruct4("'x' to clear terrain, 'c' to clear entities", font, 14);

    instruct1.setPosition( 25, SCREEN_HEIGHT - 60 );
    instruct2.setPosition( 25, SCREEN_HEIGHT - 40 );
    instruct3.setPosition( 25, SCREEN_HEIGHT - 20 );

    //=====Textures
    //load texture
    sf::Texture testText;
    if(!testText.loadFromFile( "slime1.png" ) )
        return -1;
    testText.setSmooth( true );

    sf::Texture tileText;
    if(!tileText.loadFromFile( "Tile1.png" ) )
        return -2;
    tileText.setSmooth( true );

    //==Other Objects
    //make gameObj
    GameObj test( &window, &testText, 1, 1, SCALE);
    test.setFrameDimensions(75, 75);
    test.anim.setColor( sf::Color( 0, 255, 33, 255));
    test.anim.setAnimation( sf::milliseconds( 100 ), 0, 0, 5, 0 );

    Level testLevel( &window, TILES_X, TILES_Y, SCALE, &tileText, &terrainList);
    testLevel.addEnt( &test );

    GameObj testArr[MAX_ENTITIES];
    int extraEntities = 0;
    for( int n = 0; n < MAX_ENTITIES; n++)
    {
        testArr[n].setWindow( &window);
        testArr[n].scale = SCALE;
        testArr[n].setDimensions( 1, 1 );
        testArr[n].setFrameDimensions( 75, 75 );
        testArr[n].setSprite( &testText );
        testArr[n].setFrame( 0, 0 );
        testArr[n].setPosition( rand() % TILES_X, rand() % TILES_Y );
        testArr[n].anim.setAnimation( sf::milliseconds( 100 ), 0, 0, 5, 0, true );

        testArr[n].anim.setColor( sf::Color( rand() % 255, rand() % 255,
                                              rand() % 255, 200 + rand() % 55));

        //testLevel.addEnt( &testArr[n]);
    }

    //=====Initialize other things
    sf::Clock timer;

    //=====Menu
    Menu testMenu( &window, sf::IntRect( 100, 100, 400, 200 ), sf::IntRect( 75, 30, 250, 60 ), &font );
    testMenu.addItem( "Filename");
    testMenu.addItem( "Okay" );
    bool displayFileSelect = false;

    //=====game loop
    while (window.isOpen())
    {
        sf::Event event;

        //handle inputs (keys pressed)
        while (window.pollEvent(event))
        {
            if( event.type == sf::Event::MouseButtonPressed )
            {
                //if mouse left is pressed (toggle terrain)
                if( event.mouseButton.button == sf::Mouse::Left )
                {
                    testLevel.changesMade = true;

                    int mouseX = event.mouseButton.x/SCALE;
                    int mouseY = event.mouseButton.y/SCALE;

                    //switch the matching tile
                    if( ( mouseX >= 0) && ( mouseX < TILES_X )
                        && (mouseY >= 0) && (mouseY < TILES_Y ) )
                    {
                        Tile * tempTile = &testLevel.tiles[mouseY][mouseX];

                        if( tempTile->terrain.currentTerrain < 3)
                        {
                            tempTile->setTerrain( terrainList[tempTile->terrain.currentTerrain + 1]);
                        }
                        else
                            tempTile->setTerrain( terrainList[0] );
                    }
                }
                //if mouse right is pressed (move)
                if( event.mouseButton.button == sf::Mouse::Right )
                {
                    if (!sf::Keyboard::isKeyPressed(sf::Keyboard ::LShift))
                    {
                        int mouseX = event.mouseButton.x/SCALE;
                        int mouseY = event.mouseButton.y/SCALE;

                        //switch the matching tile
                        if( ( mouseX >= 0) && ( mouseX < TILES_X )
                            && (mouseY >= 0) && (mouseY < TILES_Y ) )
                        {
                            testLevel.setEntPath( &test, mouseX, mouseY );
                        }
                    }
                    else // move all
                    {
                        int mouseX = event.mouseButton.x/SCALE;
                        int mouseY = event.mouseButton.y/SCALE;

                        timer.restart();

                        testLevel.moveAll( mouseX, mouseY );

                        cout << "All entities moved in: " << timer.getElapsedTime().asSeconds() << " seconds.\n";
                    }
                }
            }
            else if( event.type == sf::Event::KeyPressed)
            {
                //R pressed (random terrain )
                if( event.key.code == sf::Keyboard::R )
                {
                    testLevel.changesMade = true;
                    testLevel.randomizeTerrain( WALKABLE_PROB );
                }
                //Space pressed(random paths)
                else if( event.key.code == sf::Keyboard::Space )
                {
                    testLevel.randomizeAll();
                }
                //Y pressed (add)
                else if( event.key.code == sf::Keyboard::Y )
                {
                    if( extraEntities < MAX_ENTITIES )
                    {
                        testLevel.addEnt( &testArr[extraEntities]);

                        extraEntities++;
                    }
                }
                //1 pressed (save/load)
                else if( event.key.code == sf::Keyboard::Num1 )
                {
                    //if shift held, save
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard ::LShift))
                    {
                        testLevel.saveLevel( "testLevel1.txt");
                    }
                    //if not, load
                    else
                    {
                       vector< vector< int > > tempLayout;
                        testLevel.loadLevel( tempLayout, "testLevel1.txt");

                        /*
                        for( int n = 0; n < tempLayout.size(); n++)
                        {
                            for( int i = 0; i < tempLayout[n].size(); i++)
                            {
                                cout << tempLayout[n][i] << " ";
                            }
                            cout << endl;
                        }*/

                        testLevel.loadTerrain( tempLayout);
                    }

                    testLevel.changesMade = true;
                }
                //= pressed (add)
                else if( event.key.code == sf::Keyboard::Equal )
                {
                    testLevel.clearMap();
                    testLevel.changesMade = true;
                }
            }
            //check to close window
            else if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        //check keys pressed
        if (sf::Keyboard::isKeyPressed(sf::Keyboard ::W))
        {
            testLevel.setEntPath( &test, test.x, test.y - 1 );
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            testLevel.setEntPath( &test, test.x, test.y + 1);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            testLevel.setEntPath( &test, test.x - 1, test.y );
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            testLevel.setEntPath( &test, test.x + 1, test.y );
        }

        //=====Update
        if( testLevel.changesMade )
            testLevel.updateAllPaths();

        testLevel.update();
        window.clear();
        testLevel.draw();

        window.draw( instruct1 );
        window.draw( instruct2 );
        window.draw( instruct3 );

        testMenu.draw();

        window.display();

        //cap frame rate
        //sf::sleep( sf::milliseconds(100) );

        //window.setFramerateLimit( MAX_FPS );
    }

}

void initializeTerrain()
{
    TerrainType terrain;

    //Open (0)
    terrain.walkable = true;
    terrain.aquatic = false;
    terrain.flying = true;
    terrain.cost = 1;
    terrain.currentTerrain = 0;
    terrainList.push_back( terrain );

    //closed (1)
    terrain.walkable = false;
    terrain.aquatic = false;
    terrain.flying = false;
    terrain.cost = 10;
    terrain.currentTerrain = 1;
    terrainList.push_back( terrain );

    //aquatic (2)
    terrain.walkable = false;
    terrain.aquatic = true;
    terrain.flying = true;
    terrain.cost = 1;
    terrain.currentTerrain++;
    terrainList.push_back( terrain );

    //forest (3)
    terrain.walkable = true;
    terrain.aquatic = false;
    terrain.flying = false;
    terrain.cost = 5;
    terrain.currentTerrain++;
    terrainList.push_back( terrain );
}
