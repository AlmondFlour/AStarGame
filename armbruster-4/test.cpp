#include <SFML/Graphics.hpp>
#include <iostream>
#include "handler.h"
#include "entity.h"
#include "time.h"

const int SCREEN_WIDTH = 900;
const int SCREEN_HEIGHT = 700;
const int MAX_FPS = 60;
const int SCALE = 20;
const int TILES_X = 40;
const int TILES_Y = 30;
const int MAX_ENTITIES = 100;
const int WALKABLE_PROB = 70;

using namespace std;

int main()
{
    srand(time(NULL) );

    //create window
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "AStar Game");

    //====Text
    sf::Font font;
    if(!font.loadFromFile( "Choktoff.ttf"))
        return -3;

    sf::Text instruct1("Use WASD or right click to move,left click to toggle terrain", font, 14);
    sf::Text instruct2("'y' to add 1 slime", font, 14);
    sf::Text instruct3("'r' to randomize terrain, space to randomize movements", font, 14);
    sf::Text instruct4("'x' to clear terrain, 'c' to clear entities", font, 14);


    instruct1.setPosition( 25, SCREEN_HEIGHT - 100 );
    instruct2.setPosition( 25, SCREEN_HEIGHT - 80 );
    instruct3.setPosition( 25, SCREEN_HEIGHT - 60 );

    //=====Textures
    //load texture
    sf::Texture testText;
    if(!testText.loadFromFile( "slime1.png" ) )
        return -1;
    testText.setSmooth( true );

    //load texture
    sf::Texture tileText;
    if(!tileText.loadFromFile( "Tile1.png" ) )
        return -2;
    tileText.setSmooth( true );

    //==Other Objects
    //make gameObj
    GameObj test( &window, &testText, 1, 1, SCALE);
    test.setFrameDimensions(75, 75);
    test.anim.shape.setFillColor( sf::Color( 0, 255, 33, 255));

    Level testLevel( &window, TILES_X, TILES_Y, SCALE, &tileText);
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

        testArr[n].anim.shape.setFillColor( sf::Color( rand() % 255, rand() % 255,
                                                       rand() % 255, 200 + rand() % 55));

        //testLevel.addEnt( &testArr[n]);
    }


    sf::Clock timer;

    //=====game loop
    while (window.isOpen())
    {
        sf::Event event;

        //handle inputs
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
                        testLevel.tiles[mouseY][mouseX].toggleWalkable();
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
                if( event.key.code == sf::Keyboard::Space )
                {
                    testLevel.randomizeAll();
                }
                //Y pressed (add)
                if( event.key.code == sf::Keyboard::Y )
                {
                    if( extraEntities < MAX_ENTITIES )
                    {
                        testLevel.addEnt( &testArr[extraEntities]);

                        extraEntities++;
                    }
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

        window.display();

        //cap frame rate
        //sf::sleep( sf::milliseconds(100) );

        window.setFramerateLimit( MAX_FPS );
    }

}
