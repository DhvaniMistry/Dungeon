#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>


#include "dungeon.h"
#include "path.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define TRUE  1
#define FALSE 0

void usage(char *name)
{
  fprintf(stderr,
          "Usage: %s [-r|--rand <seed>] [-l|--load [<file>]]\n"
          "          [-s|--save [<file>]] [-i|--image <pgm file>]\n",
          name);

  exit(-1);
}

// Define the directions
typedef enum {
    up,
    down,
    left,
    right,
    up_left,
    up_right,
    down_left,
    down_right
} direction_t;


// Function to generate a random number within a range (inclusive)
int rand_range(int min, int max) {
    return min + rand() % (max - min + 1);
}

// Helper function to check if coordinates are within the dungeon bounds
bool is_within_bounds(int x, int y) {
    return x >= 0 && x < 80 && y >= 0 && y < 21; // Assuming 80x21 dungeon size
}

int is_t_mon(dungeon_t *d, int x, int y);
int is_nt_mon(dungeon_t *d, int x, int y);

// Helper function to handle PC death.
void handle_pc_death(dungeon_t *d) {
    if (d->pc.alive) {
        printf("The PC has died!\n"); // Optional message.
        d->pc.alive = false;
    }
}

void move_pc(dungeon_t *d){
  int r = rand_range(0,7); //There are 8 directions so the rand_range must be 0 to 7

  int new_x = d->pc.position[0];
  int new_y = d->pc.position[1];
  
  switch(r){
    case up: // [ 0, -1]
        new_y--;
        break;

    case down: // [ 0, 1]
        new_y++;
        break;
      
    case left: // [ -1, 0]
        new_x--;
        break;

    case right: // [ 1, 0]
        new_x++;
        break;

    case up_left: // [ -1, -1]
        new_x--;
        new_y--;
        break;

    case up_right: // [ 1, -1]
        new_x++;
        new_y--;
        break;

    case down_left: // [ -1, 1]
        new_x--;
        new_y++;
        break;

    case down_right: // [ 1, 1]
        new_x++;
        new_y++;
        break;

    default:
      break;
  }
    //Check if new coordinates are in bounds
    if(is_within_bounds(new_x, new_y)){
        //Check is the new position is open
        if(d->hardness[new_y][new_x] == 0){
            d->pc.position[0] = new_x;
            d->pc.position[1] = new_y;
        }
    }

    // Check for collision with mons *after* PC moves:
    if (is_nt_mon(d, d->pc.position[0], d->pc.position[1]) >= 0 || is_t_mon(d, d->pc.position[0], d->pc.position[1]) >= 0) {
        handle_pc_death(d); //PC has died.
    }
}

void move_ntmon(dungeon_t *d, ntmon *ntmon) {
  int r = rand_range(0, 7); // There are 8 directions so the rand_range must be 0 to 7
  int new_x = ntmon->position[0];
  int new_y = ntmon->position[1];
  switch (r) {
    case up: // [ 0, -1]
        new_y--;
        break;

    case down: // [ 0, 1]
        new_y++;
        break;

    case left: // [ -1, 0]
        new_x--;
        break;

    case right: // [ 1, 0]
        new_x++;
        break;

    case up_left: // [ -1, -1]
        new_x--;
        new_y--;
        break;

    case up_right: // [ 1, -1]
        new_x++;
        new_y--;
        break;

    case down_left: // [ -1, 1]
        new_x--;
        new_y++;
        break;

    case down_right: // [ 1, 1]
        new_x++;
        new_y++;
        break;

    default:
        break;
    }

    // Check if the new coordinates are in bounds
    if (is_within_bounds(new_x, new_y)) {
        // Check if the new position is open
        if (d->hardness[new_y][new_x] == 0) {
            ntmon->position[0] = new_x;
            ntmon->position[1] = new_y;
        }
    }


    //Collision detection with player
    if(ntmon->position[0] == d->pc.position[0] &&
      ntmon->position[1] == d->pc.position[1] ){
        handle_pc_death(d);
    }

    // Collision detection with other ntmons
    int i;
    int count;
    count = 0;
    for (i = 0; i < d->num_nt_mons; i++) {
        if (d->ntmon[i].alive && ntmon->position[0] == d->ntmon[i].position[0] &&
            ntmon->position[1] == d->ntmon[i].position[1]) {
            count++;
        }
    }
    if (count > 1) {
        ntmon->alive = FALSE;
    }

    // Collision detection with tunnelling mons
    if (is_t_mon(d, ntmon->position[0], ntmon->position[1]) >= 0) {
        ntmon->alive = FALSE;
    }
}

#define TUNNEL_HARDNESS_REDUCTION 85 // Define the magic number

void move_tmon(dungeon_t *d, tmon *tmon) {
    int r = rand_range(0, 7); // There are 8 directions so the rand_range must be 0 to 7
    int new_x = tmon->position[0];
    int new_y = tmon->position[1];
    switch (r) {
      case up:
        new_y--;
        break;

      case down: // [ 0, 1]
        new_y++;
        break;

      case left: // [ -1, 0]
          new_x--;
          break;

      case right: // [ 1, 0]
          new_x++;
          break;

      case up_left:
        new_x--;
        new_y--;
        break;

      case up_right: // [ 1, -1]
          new_x++;
          new_y--;
          break;

      case down_left: // [ -1, 1]
        new_x--;
        new_y++;
        break;

      case down_right: // [ 1, 1]
        new_x++;
        new_y++;
        break;

      default:
        break;
    }
    if(is_within_bounds(new_x, new_y)){
      if (d->hardness[new_y][new_x] < 255) {
          if (d->hardness[new_y][new_x] <= TUNNEL_HARDNESS_REDUCTION) {
              d->hardness[new_y][new_x] = 0;
              tmon->position[0] = new_x;
              tmon->position[1] = new_y;
          } else {
              d->hardness[new_y][new_x] -= TUNNEL_HARDNESS_REDUCTION;
          }
      }
    }

    // Collision detection with player
    if (tmon->position[0] == d->pc.position[0] &&
        tmon->position[1] == d->pc.position[1]) {
        handle_pc_death(d);
    }

    // Collision detection with other tmons
    int i;
    int count;
    count = 0;
    for (i = 0; i < d->num_t_mons; i++) {
        if (d->tmon[i].alive && tmon->position[0] == d->tmon[i].position[0] &&
            tmon->position[1] == d->tmon[i].position[1]) {
            count++;
        }
    }
    if (count > 1) {
        tmon->alive = FALSE;
    }

    //collision detection with non tunnelling mons
    if (is_nt_mon(d, tmon->position[0], tmon->position[1]) >= 0) {
        tmon->alive = FALSE;
    }
}

void place_mons(dungeon_t *d) {
    // Place Non-Tunnelling mons
    int i;
    for (i = 0; i < d->num_nt_mons; i++) {
        if (i < d->num_rooms) {
            d->ntmon[i].position[0] = (d->rooms[i].position[0] + (rand() % d->rooms[i].size[0]));
            d->ntmon[i].position[1] = (d->rooms[i].position[1] + (rand() % d->rooms[i].size[1]));
        }
    }

    // Place Tunnelling mons
    for (i = 0; i < d->num_t_mons; i++) {
        d->tmon[i].position[0] = i + 1;
        d->tmon[i].position[1] = 1;
        d->hardness[d->tmon[i].position[1]][d->tmon[i].position[0]] = 0;
    }
}

int is_t_mon(dungeon_t *d, int x, int y) {
    int i;
    for (i = 0; i < d->num_t_mons; i++) {
        if (d->tmon[i].alive && d->tmon[i].position[0] == x && d->tmon[i].position[1] == y) {
            return i;
        }
    }
    return -1; //return -1 if there is no t_mon at this coordinate
}

int is_nt_mon(dungeon_t *d, int x, int y) {
    int i;
    for (i = 0; i < d->num_nt_mons; i++) {
        if (d->ntmon[i].alive && d->ntmon[i].position[0] == x && d->ntmon[i].position[1] == y) {
            return i;
        }
    }
    return -1; //return -1 if there is no nt_mon at this coordinate
}

void create_mons(dungeon_t *d, int num) {
  int i;
  int nt_size;
 
  if (num <= 0) {
      d->num_nt_mons = 0;
      d->num_t_mons = 0;
      d->ntmon = NULL;
      d->tmon = NULL;
      return;
  }

  // Ensure at least one of each type if possible.

  // Check how many nt mons there will be
  if (num <= d->num_rooms){
      nt_size = rand_range(1,num);
  } else {
      nt_size = d->num_rooms;
  }

  // Make sure to always have one non tunnelling monster;
  d->num_nt_mons = nt_size;
  d->num_t_mons = num - d->num_nt_mons;

  // Allocate memory, check if the monsters were not initializied properly
  d->ntmon = malloc(sizeof(ntmon) * d->num_nt_mons);
  d->tmon = malloc(sizeof(tmon) * d->num_t_mons);

  // Exit the program if memory is not properly allocated
  if(!d->ntmon || !d->tmon){
      printf("Not enough memory in create_mons");
      exit(1);
  }

  // If there are any issues with these values,
  //Exit program
  if(d->num_t_mons < 0 || d->num_nt_mons < 0){
      printf("Too many mons");
      exit(1);
  }

  printf("Num NT: %d\t T: %d\n", d->num_nt_mons, d->num_t_mons);

  for (i = 0; i < d->num_nt_mons; i++) {
      d->ntmon[i].position[dim_x] = 0;
      d->ntmon[i].position[dim_y] = 0;
      d->ntmon[i].alive = 1;
  }
  for (i = 0; i < d->num_t_mons; i++) {
      d->tmon[i].position[dim_x] = i + 2;
      d->tmon[i].position[dim_y] = 1;
      d->tmon[i].alive = 1;
  }

  place_mons(d);
}

void print_game_over() {
    printf("****************************\n");
    printf("*                         *\n");
    printf("*    G A M E   O V E R    *\n");
    printf("*                         *\n");
    printf("****************************\n");
}

int main(int argc, char *argv[])
{
  dungeon_t d;
  time_t seed;
  struct timeval tv;
  uint32_t i;
  uint32_t do_load, do_save, do_seed, do_image, do_save_seed, do_save_image;
  uint32_t long_arg;
  char *save_file;
  char *load_file;
  char *pgm_file;
  int num_mons = 3;

  /* Quiet a false positive from valgrind. */
  memset(&d, 0, sizeof (d));
  create_mons(&d, num_mons);
  
  /* Default behavior: Seed with the time, generate a new dungeon, *
   * and don't write to disk.                                      */
  do_load = do_save = do_image = do_save_seed = do_save_image = 0;
  do_seed = 1;
  save_file = load_file = NULL;

  /* The project spec requires '--load' and '--save'.  It's common  *
   * to have short and long forms of most switches (assuming you    *
   * don't run out of letters).  For now, we've got plenty.  Long   *
   * forms use whole words and take two dashes.  Short forms use an *
    * abbreviation after a single dash.  We'll add '--rand' (to     *
   * specify a random seed), which will take an argument of it's    *
   * own, and we'll add short forms for all three commands, '-l',   *
   * '-s', and '-r', respectively.  We're also going to allow an    *
   * optional argument to load to allow us to load non-default save *
   * files.  No means to save to non-default locations, however.    *
   * And the final switch, '--image', allows me to create a dungeon *
   * from a PGM image, so that I was able to create those more      *
   * interesting test dungeons for you.                             */
 
 if (argc > 1) {
    for (i = 1, long_arg = 0; i < argc; i++, long_arg = 0) {
      if (argv[i][0] == '-') { /* All switches start with a dash */
        if (argv[i][1] == '-') {
          argv[i]++;    /* Make the argument have a single dash so we can */
          long_arg = 1; /* handle long and short args at the same place.  */
        }
        switch (argv[i][1]) {
        case 'r':
          if ((!long_arg && argv[i][2]) ||
              (long_arg && strcmp(argv[i], "-rand")) ||
              argc < ++i + 1 /* No more arguments */ ||
              !sscanf(argv[i], "%lu", &seed) /* Argument is not an integer */) {
            usage(argv[0]);
          }
          do_seed = 0;
          break;
        case 'l':
          if ((!long_arg && argv[i][2]) ||
              (long_arg && strcmp(argv[i], "-load"))) {
            usage(argv[0]);
          }
          do_load = 1;
          if ((argc > i + 1) && argv[i + 1][0] != '-') {
            /* There is another argument, and it's not a switch, so *
             * we'll treat it as a save file and try to load it.    */
            load_file = argv[++i];
          }
          break;
        case 's':
          if ((!long_arg && argv[i][2]) ||
              (long_arg && strcmp(argv[i], "-save"))) {
            usage(argv[0]);
          }
          do_save = 1;
          if ((argc > i + 1) && argv[i + 1][0] != '-') {
            /* There is another argument, and it's not a switch, so *
             * we'll save to it.  If it is "seed", we'll save to    *
	     * <the current seed>.rlg327.  If it is "image", we'll  *
	     * save to <the current image>.rlg327.                  */
	    if (!strcmp(argv[++i], "seed")) {
	      do_save_seed = 1;
	      do_save_image = 0;
	    } else if (!strcmp(argv[i], "image")) {
	      do_save_image = 1;
	      do_save_seed = 0;
	    } else {
	      save_file = argv[i];
	    }
          }
          break;
        case 'i':
          if ((!long_arg && argv[i][2]) ||
              (long_arg && strcmp(argv[i], "-image"))) {
            usage(argv[0]);
          }
          do_image = 1;
          if ((argc > i + 1) && argv[i + 1][0] != '-') {
            /* There is another argument, and it's not a switch, so *
             * we'll treat it as a save file and try to load it.    */
            pgm_file = argv[++i];
          }
          break;
        default:
          usage(argv[0]);
        }
      } else { /* No dash */
        usage(argv[0]);
      }
    }
  }

  if (do_seed) {
    /* Allows me to generate more than one dungeon *
     * per second, as opposed to time().           */
    gettimeofday(&tv, NULL);
    seed = (tv.tv_usec ^ (tv.tv_sec << 20)) & 0xffffffff;
  }

  printf("Seed is %ld.\n", seed);
  srand(seed);

  init_dungeon(&d);

  if (do_load) {
    read_dungeon(&d, load_file);
  } else if (do_image) {
    read_pgm(&d, pgm_file);
  } else {
    gen_dungeon(&d);
  }

  if (!do_load) {
    /* Set a valid position for the PC */
    d.pc.position[dim_x] = (d.rooms[0].position[dim_x] +
                            (rand() % d.rooms[0].size[dim_x]));
    d.pc.position[dim_y] = (d.rooms[0].position[dim_y] +
                            (rand() % d.rooms[0].size[dim_y]));
  }

  printf("PC is at (y, x): %d, %d\n",
         d.pc.position[dim_y], d.pc.position[dim_x]);

         printf("PC is at (y, x): %d, %d\n",
          d.pc.position[dim_y], d.pc.position[dim_x]);
 
   d.pc.alive = TRUE;
 
   int counter = 0;
 
   while(d.pc.alive){
     usleep(300000);
     move_pc(&d);
     render_dungeon(&d);
     dijkstra(&d);
     dijkstra_tunnel(&d);
     int i;
     for(i = 0; i < d.num_nt_mons; i++){
         move_ntmon(&d, &d.ntmon[i]);
     }
       for(i = 0; i < d.num_t_mons; i++){
           move_tmon(&d, &d.tmon[i]);
       }
     counter++;
   }

   print_game_over();

  // render_dungeon(&d);

  // dijkstra(&d);
  // dijkstra_tunnel(&d);
  // render_distance_map(&d);
  // render_tunnel_distance_map(&d);
  // render_hardness_map(&d);
  // render_movement_cost_map(&d);

  if (do_save) {
    if (do_save_seed) {
       /* 10 bytes for number, plus dot, extention and null terminator. */
      save_file = malloc(18);
      sprintf(save_file, "%ld.rlg327", seed);
    }
    if (do_save_image) {
      if (!pgm_file) {
	fprintf(stderr, "No image file was loaded.  Using default.\n");
	do_save_image = 0;
      } else {
	/* Extension of 3 characters longer than image extension + null. */
	save_file = malloc(strlen(pgm_file) + 4);
	strcpy(save_file, pgm_file);
	strcpy(strchr(save_file, '.') + 1, "rlg327");
      }
    }
    write_dungeon(&d, save_file);

    if (do_save_seed || do_save_image) {
      free(save_file);
    }
  }

  delete_dungeon(&d);

  return 0;
}
