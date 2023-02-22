#include "../include/player.h"

/**
 * @brief If the speed exceeds a certain value
 *        then its blocks the speed of the player
 *        on this one
 * 
 * @param player 
 */
static void clamp_speed(Player* player){
    double max_speed;

    assert(player != NULL);

    if(player->power.is_accelerated == 1)
        max_speed = POWER_MAX_SPEED;
    else
        max_speed = MAXIMAL_SPEED;

    /* Block at maximum speed */
    if(player->speed >= max_speed * SPEED)
        player->speed = max_speed * SPEED;
}

void initialize_player(Player* player){
    assert(player != NULL);

    player->x = POS_ORIGIN_PLAYER_X;
    player->y = POS_ORIGIN_PLAYER_Y;
    player->speed = INITIAl_SPEED;
    player->mana = 0;
    player->power.is_accelerated = 0;
    player->power.is_invisible = 0;
}

int change_speed(Player* player, Direction_Vertical new_vertical, Direction_Horizontal new_horizontal){
    assert(player != NULL);

    /* Player change direction so we change speed */
    if(new_vertical != player->direction_vertical || new_horizontal != player->direction_horizontal){
        player->speed = INITIAl_SPEED * SPEED;
        
        player->direction_vertical = new_vertical;
        player->direction_horizontal = new_horizontal;
    }

    clamp_speed(player);

    /* Acceleration */
    player->speed += ACCELERATION * SPEED;

    return 1;
}

void movement_player(Player* player, int** terrain){
    int is_in_movement = 0;

    assert(player != NULL);
    assert(terrain != NULL);

    if(MLV_get_keyboard_state( MLV_KEYBOARD_z ) == MLV_PRESSED){
        is_in_movement = change_speed(player, NORTH, player->direction_horizontal);
        player->y -= player->speed;
    }

    if(MLV_get_keyboard_state( MLV_KEYBOARD_s ) == MLV_PRESSED){
        is_in_movement = change_speed(player, SOUTH, player->direction_horizontal);
        player->y += player->speed;   
    }

    if(MLV_get_keyboard_state( MLV_KEYBOARD_d ) == MLV_PRESSED){
        is_in_movement = change_speed(player, player->direction_vertical, EAST);
        player->x += player->speed;
    }

    if(MLV_get_keyboard_state( MLV_KEYBOARD_q ) == MLV_PRESSED){
        is_in_movement = change_speed(player, player->direction_vertical, WEST);
        player->x -= player->speed;
    }

    if(is_in_movement == 0){
        player->speed = ACCELERATION * SPEED;

        player->direction_horizontal = NONE_H;
        player->direction_vertical = NONE_V;
    }
}

void choose_skill(Player* player, int* total_mana_used){
    assert(player != NULL);
    assert(total_mana_used != NULL);

    player->power.is_accelerated = 0;
    player->power.is_invisible = 0;
    
    if( (MLV_get_keyboard_state(MLV_KEYBOARD_LSHIFT) == MLV_PRESSED ||
       MLV_get_keyboard_state(MLV_KEYBOARD_RSHIFT) == MLV_PRESSED) &&
       player->mana > 2){

        player->power.is_accelerated = 1;
        player->mana -= 2;
        *total_mana_used += 2;
    }

    if(MLV_get_keyboard_state(MLV_KEYBOARD_SPACE) == MLV_PRESSED &&
       player->mana > 1){

        player->power.is_invisible = 1;
        player->mana -= 1;
        *total_mana_used += 1;
    }
}