// By Max Whitehouse 27/02/2022.
// A brute force method to calculate the maximum score obtainable in the game "Luck Be A Landlord"
// using a flower based plan and without items.

// This looks a bit odd on reflection. Why not use a macro in at_flower? Also hard coding
// grid_to_string is some kind of bad choice. It does do the thing for a quickly written
// solver thought.

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>



#define WIDTH 5
#define HIGHT 5
#define STRING_BUFFER_SIZE 8192

#define FLOWER_BASE 1
#define SUN_BASE 3

enum Symbol {
    FLOWER,
    SUN,
    NUMBER_OF_TYPES
};

typedef enum Symbol Grid[WIDTH][HIGHT];

/* The x index (first) is the symbol you are at. The y index (second) is the symbol effecting the
 * symbol you are at. */
const int MULTI_MAP[NUMBER_OF_TYPES][NUMBER_OF_TYPES] = {{1, 5}, {1, 1}};



char * grid_to_string(char buffer[STRING_BUFFER_SIZE], Grid grid) {
    int pos = 0;
    const char *board_format =
        "[%d, %d, %d, %d, %d]\n"
        "[%d, %d, %d, %d, %d]\n"
        "[%d, %d, %d, %d, %d]\n"
        "[%d, %d, %d, %d, %d]\n"
        "[%d, %d, %d, %d, %d]\n"
    ;
    sprintf(buffer, board_format, 
        grid[0][0], grid[1][0], grid[2][0], grid[3][0], grid[4][0],
        grid[0][1], grid[1][1], grid[2][1], grid[3][1], grid[4][1],
        grid[0][2], grid[1][2], grid[2][2], grid[3][2], grid[4][2],
        grid[0][3], grid[1][3], grid[2][3], grid[3][3], grid[4][3],
        grid[0][4], grid[1][4], grid[2][4], grid[3][4], grid[4][4]
    );

    return buffer;
}

void init_grid(Grid grid) {
    for (int y = 0; y < HIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            grid[x][y] = 0;
        }
    }
}

bool make_new_grid(Grid grid) {
    for (int y = 0; y < HIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            grid[x][y] += 1;
            if (grid[x][y] != NUMBER_OF_TYPES) {
                return true;
            }
            grid[x][y] = FLOWER;
        }
    }
    
    return false;
}

long long at_flower(Grid grid, int x, int y) {
    long long score = FLOWER_BASE;

    // Origin top left. x increasing left to right (0 to WIDTH). y increasing top to bottom (0 to 
    // HIGHT).

    // Left middle.
    if (x > 0) {
        score *= MULTI_MAP[FLOWER][grid[x - 1][y]];
    }

    // Top left corner. 
    if (x > 0 && y > 0) {
        score *= MULTI_MAP[FLOWER][grid[x - 1][y - 1]];
    }

    // Top middle.
    if (y > 0) {
        score *= MULTI_MAP[FLOWER][grid[x][y - 1]];
    }
    
    // Top right corner.
    if (x + 1 < WIDTH && y > 0) {
        score *= MULTI_MAP[FLOWER][grid[x + 1][y - 1]];
    }

    // Middle right.
    if (x + 1 < WIDTH) {
        score *= MULTI_MAP[FLOWER][grid[x + 1][y]];
    }

    // Bottom right corner.
    if (x + 1 < WIDTH && y + 1 < HIGHT) {
        score *= MULTI_MAP[FLOWER][grid[x + 1][y + 1]];
    }

    // Bottom middle.
    if (y + 1 < HIGHT) {
        score *= MULTI_MAP[FLOWER][grid[x][y + 1]];
    }

    // Bottom left corner.
    if (x > 0 && y + 1 < HIGHT) {
        score *= MULTI_MAP[FLOWER][grid[x][y - 1]];
    }
    
    return score;
}

long long at_sun(Grid grid, int x, int y) {
    long long score = SUN_BASE;

    // No multiplier affect sun so return score.
    return score;
}

long long calc_score(Grid grid) {
    long long score = 0;
    for (int y = 0; y < HIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            switch (grid[x][y]) {
                case FLOWER: score += at_flower(grid, x, y); break;
                case SUN: score += at_sun(grid, x, y); break;
                default: 
                    printf("Unrecognised symbol: %d.", grid[x][y]);
                    exit(1);
            }
        }
    }

    return score;
}

void run_sim(Grid grid, Grid best_grid) {
    long long max_score = -1ll;
    long long loop_count = -1ll;

    init_grid(grid);
    init_grid(best_grid);

    printf("Search loop starting.\n");

    while (make_new_grid(grid)) {
        long long score = calc_score(grid); 

        if (score > max_score) {
            max_score = score;
            memcpy(best_grid, grid, sizeof(Grid));
            char string_buffer[STRING_BUFFER_SIZE];
            printf("Loop count: %d\nHigh score: %lld\n%s\n", loop_count, score, grid_to_string(string_buffer, best_grid)); 
        }

        if (++loop_count % 1000000 == 0) {
            printf("Loop count: %lld.\n", loop_count);
        }
    }

    char string_buffer[STRING_BUFFER_SIZE];
    printf("The best score is: %lld.\n", max_score);
    printf("The best Grid is: \n%s\n", grid_to_string(string_buffer, best_grid));
}



int main(int argc, char const *argv[]) {
    enum Symbol best_grid[WIDTH][HIGHT];
    enum Symbol grid[WIDTH][HIGHT];

    run_sim(grid, best_grid);

    // char string_buffer[STRING_BUFFER_SIZE];
    // printf("The best grid is: \n%s\n", grid_to_string(string_buffer, best_grid));

    return 0;
}
