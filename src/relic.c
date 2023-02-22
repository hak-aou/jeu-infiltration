#include "../include/relic.h"

void place_relics(int** terrain, int number_relics, double player_x, double player_y){
    double rand_x, rand_y;
    int row, col;
    int distance_cases;
    int i;

    assert(terrain != NULL);
    assert(number_relics >= 0);


    for(i = 0; i < number_relics; i++){
        do {
            rand_x = rand() % (TAILLE_CASE * NUMBER_CASE_COL);
            rand_y = rand() % (TAILLE_CASE * NUMBER_CASE_LIG);

            distance_cases = euclidean_distance(rand_x, rand_y, player_x, player_y) / TAILLE_CASE;

            row = rand_y / TAILLE_CASE;
            col = rand_x / TAILLE_CASE;

        } while(distance_cases < DISTANCE_CASE_FROM_PLAYER ||
                    terrain[row][col] == DOOR ||
                    terrain[row][col] == WALL ||
                    terrain[row][col] == RELIC);
                
        terrain[row][col] = RELIC;
    }
}

int player_collect_all_relics(int counts_relics, int number_total_relics){
    return counts_relics >= number_total_relics;
}

void player_get_relic(Player* player, int** terrain, int* counter){
    /* We get the row and column position of player */
    int player_row = player->y / TAILLE_CASE;
    int player_column = player->x / TAILLE_CASE;
    
    assert(player != NULL);
    assert(terrain != NULL);
    assert(counter != NULL);

    if(terrain[player_row][player_column] == RELIC){
        terrain[player_row][player_column] = NO_RELIC;
        (*counter)++;
    }
}
