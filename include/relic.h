#ifndef __RELIC__
#define __RELIC__

#include "terrain.h"
#include "player.h"
#include "game_manager.h"


/**
 * @brief Place the right number of relics on the ground,
 *        avoiding placing them on the walls, a door or another relic.
 *        Also places relics 20 squares away from the player
 * 
 * @param terrain 
 * @param number_relics 
 * @param player_x 
 * @param player_y 
 */
void place_relics(int** terrain, int number_relics, double player_x, double player_y);

/**
 * @brief Checks if the player has collected all the relics
 * 
 * @param counts_relics 
 * @param number_total_relics 
 * @return int 
 */
int player_collect_all_relics(int counts_relics, int number_total_relics);

/**
 * @brief If the player steps on a relic then it is recovered by replacing
 *        the box with the value NO_RELICS
 * 
 * @param player 
 * @param terrain 
 * @param counter 
 */
void player_get_relic(Player* player, int** terrain, int* counter);

#endif