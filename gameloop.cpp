#include <SFML/Graphics.hpp>
#include <iostream>
#include "handler.h"
#include "entity.h"
#include "time.h"
#include "menu.h"
#include "camera.h"
#include "interface.h"
#include <string>
#include <vector>
#include <map>

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
map< string, sf::Texture * > textures;
map< string, Anim > animators;
map< string, Ability > abilities;

UnitStats * defaultUnitStats;


sf::RenderWindow * window;
Camera * gameView;
Camera * menuView;
Camera * interfaceView;
Camera * minimapView;
sf::Font * font;
GameInterface * gameInterface;

Menu * pauseMenu;
bool paused = false;
int zoom = 20;

Level * currentLevel;

//Initialization
void initializeTerrain();
bool initializeWindow();
bool initializeTextures();
bool initializeAnimators();
bool initializeAbilities();
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
    initializeTextures();
    initializeAnimators();
    initializeTerrain();
    initializeObjects();
    initializeAbilities();
    initializeOther();

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
    iView->reset( sf::FloatRect( 0, 0, 800, 200 ));
    iView->setViewport( sf::FloatRect( 0.f, 0.8f, 0.8f, 0.2f ) );
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
    sf::Texture * slime = new sf::Texture;
    if(!slime->loadFromFile( "Images/slime1.png" ) )
        return -1;
    textures.insert( textures.end(), std::pair< string, sf::Texture * >( "slime", slime ) );

    sf::Texture * tile = new sf::Texture;
    if(!tile->loadFromFile( "Images/tile1 Border.png" ) )
        return -2;
    textures.insert( textures.end(), std::pair< string, sf::Texture * >( "tile", tile ) );

    font = new sf::Font;
    if(!font->loadFromFile( "Choktoff.ttf"))
        return -3;

    sf::Texture * fire = new sf::Texture;
    if( !fire->loadFromFile( "Images/fire.png" ) )
        return -4;
    textures.insert( textures.end(), std::pair< string, sf::Texture * >( "fire", fire ) );
}
bool initializeAnimators()
{

    Anim baseAnim( window, NULL, sf::IntRect( 0, 0, 75, 75 ), SCALE, SCALE );
    //baseAnim.addAnimation( "idle", sf::milliseconds( 100 ), 0, 0, 5, 0 );
    //baseAnim.setAnimation( "idle");

    animators.insert( animators.end(), std::pair< string, Anim >( "slime", baseAnim ) );
    animators["slime"].setTexture( textures["slime"] );
    animators["slime"].addAnimation( "idle", sf::milliseconds( 100 ), 0 , 0, 5, 0 );
    animators["slime"].setAnimation( "idle" );

    animators.insert( animators.end(), std::pair< string, Anim >( "tile", baseAnim ) );
    animators["tile"].setTexture( textures[ "tile"] );
    //animators["tile"].addAnimation( "idle", sf::milliseconds( 100 ), 0, 0, 5, 0 );
    //animators["tile"].setAnimation( "idle" );

    animators.insert( animators.end(), std::pair< string, Anim >( "fire", baseAnim ) );
    animators["fire"].setTexture( textures["fire"] );
    animators["fire"].setClipDimensions( 256, 256 );
    animators["fire"].addAnimation( "idle", sf::milliseconds( 100 ), 0, 0, 2, 0 );
    animators["fire"].setAnimation( "idle" );
}
bool initializeObjects()
{
    //Level
    currentLevel = new Level( window, TILES_X, TILES_Y, SCALE, &terrainList );

    //Stuff for Game Object setup
    defaultUnitStats = new UnitStats( animators["slime"], terrainList[0] );
    defaultUnitStats->anim.setColor( sf::Color( 0, 255, 33, 255 ) );
    defaultUnitStats->name = "Slime";
    defaultUnitStats->description = "A test unit";

    //Player Character
    GameObj * pChar = new GameObj( currentLevel, *defaultUnitStats, 1, 1, SCALE );
    entities.push_back( pChar );
    currentLevel->addEnt( pChar );

    //Other entities
    for( int n = 1; n < MAX_ENTITIES; n++ )
    {
        defaultUnitStats->anim.setColor( sf::Color( rand() % 255, rand() % 255,
                            rand() % 255, 200 + rand() % 55));

        GameObj * obj = new GameObj( currentLevel, *defaultUnitStats, 1, 1, SCALE );

        obj->setPosition( rand() % TILES_X, rand() % TILES_Y );

        entities.push_back( obj );
        currentLevel->addEnt( obj );
    }


}
void initializeTerrain()
{
    //Anim tileAnim( window, textures["tile"], sf::IntRect( 0, 0, 75, 75 ), SCALE, SCALE );

    TerrainType terrain;
    //terrain.anim.initilize( window, textures["tile"], sf::IntRect( 0, 0, 75, 75 ), SCALE, SCALE );
    terrain.anim = animators["tile"];


    //Open (0)
    terrain.walkable = true;
    terrain.aquatic = false;
    terrain.flying = true;
    terrain.cost = 1;
    terrain.currentTerrain = 0;
    terrainList.push_back( terrain );

    terrain.anim.setFrame( 1, 0 );

    //closed (1)
    terrain.walkable = false;
    terrain.aquatic = false;
    terrain.flying = false;
    terrain.cost = 10;
    terrain.currentTerrain = 1;
    terrainList.push_back( terrain );

    terrain.anim.setFrame( 2, 0 );

    //aquatic (2)
    terrain.walkable = false;
    terrain.aquatic = true;
    terrain.flying = true;
    terrain.cost = 1;
    terrain.currentTerrain++;
    terrainList.push_back( terrain );

    terrain.anim.setFrame( 3, 0 );

    //forest (3)
    terrain.walkable = true;
    terrain.aquatic = false;
    terrain.flying = false;
    terrain.cost = 5;
    terrain.currentTerrain++;
    terrainList.push_back( terrain );
}
bool initializeAbilities()
{

    Effect testEffect( animators["fire"], sf::milliseconds( 5000 ));
    testEffect.setPosition( 75, 75 );
    Ability testAbility( testEffect );

    abilities.insert( abilities.end(), std::pair< string, Ability >( "fire", testAbility ) );

    entities[0]->stats.addAbility( abilities["fire"]);

    return true;
}
bool initializeOther()
{
    gameInterface = new GameInterface( interfaceView, font );

    return true;
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
    }
}

void gameInput( sf::Event & event )
{
    if( event.type == sf::Event::MouseMoved )
    {
        sf::Vector2f mousePos( sf::Mouse::getPosition( *window ).x / window->getSize().x,
                               sf::Mouse::getPosition( *window ).y / window->getSize().y );

        if( gameView->view->getViewport().contains( mousePos ));
        {
            sf::Vector2f mousePos = gameView->convertCoords(
            sf::Mouse::getPosition( *window ).x, sf::Mouse::getPosition(*window).y );

            int mouseX = mousePos.x/(float)SCALE ;
            int mouseY = mousePos.y/(float)SCALE;

            //switch the matching tile
            if( ( mouseX >= 0) && ( mouseX < TILES_X )
                && (mouseY >= 0) && (mouseY < TILES_Y ) )
            {
                if( !currentLevel->tiles[mouseY][mouseX].entities.empty())
                {
                    gameInterface->selectUnit( currentLevel->tiles[mouseY][mouseX].entities[0] );
                }
                else
                {
                    //gameInterface->selectUnit();
                }
            }
        }
    }
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
                    tempTile->setTerrain( terrainList[tempTile->terrain.currentTerrain + 1], SCALE, SCALE);
                }
                else
                    tempTile->setTerrain( terrainList[0], SCALE, SCALE);
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
        else if ( event.key.code == sf::Keyboard::F )
        {
            sf::Vector2f mousePos = gameView->convertCoords(
                    sf::Mouse::getPosition( *window ).x, sf::Mouse::getPosition(*window).y );

            int mouseX = mousePos.x/(float)SCALE ;
            int mouseY = mousePos.y/(float)SCALE;

            //switch the matching tile
            if( ( mouseX >= 0) && ( mouseX < TILES_X )
                && (mouseY >= 0) && (mouseY < TILES_Y ) )
            {
                entities[0]->stats.abilities[0].cast( mouseX, mouseY );
            }
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

    gameInterface->draw();

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
