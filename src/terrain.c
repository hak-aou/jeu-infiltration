#include "../include/terrain.h"

void check_allocation(int** terrain){
    assert(terrain != NULL);

    if(terrain == NULL){
        fprintf(stderr, "Error problem on allocation\n");
        exit(EXIT_FAILURE);
    }
}

void free_terrain(int** terrain){
    int i;
    for(i = 0; i < NUMBER_CASE_LIG; i++){
        free(terrain[i]);
        terrain[i] = NULL;
    }

    free(terrain);
    terrain = NULL;
}

int** initialize_terrain(){
    int i, j;
    int** terrain;

    /* Create NUMBER_CASE_LIG array of int in an array */
    terrain = (int **) malloc(sizeof(int *) * NUMBER_CASE_LIG);
    check_allocation(terrain);

    for (i = 0 ; i < NUMBER_CASE_LIG ; ++i) {

        /* We fill each array with an array of int size NUMBER_CASE_COL*/
        terrain[i] = (int *) malloc(sizeof(int) * NUMBER_CASE_COL);
        check_allocation(&terrain[i]);
        
        for (j = 0 ; j < NUMBER_CASE_COL ; ++j)
            /* We implement exterior walls with WALL enum*/
            if( (i == 0 || j == 0 ) || (i == NUMBER_CASE_LIG - 1 || j == NUMBER_CASE_COL - 1) )
                terrain[i][j] = WALL;
            else
                terrain[i][j] = MANA;
    }

    return terrain;
}

int random_range(int min, int max){
    return rand() % (max - min + 1) + min;
}

void trace_porte(int** terrain, int first, int second, int wall_pos, int orientation){
    int pos_begin, pos_end;
    int porte, i;

    assert(terrain != NULL);

    /* Determine door position up or down */
    porte = rand()%2;
    
    /* Determine the positions where to draw door */
    if(porte) {
        pos_begin = first;
        pos_end = first + TAILLE_PORTE;
    } else {
        pos_begin = second - TAILLE_PORTE;
        pos_end = second;
    }

    /* Trace the door */
    for(i = pos_begin; i < pos_end; i++){
        if(orientation == VERTICAL)
            terrain[i][wall_pos] = DOOR;
        else
            terrain[wall_pos][i] = DOOR;
    }
}

int trace_wall(int** terrain, int x1, int y1, int x2, int y2, int orientation, int minside){
    int mur_pos;
    int deb, fin;
    int i;

    assert(terrain != NULL);

    if(orientation == VERTICAL){
        mur_pos = random_range(x1 + minside, x2 - minside);
        deb = y1;
        fin = y2;
    } else {
        mur_pos = random_range(y1 + minside, y2 - minside);
        deb = x1;
        fin = x2;
    }


    /* Generate the horizontal wall */
    for(i = deb; i < fin; i++) {
        if(orientation == VERTICAL)
            terrain[i][mur_pos] = WALL;
        else
            terrain[mur_pos][i] = WALL;
    }
    
    trace_porte(terrain, deb, fin, mur_pos, orientation);

    return mur_pos;
}


void generation_wall(int** terrain, int x1, int y1, int x2, int y2, int minside){
    int mur_lig, mur_col;
    
    assert(terrain != NULL);
    
    /* Inverse x and y according to the superiority between them */
    if(x2 > y2){
        /* Espace libre trop petit */
        if(x2 - x1 < (2 * minside) + 1){
            return;
        }

        /* Finish with probability 1/2 */
        if(x2 - x1 < 4 * minside){
            if(rand() % 2)
                return;
        }

        mur_col = trace_wall(terrain, x1, y1, x2, y2, VERTICAL, minside);

        generation_wall(terrain, x1, y1, mur_col, y2, minside);
        generation_wall(terrain, mur_col + 1, y1, x2, y2, minside);
    } else {
        /* It means the free space is too small */
        if(y2 - y1 < (2 * minside) + 1){
            return;
        }

        /* Finish with 1/2 probability  */
        if(y2 - y1 < 4 * minside){
            if(rand() % 2)
                return;
        }

        mur_lig = trace_wall(terrain, x1, y1, x2, y2, HORIZONTAL, minside);

        generation_wall(terrain, x1, y1, x2, mur_lig, minside);
        generation_wall(terrain, x1, mur_lig + 1, x2, y2, minside);
    }
}

int inside_terrain(int row, int col){
    if(0 <= col && col <= NUMBER_CASE_COL - 1 &&
       0 <= row && row <= NUMBER_CASE_LIG -1)
        return 1;
    return 0;
}