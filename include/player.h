#ifndef __PLAYER__
#define __PLAYER__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <MLV/MLV_all.h>

#include "terrain.h"

#define SPEED 5
#define INITIAl_SPEED 0.1
#define ACCELERATION 0.03

#define MAXIMAL_SPEED 0.9
#define POWER_MAX_SPEED 1.2

typedef struct {
    int is_accelerated;
    int is_invisible;
} Power;

typedef enum {
    NONE_V,
    NORTH,
    SOUTH
} Direction_Vertical;

typedef enum {
    NONE_H,
    EAST,
    WEST
} Direction_Horizontal;

typedef struct {
    double x;
    double y;
    double speed;
    int mana;
    Power power;
    Direction_Vertical direction_vertical;
    Direction_Horizontal direction_horizontal;
} Player;

/**
 * @brief Function that initializes the player with
 *        basic parameters such as his initial position
 *        and his empty mana
 * 
 * @param player 
 */
void initialize_player(Player* player);

/**
 * @brief The function detects if the player has changed direction.
 *        If he changes direction then we put the initial base speed back
 * 
 * @param player 
 * @param new_vertical 
 * @param new_horizontal 
 * @return int 
 */
int change_speed(Player* player, Direction_Vertical new_vertical,
                                 Direction_Horizontal new_horizontal);

/**
 * @brief Makes the player move in a direction depending
 *        on the key they press
 * 
 * @param player 
 * @param terrain 
 */
void movement_player(Player* player, int** terrain);

/**
 * @brief Let the player choose their ability with either the
 *        Space key for invisibility or Shift for speed
 * 
 * @param player 
 * @param total_mana_used 
 */
void choose_skill(Player* player, int* total_mana_used);

#endif