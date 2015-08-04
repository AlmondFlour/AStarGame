#include <SFML/Graphics.hpp>
#include <iostream>
#include "handler.h"
#include "entity.h"
#include "time.h"
#include "menu.h"
#include "camera.h"
#include <vector>

const int SCREEN_WIDTH = 1300;
const int SCREEN_HEIGHT = 700;
const int MAX_FPS = 60;
const int SCALE = 75;
const int TILES_X = 40;
const int TILES_Y = 40;
const int MAX_ENTITIES = 50;
const int WALKABLE_PROB = 80;

using namespace std;

vector<TerrainType> terrainList;
vector<GameObj *>  entities;

sf::Texture * slime;
sf::Texture * tile;

sf::RenderWindow * window;
Camera * gameView;
Camera * menuView;
Camera * interfaceView;
Camera * minimapView;
sf::Font * font;

Menu * pauseMenu;
bool paused = false;
int zoom = 20;

Level * currentLevel;

//Initialization
void initializeTerrain();
bool initializeWindow();
bool initializeTextures();
bool initializeObjects();
bool initializeOther();

//Start Menu
void startMenu();

//Game
void gameLoop();
void gameInput( sf::Event & event );
void menuInput( sf::Event & event );
void gameKeyState();
void gamePauseMenu();
void gameUpdate( bool paused );

//Cleanup
void cleanup();

//Other
sf::Vector2f convertCoordinates( sf::Vector2f coord );

int main()
{
    initializeWindow();
    initializeTerrain();
    initializeTextures();
    initializeObjects();

    startMenu();

    cleanup();
    return 0;
}

//=====Initialization
bool initializeWindow()
{
    window = new sf::RenderWindow(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "AStar Game" );
    window->setFramerateLimit( MAX_FPS );
    window->setKeyRepeatEnabled( false );

    sf::View * gView = new sf::View;
    gView->reset( sf::FloatRect( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT * 0.9f ));
    gView->setViewport( sf::FloatRect( 0.f, 0.f, 1.f, 0.8f ));
    gameView = new Camera( window, gView, currentLevel );

    sf::View * iView = new sf::View;
    iView->reset( sf::FloatRect( 0, 0, 800, 120 ));
    iView->setViewport( sf::FloatRect( 0.f, 0.8f, 1.f, 0.2f ) );
    interfaceView = new Camera( window, iView );

    sf::View * mView = new sf::View;
    mView->reset( sf::FloatRect( 0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT ));
    mView->setViewport( sf::FloatRect( 0.125f, 0.125f, 0.75f, 0.75f ));
    //mView->setViewport( sf::FloatRect( 0.4f, 0.1f, 0.3f, 0.23f ));
    menuView = new Camera( window, mView );

    sf::View * miniView = new sf::View;
    miniView->reset( sf::FloatRect( 0.f, 0.f, TILES_X * SCALE, TILES_Y * SCALE ));
    miniView->setViewport( sf::FloatRect( 0.8f, 0.8f, 0.2f, 0.2f ));
    minimapView = new Camera( window, miniView );

}
bool initializeTextures()
{
    slime = new sf::Texture;
    if(!slime->loadFromFile( "Images/slime1.png" ) )
        return -1;
    slime->setSmooth( true );

    tile = new sf::Texture;
    if(!tile->loadFromFile( "Images/tile1.png" ) )
        return -2;
    tile->setSmooth( true );

    font = new sf::Font;
    if(!font->loadFromFile( "Choktoff.ttf"))
        return -3;
}
bool initializeObjects()
{
    //Level
    currentLevel = new Level( window, TILES_X, TILES_Y, SCALE, tile, &terrainList );

    //Player Character
    GameObj * pChar = new GameObj( window, slime, 1, 1, SCALE );
    pChar->anim.setColor( sf::Color( 0, 255, 33, 255));
    entities.push_back( pChar );
    pChar->anim.setAnimation( sf::milliseconds( 100 ), 0, 0, 5, 0 );
    currentLevel->addEnt( pChar );

    //Other entities
    for( int n = 1; n < MAX_ENTITIES; n++ )
    {
        GameObj * obj = new GameObj( window, slime, 1, 1, SCALE );

        obj->setPosition( rand() % TILES_X, rand() % TILES_Y );
        obj->anim.setAnimation( sf::milliseconds( 100 ), 0, 0, 5, 0, true );
        obj->anim.setColor( sf::Color( rand() % 255, rand() % 255,
                            rand() % 255, 200 + rand() % 55));
        obj->anim.setAnimation( sf::milliseconds( 100 ), 0, 0, 5, 0 );

        entities.push_back( obj );
        currentLevel->addEnt( obj );
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

//====Menu
void startMenu()
{
    Menu menu( window, menuView, font );
    menu.addItem( "Start" );

    //Menu loop
    while (window->isOpen())
    {
        sf::Event event;
        while (window->pollEvent(event))
        {
            //Mouse button pressed
            if( event.type == sf::Event::MouseButtonPressed )
            {
                //if mouse left is pressed (toggle terrain)
                if( event.mouseButton.button == sf::Mouse::Left )
                {
                    int buttonPressed = menu.checkClicked( event.mouseButton.x, event.mouseButton.y );

                    switch( buttonPressed )
                    {
                        case 0:
                            gameLoop();
                            break;
                    }
                }
            }
            //check to close window
            else if (event.type == sf::Event::Closed)
            {
                window->close();
            }
        }

        //update screen
        menu.draw();
        window->display();
    }
}

//=====Game
void gameLoop()
{
    if( pauseMenu != NULL )
    {
        delete pauseMenu;
    }

    pauseMenu = new Menu( window, menuView, font );
    pauseMenu->addItem( "enter filename" );
    pauseMenu->addItem( "Load" );
    pauseMenu->addItem( "Save" );

    while (window->isOpen())
    {
        sf::Event event;

        //=====Input events
        while (window->pollEvent(event))
        {
            if ( !paused )
            {
                gameInput( event );
            }
            else
            {
                menuInput( event );
            }

            if ( event.type == sf::Event::Closed )
            {
                window->close();
            }

        }

        //=====Key state actions
        gameKeyState();

        //=====Update
        gameUpdate( paused );

        window->setFramerateLimit( MAX_FPS );
    }
}

void gameInput( sf::Event & event )
{
    if( event.type == sf::Event::MouseButtonPressed )
    {
        //if mouse left is pressed (toggle terrain)
        if( event.mouseButton.button == sf::Mouse::Left )
        {
            currentLevel->changesMade = true;

            sf::Vector2f mousePos = gameView->convertCoords(
                     event.mouseButton.x, event.mouseButton.y );

            int mouseX = mousePos.x/(float)SCALE ;
            int mouseY = mousePos.y/(float)SCALE;

            //switch the matching tile
            if( ( mouseX >= 0) && ( mouseX < TILES_X )
                && (mouseY >= 0) && (mouseY < TILES_Y ) )
            {
                Tile * tempTile = &currentLevel->tiles[mouseY][mouseX];

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
                sf::Vector2f mousePos = gameView->convertCoords(
                    event.mouseButton.x, event.mouseButton.y );

                int mouseX = mousePos.x/SCALE;
                int mouseY = mousePos.y/SCALE;

                //switch the matching tile
                if( ( mouseX >= 0) && ( mouseX < TILES_X )
                    && (mouseY >= 0) && (mouseY < TILES_Y ) )
                {
                    currentLevel->setEntPath( entities[0], mouseX, mouseY );
                }
            }
            else // move all
            {
                sf::Vector2f mousePos = gameView->convertCoords(
                    event.mouseButton.x, event.mouseButton.y );

                int mouseX = mousePos.x/SCALE;
                int mouseY = mousePos.y/SCALE;

                sf::Clock timer;
                timer.restart();

                currentLevel->moveAll( mouseX, mouseY );

                cout << "All entities moved in: " << timer.getElapsedTime().asSeconds() << " seconds.\n";
            }
        }
    }
    else if( event.type == sf::Event::KeyPressed)
    {
        //R pressed (random terrain )
        if( event.key.code == sf::Keyboard::R )
        {
            currentLevel->changesMade = true;
            currentLevel->randomizeTerrain( WALKABLE_PROB );
        }
        //Space pressed(random paths)
        else if( event.key.code == sf::Keyboard::Space )
        {
            currentLevel->randomizeAll();
        }
        //Y pressed (add)
        else if( event.key.code == sf::Keyboard::Y )
        {
            if( currentLevel->entities.size() < MAX_ENTITIES )
            {
                currentLevel->addEnt( entities[currentLevel->entities.size() ]);
            }
        }
        //1 pressed (save/load)
        else if( event.key.code == sf::Keyboard::Num1 )
        {
            //if shift held, save
            if (sf::Keyboard::isKeyPressed(sf::Keyboard ::LShift))
            {
                currentLevel->saveLevel( "testLevel1.txt");
            }
            //if not, load
            else
            {
               vector< vector< int > > tempLayout;
                currentLevel->loadLevel( tempLayout, "testLevel1.txt");

                currentLevel->loadTerrain( tempLayout);
            }

            currentLevel->changesMade = true;
        }
        //= pressed (add)
        else if( event.key.code == sf::Keyboard::Equal )
        {
            paused = true;
            pauseMenu->setItemText( 0, "filename" );
            pauseMenu->setInputItem( -1 );
        }
    }
    else if( event.type == sf::Event::MouseWheelMoved )
    {
        //Mouse wheel up
        if( ( event.mouseWheel.delta > 0 ) && ( zoom < 40 ) )
        {
            for( int n = 0; n < event.mouseWheel.delta; n++ )
            {
                gameView->setZoom( 1 );
                zoom++;
            }
        }
        //Mouse wheel down
        else if ( ( event.mouseWheel.delta < 0 ) && ( zoom > 0 ) )
        {
            for( int n = 0; n < (event.mouseWheel.delta * -1) ; n++ )
            {
                gameView->setZoom( -1 );
                zoom--;
            }
        }
    }
}

void menuInput( sf::Event & event )
{
    if( event.type == sf::Event::MouseButtonPressed )
    {
        //if mouse left is pressed
        if( event.mouseButton.button == sf::Mouse::Left )
        {
            int buttonPressed = pauseMenu->checkClicked( event.mouseButton.x, event.mouseButton.y );

            switch( buttonPressed )
            {
                case 0:
                {

                    //start entering a filename
                    pauseMenu->setInputItem( 0 );
                    pauseMenu->setItemText( 0 , "..." );
                    break;
                }
                case 1:
                {
                    //Load map and close menu
                    paused = false;
                    vector< vector< int > > tempLayout;
                    currentLevel->loadLevel( tempLayout, pauseMenu->getInput() );

                    currentLevel->loadTerrain( tempLayout);
                    currentLevel->changesMade = true;
                    break;
                }
                case 2:
                {
                    //Save map and close menu
                    paused = false;
                    currentLevel->saveLevel( pauseMenu->getInput());
                    break;
                }
            }
        }

    }
    else if( event.type == sf::Event::KeyPressed )
    {
        //= pressed (add)
        if( event.key.code == sf::Keyboard::Equal )
        {
            paused = false;
        }
    }
    // In event loop...
    else if ( event.type == sf::Event::TextEntered && pauseMenu->enteringText() )
    {
        pauseMenu->inputText( event.text.unicode );
    }
}

void gameKeyState()
{
    bool movingCameraX;
    bool movingCameraY;

    //Camera Controls
    if (sf::Keyboard::isKeyPressed(sf::Keyboard ::W))
    {
        //gameView->view->move( sf::Vector2f( 0, -10 ));
        gameView->acceleration.y = -10;
        movingCameraY = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard ::S))
    {
        //gameView->view->move( sf::Vector2f( 0, 10 ));
        gameView->acceleration.y = 10;
        movingCameraY = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard ::A))
    {
        //gameView->view->move( sf::Vector2f( -10, 0 ));
        gameView->acceleration.x = -10;
        movingCameraX = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard ::D))
    {
        //gameView->view->move( sf::Vector2f( 10, 0 ));
        gameView->acceleration.x = 10;
        movingCameraX = true;
    }

    if( !movingCameraX )
        gameView->acceleration.x = 0;
    if( !movingCameraY )
        gameView->acceleration.y = 0;
}

void gameUpdate( bool paused )
{
    window->clear();

    if( !paused )
    {
        if( currentLevel->changesMade )
                currentLevel->updateAllPaths();

        currentLevel->update();
        gameView->update();
    }

    window->setView( *gameView->view );
    currentLevel->draw();

    window->setView( *minimapView->view );
    currentLevel->drawTerrain();

    window->setView( window->getDefaultView() );



    if( paused )
    {
        //note: menu class uses views by itself
        pauseMenu->draw();
    }

    window->display();
}

//=====Cleanup
void cleanup()
{

}
