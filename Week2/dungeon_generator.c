#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

#define WIDTH 80
#define HEIGHT 21
#define MIN_ROOMS 6
#define MAX_ROOMS 10
#define MIN_ROOM_WIDTH 4
#define MIN_ROOM_HEIGHT 3
#define FILE_HEADER "RLG327-S2025"
#define FILE_VERSION 0

typedef struct {
    int x, y, w, h;
} Room;

char dungeon[HEIGHT][WIDTH];
Room rooms[MAX_ROOMS];
int room_count = 0;
int pc_x, pc_y;
uint8_t hardness[HEIGHT][WIDTH];


//making the border and blank spaces in between
void initialize_dungeon() {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (y == 0 || y == HEIGHT - 1 || x == 0 || x == WIDTH - 1) {
                dungeon[y][x] = (y == 0 || y == HEIGHT - 1) ? '-' : '|';
                hardness[y][x] = 255;  // Maximum hardness for immutable border
            } else {
                dungeon[y][x] = ' ';
                hardness[y][x] = 255;  // Default rock hardness
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
            hardness[i][j] = 0;  // Room floors have zero hardness
        }
    }
    rooms[room_count++] = (Room){x, y, w, h};
}


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
    pc_x = rooms[0].x + 1;
    pc_y = rooms[0].y + 1;
    dungeon[pc_y][pc_x] = '@';
}


//connecting rooms by creating corridor using "#"
void connect_rooms() {
    for (int i = 1; i < room_count; i++) {
        int x1 = rooms[i - 1].x + rooms[i - 1].w / 2;
        int y1 = rooms[i - 1].y + rooms[i - 1].h / 2;
        int x2 = rooms[i].x + rooms[i].w / 2;
        int y2 = rooms[i].y + rooms[i].h / 2;

        while (x1 != x2) {
            dungeon[y1][x1] = '#';
            hardness[y1][x1] = 1;  // Corridors have zero hardness
            x1 += (x2 > x1) ? 1 : -1;
        }
        while (y1 != y2) {
            dungeon[y1][x1] = '#';
            hardness[y1][x1] = 1;
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
    hardness[up_y][up_x] = 3;  // Assigning a hardness value for the "up" stair

    int down_x = rooms[down].x + rooms[down].w / 2;
    int down_y = rooms[down].y + rooms[down].h / 2;
    dungeon[down_y][down_x] = '>';
    hardness[down_y][down_x] = 2;  // Assigning a hardness value for the "down" stair
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

char *get_dungeon_file_path() {
    char *home = getenv("HOME");
    if (!home) {
        fprintf(stderr, "Error: HOME environment variable not set.\n");
        exit(1);
    }

    static char path[256];
    snprintf(path, sizeof(path), "%s/.rlg327", home);

    struct stat st = {0};
    if (stat(path, &st) == -1) {
        mkdir(path, 0700);  // Create the directory if it doesn't exist
    }

    snprintf(path, sizeof(path), "%s/.rlg327/dungeon", home);
    return path;
}

void save_dungeon() {
    FILE *f = fopen(get_dungeon_file_path(), "wb");
    if (!f) {
        perror("Error opening file for saving");
        return;
    }
    fwrite(FILE_HEADER, 1, 12, f);
    uint32_t version = htonl(FILE_VERSION);
    fwrite(&version, 4, 1, f);
    uint32_t size = htonl(1708 + room_count * 4);
    fwrite(&size, 4, 1, f);
    fputc(pc_x, f);
    fputc(pc_y, f);
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            fputc(dungeon[y][x] == ' ' ? 255 : 0, f);
        }
    }
    uint16_t r = htons(room_count);
    fwrite(&r, 2, 1, f);
    for (int i = 0; i < room_count; i++) {
        fputc(rooms[i].x, f);
        fputc(rooms[i].y, f);
        fputc(rooms[i].w, f);
        fputc(rooms[i].h, f);
    }
    fclose(f);
}

void load_dungeon() {
    FILE *f = fopen(get_dungeon_file_path(), "rb");
    if (!f) {
        printf("No saved dungeon found. Generating a new one.\n");
        initialize_dungeon();
        generate_rooms();
        return;
    }
    char header[13] = {0};
    fread(header, 1, 12, f);
    if (strcmp(header, FILE_HEADER) != 0) {
        fprintf(stderr, "Invalid file format.\n");
        fclose(f);
        exit(1);
    }
    fseek(f, 20, SEEK_SET);
    pc_x = fgetc(f);
    pc_y = fgetc(f);
    fseek(f, 22, SEEK_SET);
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            int h = fgetc(f);  // Read the hardness value

            // Set the correct character based on the hardness
            if (h == 255) {
                dungeon[y][x] = ' ';  // Wall (hardness 255)
            } else if (h == 0) {
                dungeon[y][x] = '.';  // Room floor (hardness 0)
            } else if (h == 1) {
                dungeon[y][x] = '#';  // Corridor (hardness 1)
            } else if (h == 2) {
                dungeon[y][x] = '>';  // Down stair (hardness 2)
            } else if (h == 3) {
                dungeon[y][x] = '<';  // Up stair (hardness 3)
            }
        }
    }
    
    fclose(f);
    dungeon[pc_y][pc_x] = '@';
}


int main(int argc, char *argv[]) {
    srand(time(NULL));
    struct stat st = {0};
    if (stat(get_dungeon_file_path(), &st) == -1) {
        mkdir(get_dungeon_file_path(), 0700);
    }
    int save = 0, load = 0;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--save") == 0) save = 1;
        else if (strcmp(argv[i], "--load") == 0) load = 1;
    }
    if (load) load_dungeon();
    else { initialize_dungeon(); generate_rooms(); connect_rooms(); place_stairs(); }
    print_dungeon();
    if (save) save_dungeon();
    return 0;
}
