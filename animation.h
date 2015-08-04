#ifndef ANIM_H
#define	ANIM_H

#include <SFML/Graphics.hpp>
#include <map>
#include <iostream>

//holds information for Anim
struct Animation
{
    int startX;
    int startY;
    int endX;
    int endY;

    bool loopAnim;
    sf::Time duration;

    Animation() : startX(0), startY(0), endX(0), endY(0),
        loopAnim( false ), duration( sf::milliseconds(100) ) {}

    Animation( int sx, int sy, int ex, int ey, sf::Time dur, bool loop = true ) :
        startX(sx), startY( sy ), endX( ex), endY( ey ), duration( dur ), loopAnim(loop)  {}

    Animation( const Animation & other ) : startX(other.startX),
        startY(other.startY), endX(other.endX), endY(other.endY),
        loopAnim( other.loopAnim ), duration( other.duration ) {}


};

//handles interactions with SFML
class Anim
{
    private:
        sf::RenderWindow * window;
        sf::Texture * texture;
        sf::RectangleShape shape;
        sf::IntRect clip;

        sf::Clock clock;

        bool inAnimation;
        sf::Time duration;

        std::map< std::string, Animation > animList;
        Animation * currentAnimation;
        std::string currentName;

        //frames for current animation
        int currentX;
        int currentY;

    public:
        Anim( sf::RenderWindow * win, sf::Texture * t, sf::IntRect c, int w, int h );
        Anim();

        Anim( const Anim &other);

        //for use with empty construction
        void initilize( sf::RenderWindow * w, sf::Texture * t, sf::IntRect c, int x, int y );

        void operator=(const Anim &newAnim);

        void setTexture( sf::Texture * t, bool reset = false );
        void setWindow( sf::RenderWindow * w );
        void setFrame( int fx, int fy );
        void setPosition( int x, int y );
        void setDimensions( int w, int h);
        void setClipDimensions( int w, int h );
        void setColor( sf::Color c);
        void setOutlineThickness( const float & t );
        void setOutlineColor( const sf::Color & c );
        void setRotation( float r );
        void setOrigin( sf::Vector2f o );

        sf::Texture * getTexture();
        sf::Vector2f getPosition();
        sf::FloatRect getLocalBounds();
        sf::IntRect getClipDimensions();
        sf::Color getFillColor();

        void addAnimation( std::string name, sf::Time dur, int sx, int sy, int fx, int fy, bool loop = true );
        void setAnimation( std::string name );

        void draw();
        void update();
};

class Effect
{
    public:
        Anim anim;
        sf::Time duration;
        sf::Clock timer;

        Effect( Anim a, sf::Time dur );
        Effect( const Effect & other );

        bool checkExpiration();
        void setPosition( int x, int y);
        void draw();
};

#endif	/* ANIM_H */
