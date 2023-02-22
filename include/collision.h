#ifndef __COLLISION__
#define __COLLISION__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "player.h"
#include "terrain.h"

/**
 * @brief  Function that detects a simple collision
           between a wall and a circle on the right,
           left, top and bottom walls.
 * 
 * @param x double pointers representing the x position
            of the player or guardian
 * @param y double pointers representing the y position
            of the player or guardian
 * @param terrain 
 * @return int 1 collision
               0 no collision
 */
int collision_simple(double* x, double* y, int** terrain, int shift);


/**
 * @brief Function that detects collisions on angle
          between a wall and a circle on the top right,
          top left, bottom left and bottom right walls.
 * 
 * @param x double pointers representing the x position
            of the player and pointer on the terrain.
 * @param y double pointers representing the y position
            of the player and pointer on the terrain.
 * @param terrain 
 * @return int 1 collision
               0 no collision
 */
int collision_diagonal(double* x, double* y, int** terrain, int shift);


/**
 * @brief Function that calls collision functions and
          changes player position
 * 
 * @param player Pointer on the player
 * @param terrain Pointer on the terrain
 */
void clamp_player_position(Player* player, int** terrain);

#endif