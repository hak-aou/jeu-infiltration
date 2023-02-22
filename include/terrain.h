#ifndef __TERRAIN__
#define __TERRAIN__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <time.h>
#include <math.h>

#include <MLV/MLV_all.h>

#define TAILLE_PORTE 3

#define TAILLE_CASE 17
#define NUMBER_CASE_COL 60
#define NUMBER_CASE_LIG 45

#define POS_ORIGIN_PLAYER_X (TAILLE_CASE * 2)
#define POS_ORIGIN_PLAYER_Y (TAILLE_CASE * 2)

#define DISTANCE_CASE_FROM_PLAYER 20

enum orientation {
    HORIZONTAL,
    VERTICAL
};

enum object {
    EMPTY,
    WALL,
    MANA,
    DOOR,
    RELIC,
    NO_RELIC,
    DETECTED_NO_RELIC
};


/**
 * @brief Checks if the allocation went well
 * 
 * @param terrain 
 */
void check_allocation(int** terrain);

/**
 * @brief Free the two-dimensional array
 * 
 * @param terrain 
 */
void free_terrain(int** terrain);

/**
 * @brief We initialize the terrain of size NUMBER_CASE_LIG, NUMBER_CASE_COL
 *        which we fill with mana boxes and walls only on the edges
 * 
 * @return int** Return terrain
 */
int** initialize_terrain();


/**
 * @brief Returns a random integer between two values [min, max[
 * 
 * @param min 
 * @param max 
 * @return int 
 */
int random_range(int min, int max);

/**
 * @brief Function that draws a door on wall between two coordinates given in parameter.
 * 
 * @param terrain 
 * @param first 
 * @param second 
 * @param wall_pos 
 * @param orientation 
 */
void trace_porte(int** terrain, int first, int second, int wall_pos, int orientation);

/**
 * @brief Draws a wall on the terrain in VERTICAL or HORIZONTAL orientation
 * 
 * @param terrain 
 * @param x1 
 * @param y1 
 * @param x2 
 * @param y2 
 * @param orientation 
 * @param minside 
 * @return int 
 */
int trace_wall(int** terrain, int x1, int y1, int x2, int y2, int orientation, int minside);

/**
 * @brief Function that implements the given algorithm
 * 
 * @param terrain 
 * @param x1 
 * @param y1 
 * @param x2 
 * @param y2 
 * @param minside 
 */
void generation_wall(int** terrain, int x1, int y1, int x2, int y2, int minside);

/**
 * @brief Simple function which makes it possible to know
 *        if two coordinates line,column does not leave the ground.
 * 
 * @param row 
 * @param col 
 * @return int 
 */
int inside_terrain(int row, int col);

#endif