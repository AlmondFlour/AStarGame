#include "entitiy.h"


class StandardAbility : public Ability
{
    public:
        Effect castEffect;
        Effect persistentEffect;

        sf::Time castTime;
        sf::Time effectDuration;
};
