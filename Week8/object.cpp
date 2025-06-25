#include <vector>
#include <cstdlib>
#include "object.h"
#include "dungeon.h"
#include "utils.h"
#include "descriptions.h"


void create_obj(dungeon_t *d)
{
    int i;
    pair_t p;
    object *it;
    uint32_t room;

    std::vector<object_description> &desc = d->object_descriptions;

    // Set the number of items to place
    d->num_objects = std::min((uint32_t)10, (uint32_t)desc.size());

    for (i = 0; i < d->num_objects; i++)
    {
        // Generate an item from a random description
        it = desc[rand_range(0, desc.size() - 1)].gen_object();

        // Choose a random room and a position inside it
        do
        {
            room = rand_range(0, d->num_rooms - 1);
            p[dim_y] = rand_range(d->rooms[room].position[dim_y],
                                  d->rooms[room].position[dim_y] + d->rooms[room].size[dim_y] - 1);
            p[dim_x] = rand_range(d->rooms[room].position[dim_x],
                                  d->rooms[room].position[dim_x] + d->rooms[room].size[dim_x] - 1);
        } while (d->object_map[p[dim_y]][p[dim_x]] != nullptr);  // Ensure no overlap

        // Assign position and place in the dungeon
        it->position[dim_y] = p[dim_y];
        it->position[dim_x] = p[dim_x];
        d->object_map[p[dim_y]][p[dim_x]] = it;
    }
}

int16_t *position_obj(object *i)
{
    return i->position;
}

void delete_obj(dungeon_t *d)
{
    for (uint32_t y = 0; y < DUNGEON_Y; y++)
    {
        for (uint32_t x = 0; x < DUNGEON_X; x++)
        {
            if (d->object_map[y][x])
            {
                delete d->object_map[y][x];
                d->object_map[y][x] = nullptr;
            }
        }
    }
}
