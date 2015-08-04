#ifndef ANIM_H
#define	ANIM_H

#include <SFML/Graphics.hpp>

class Anim
{
    private:
        sf::RenderWindow * window;
        sf::Texture * texture;
        sf::RectangleShape shape;
        sf::IntRect clip;

        sf::Clock clock;

        bool inAnimation;
        bool loopAnim;
        sf::Time duration;

        //frames for current animation
        int currentX;
        int currentY;
        int startX;
        int startY;
        int endX;
        int endY;

    public:
        Anim( sf::RenderWindow * win, sf::Texture * t, sf::IntRect c, int w, int h );
        Anim();

        Anim( const Anim &other) : window(other.window),
                                   texture(other.texture),
                                   shape(other.shape),
                                   clip(other.clip),
                                   clock(other.clock),
                                   inAnimation(other.inAnimation ),
                                   loopAnim( other.loopAnim),
                                   duration( other.duration),
                                   currentX( other.currentX),
                                   currentY( other.currentY),
                                   startX( other.startX),
                                   startY( other.startY),
                                   endX( other.endX),
                                   endY( other.endY){};

        //for use with empty construction
        void initilize( sf::RenderWindow * w, sf::Texture * t, sf::IntRect c, int x, int y );

        void setTexture( sf::Texture * t );
        void setWindow( sf::RenderWindow * w );
        void setFrame( int fx, int fy );
        void setFrameAbsolute( int x, int y );
        void setPosition( int x, int y );
        void setDimensions( int w, int h);
        void setClipDimensions( int w, int h );
        void setColor( sf::Color c);
        sf::Vector2f getPosition();
        sf::FloatRect getLocalBounds();

        //duration is in miliseconds (or maybe frames?)
        void setAnimation( sf::Time duration, int sx, int sy, int fx, int fy, bool loop = true );

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
        void draw();
};

#endif	/* ANIM_H */
