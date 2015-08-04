#include "animation.h"

Anim::Anim( sf::RenderWindow * win, sf::Texture * t, sf::IntRect c, int w, int h )
{
    window = win;
    texture = t;
    clip = c;

    shape.setSize( sf::Vector2f( w, h ));
    shape.setPosition( sf::Vector2f(0, 0));
    shape.setTexture( texture );
    shape.setTextureRect( clip );

    inAnimation = false;
    loopAnim = false;

    currentX = 0;
    currentY = 0;
    startX = 0;
    startY = 0;
    endX = 0;
    endY = 0;

}
Anim::Anim()
{
    window = NULL;
    texture = NULL;

    clip.width = 0;
    clip.height = 0;
    clip.top = 0;
    clip.left = 0;

    shape.setSize( sf::Vector2f( 1, 1 ));
    shape.setPosition( sf::Vector2f(0, 0));
    shape.setTexture( texture );
    shape.setTextureRect( clip );

    inAnimation = false;
    loopAnim = false;

    currentX = 0;
    currentY = 0;
    startX = 0;
    startY = 0;
    endX = 0;
    endY = 0;

}

//for use with empty construction
void Anim::initilize( sf::RenderWindow * w, sf::Texture * t, sf::IntRect c, int x, int y )
{
    window = w;
    texture = t;
    clip = c;

    shape.setSize( sf::Vector2f( x, y ));
    shape.setPosition( sf::Vector2f(0, 0));
    shape.setTexture( texture );
    shape.setTextureRect( clip );

    inAnimation = false;
    loopAnim = false;

    currentX = 0;
    currentY = 0;
    startX = 0;
    startY = 0;
    endX = 0;
    endY = 0;
}

void Anim::setTexture( sf::Texture * t )
{
    texture = t;
    shape.setTexture( t );
}
void Anim::setWindow( sf::RenderWindow * w )
{
    window = w;
}
void Anim::setFrame( int fx, int fy )
{
    clip.left = fx * clip.width;
    clip.top =  fy * clip.height;

    shape.setTextureRect(clip);
}
void Anim::setFrameAbsolute( int x, int y )
{
    clip.left = x;
    clip.top = y;

    shape.setTextureRect( clip );
}
void Anim::setPosition( int x, int y )
{
    shape.setPosition( sf::Vector2f( x, y ) );
}
void Anim::setDimensions( int w, int h)
{
    shape.setSize( sf::Vector2f( w, h ));
}
void Anim::setClipDimensions( int w, int h )
{
    clip.width = w;
    clip.height = h;

    shape.setTextureRect( clip );
}
void Anim::setColor( sf::Color c )
{
    shape.setFillColor( c );
}

sf::Vector2f Anim::getPosition()
{
    return shape.getPosition();
}
sf::FloatRect Anim::getLocalBounds()
{
    return shape.getLocalBounds();
}

//dur is the duration of each frame
void Anim::setAnimation( sf::Time dur, int sx, int sy, int ex, int ey, bool loop )
{
    duration = dur;
    clock.restart();

    currentX = sx;
    currentY = sy;
    startX = sx;
    startY = sy;
    endX = ex;
    endY = ey;

    inAnimation = true;
    loopAnim = loop;

    setFrame( sx, sy );
}
void Anim::draw()
{
    window->draw( shape );
}
void Anim::update()
{
    if( !inAnimation )
    {
        return;
    }

    //if not enough time has passed
    if( clock.getElapsedTime() < duration )
    {
        return;
    }

    currentX++;

    if( currentX * clip.width >  texture->getSize().x )
    {
        currentY++;
        currentX = 0;
    }

    //if animation is still going
    if ( (currentY < endY ) || (currentX < endX ) )
    {
        setFrame( currentX, currentY );
    }
    //if animation is ending
    else if( loopAnim )
    {
        currentX = startX;
        currentY = startY;
        setFrame( currentX, currentY );
    }
    else
    {
        inAnimation = false;
    }

    clock.restart();
}

/*================
    Effect
================*/

Effect::Effect( Anim a, sf::Time dur )
{
    anim = a;
    duration = dur;

    timer.restart();
}
Effect::Effect( const Effect & other )
{
    anim = other.anim;
    duration = other.duration;

    timer.restart();
}

bool Effect::checkExpiration()
{
    return ( duration > timer.getElapsedTime() );
}

void Effect::draw()
{
    anim.draw();
}
