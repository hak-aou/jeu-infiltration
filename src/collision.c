#include "../include/collision.h"

int collision_simple(double* x, double* y, int** terrain, int shift){
    /* Declaration and Initialization */
    int pos_row = *y / TAILLE_CASE; /* Get player row */
    int pos_col = *x / TAILLE_CASE; /* Get player column */

    int x0 = pos_col * TAILLE_CASE; /* Get the x position of the field */
    int y0 = pos_row * TAILLE_CASE; /* Get the y position of the field */
    
    int have_touched = 0;

    assert(x != NULL);
    assert(y != NULL);
    assert(terrain != NULL);

    /* Left Wall */
    if(inside_terrain(pos_row, pos_col - shift) && terrain[pos_row][pos_col - shift] == WALL && *x < x0 + (double) TAILLE_CASE / 2){
        *x = x0 + (double) TAILLE_CASE / 2;
        have_touched = 1;
    }

    /* Right Wall */
    if(inside_terrain(pos_row, pos_col + shift) && terrain[pos_row][pos_col + shift] == WALL && *x > x0 + (double) TAILLE_CASE / 2){
        *x = x0 + (double) TAILLE_CASE / 2;
        have_touched = 1;
    }
    
    /* Down Wall */
    if(inside_terrain(pos_row + shift, pos_col) && terrain[pos_row + shift][pos_col] == WALL && *y > y0 + (double) TAILLE_CASE / 2){
        *y = y0 + (double) TAILLE_CASE / 2;
        have_touched = 1;
    }

    /* Up Wall */
    if(inside_terrain(pos_row - shift, pos_col) && terrain[pos_row - shift][pos_col] == WALL && *y < y0 + (double) TAILLE_CASE / 2){
        *y = y0 + (double) TAILLE_CASE / 2;
        have_touched = 1;
    }

    return have_touched;
}

int collision_diagonal(double* x, double* y, int** terrain, int shift){
    double y_max;
    int pos_row = *y / TAILLE_CASE; /* Get player row */
    int pos_col = *x / TAILLE_CASE; /* Get player column */

    int x0 = pos_col * TAILLE_CASE; /* Get the x position of the field */
    int y0 = pos_row * TAILLE_CASE; /* Get the y position of the field */
    
    int have_touched = 0;

    assert(x != NULL);
    assert(y != NULL);
    assert(terrain != NULL);

    /* Bottom Left Wall */
    if(inside_terrain(pos_row + shift, pos_col - shift) && terrain[pos_row + shift][pos_col - shift] == WALL && *x < x0 + (double) TAILLE_CASE / 2){
        y_max = y0 + TAILLE_CASE - sqrt( (TAILLE_CASE / 4 * TAILLE_CASE) - pow(*x - x0, 2) );
        if(*y > y_max){
            *y = y_max;
            have_touched = 1;
        }
    }

    /* Bottom Right Wall */
    if(inside_terrain(pos_row + shift, pos_col + shift) && terrain[pos_row + shift][pos_col + shift] == WALL && *x > x0 + (double) TAILLE_CASE / 2){
        y_max = y0 + TAILLE_CASE - sqrt( (TAILLE_CASE / 4 * TAILLE_CASE) - pow(x0 + TAILLE_CASE - *x, 2) );
        if(*y > y_max){
            *y = y_max;
            have_touched = 1;
        }
    }

    
    /* Top Right Wall */
    if(inside_terrain(pos_row - shift, pos_col + shift) && terrain[pos_row - shift][pos_col + shift] == WALL && *x > x0 + (double) TAILLE_CASE / 2){
        y_max = y0 + sqrt((TAILLE_CASE / 4 * TAILLE_CASE) - pow(x0 + TAILLE_CASE - *x, 2));
        if(*y < y_max){
            *y = y_max;
            have_touched = 1;
        }
    }

    /* Top Left Wall */
    if(inside_terrain(pos_row - shift, pos_col - shift) && terrain[pos_row - shift][pos_col - shift] == WALL && *x < x0 + (double) TAILLE_CASE / 2){
        y_max = y0 + sqrt((TAILLE_CASE / 4 * TAILLE_CASE) - pow(*x - x0, 2));
        if(*y < y_max){
            *y = y_max;
            have_touched = 1;
        }
    }

    return have_touched;
}


void clamp_player_position(Player* player, int** terrain){
    double new_x = player->x;
    double new_y = player->y;
    
    assert(player != NULL);
    assert(terrain != NULL);

    collision_simple(&new_x, &new_y, terrain, 1);
    collision_diagonal(&new_x, &new_y, terrain, 1);
    
    /* Change position of the player */
    player->x = new_x;
    player->y = new_y;
}
