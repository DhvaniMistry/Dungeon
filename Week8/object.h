#ifndef OBJECT_H
#define OBJECT_H

#include <string>
#include "dice.h"
#include "character.h"
#include "descriptions.h"


typedef struct dungeon dungeon_t;

class object
{
    public:
    std::string name;
    std::string description;
    char symbol;
    object_type_t type;
    uint32_t color;
    pair_t position;
    dice damage;
    int32_t weight,
        speed,
        attribute,
        value,
        hit,
        dodge,
        defense;
    bool seen;
};

void create_obj(dungeon_t *d);
void delete_obj(dungeon_t *d);
int16_t *position_obj(object *i);

#endif
