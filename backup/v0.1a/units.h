#ifndef UNITS_H
#define	UNITS_H

#include "entity.h"
#include <vector>

struct Ability;
enum Target {SINGLE = 0, AREA = 1, NO_TARGET = 2 };

struct UnitStats
{
    Anim anim;

    int health;
    int maxHealth;
    float moveSpeed;
    TerrainType movement;

    std::vector< Ability > abilities;

    UnitStats(){}

    UnitStats( Anim a, TerrainType moveType ):
    health(1), maxHealth(1), moveSpeed(1),
    movement(), anim( a ){}

    UnitStats( const UnitStats & other ) :
        health( other.health ), maxHealth( other.maxHealth ),
        moveSpeed(1), movement( other.movement), anim( other.anim ) {}

    void damage( int d );
    void addAbility( Ability a );
};



struct Ability
{
    GameObj * owner;

    int damage;
    int cost;

    Target targetType;
    Effect effect;

    Ability( Effect e, GameObj * own = NULL ) :
        damage( 0 ), cost( 0 ), targetType( SINGLE ),
        effect( e ), owner( own ){}

    Ability( const Ability & other ) :
        damage( other.damage ), cost(other.cost), targetType( other.targetType),
        effect( other.effect), owner(other.owner) {}

    bool cast( int x, int y );
    bool cast( GameObj * obj );
    bool cast();
};

#endif	/* UNITS_H */
