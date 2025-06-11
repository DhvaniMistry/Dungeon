#ifndef IO_H
# define IO_H
#include "object.h"
class dungeon;

void io_init_terminal(void);
void io_reset_terminal(void);
void io_display(dungeon *d);
void io_handle_input(dungeon *d);
void io_queue_message(const char *format, ...);
void select_monster(dungeon *d);

uint32_t destroy(dungeon *d);
uint32_t key_inv(dungeon *d);
void show_inventory(dungeon *d);
uint32_t inventory_list(dungeon *d);
uint32_t remove(dungeon *d);
uint32_t dislay_inv_info(object *o);
uint32_t dropp(dungeon *d);
void whats_in_inv(dungeon *d);
uint32_t wear_equip(dungeon *d);
void object_to_string(object *o, char *s, uint32_t size);

#endif
