#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 80
#define HEIGHT 21
#define MIN_ROOMS 6
#define MAX_ROOMS 10
#define MIN_ROOM_WIDTH 4
#define MIN_ROOM_HEIGHT 3

typedef struct {
    int x, y, w, h;
} Room;

char dungeon[HEIGHT][WIDTH];
Room rooms[MAX_ROOMS];
int room_count = 0;

//making the border and blank spaces in between
void initialize_dungeon() {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (y == 0 || y == HEIGHT - 1) {
                dungeon[y][x] = '-'; 
            } else if (x == 0 || x == WIDTH - 1) {
                dungeon[y][x] = '|';
            } else {
                dungeon[y][x] = ' ';
            }
        }
    }
}

//1 to make sure that there is some space between the rooms,
//making sure room don't overlap
int room_cover_size(int x, int y, int w, int h) {
    if (x + w >= WIDTH - 1 || y + h >= HEIGHT - 1) return 0;
    for (int i = y - 1; i < y + h + 1; i++) {
        for (int j = x - 1; j < x + w + 1; j++) {
            if (dungeon[i][j] != ' ') return 0;
        }
    }
    return 1;
}

//puts "." at room
void place_room(int x, int y, int w, int h) {
    for (int i = y; i < y + h; i++) {
        for (int j = x; j < x + w; j++) {
            dungeon[i][j] = '.';
        }
    }
    rooms[room_count++] = (Room){x, y, w, h};
}

//calls the previous method to properly create the room.
void generate_rooms() {
    int attempts = 0;
    while (room_count < MIN_ROOMS && attempts < 1000) {
        int w = MIN_ROOM_WIDTH + rand() % (WIDTH / 6);
        int h = MIN_ROOM_HEIGHT + rand() % (HEIGHT / 4);
        int x = 1 + rand() % (WIDTH - w - 2);
        int y = 1 + rand() % (HEIGHT - h - 2);

        if (room_cover_size(x, y, w, h)) {
            place_room(x, y, w, h);
        }
        attempts++;
    }
}

//connecting rooms by creating corridor using "#"
void connect_rooms() {
    for (int i = 1; i < room_count; i++) {
        int x1 = rooms[i - 1].x + rooms[i - 1].w / 2;
        int y1 = rooms[i - 1].y + rooms[i - 1].h / 2;
        int x2 = rooms[i].x + rooms[i].w / 2;
        int y2 = rooms[i].y + rooms[i].h / 2;

        while (x1 != x2) {
            dungeon[y1][x1] = '#'; // Corridor
            x1 += (x2 > x1) ? 1 : -1;
        }
        while (y1 != y2) {
            dungeon[y1][x1] = '#';
            y1 += (y2 > y1) ? 1 : -1;
        }
    }
}

//random stair ">", "<" placement
void place_stairs() {
    int up = rand() % room_count;
    int down = rand() % room_count;
    while (down == up) down = rand() % room_count;

    int up_x = rooms[up].x + rooms[up].w / 2;
    int up_y = rooms[up].y + rooms[up].h / 2;
    dungeon[up_y][up_x] = '<';

    int down_x = rooms[down].x + rooms[down].w / 2;
    int down_y = rooms[down].y + rooms[down].h / 2;
    dungeon[down_y][down_x] = '>';
}

//printing
void print_dungeon() {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            putchar(dungeon[y][x]);
        }
        putchar('\n');
    }
}

int main() {
    srand(time(NULL));
    initialize_dungeon();
    generate_rooms();
    connect_rooms();
    place_stairs();
    print_dungeon();
    return 0;
}
