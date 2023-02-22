#include "../include/game_manager.h"


void player_get_mana(Player* player, int** terrain, List* lst_case_mana){
    /* We get the row and column position of player */
    Case mana_case;
    int player_row = player->y / TAILLE_CASE;
    int player_column = player->x / TAILLE_CASE;
    
    assert(player != NULL);
    assert(terrain != NULL);
    assert(lst_case_mana != NULL);

    if((terrain[player_row][player_column] == MANA ||
       terrain[player_row][player_column] == DOOR) ){

        if(lst_case_mana != NULL)
            count_size_list(*lst_case_mana);
        
        /* Add mana to player if he has not reached the max */
        if(player->mana < 100){
            terrain[player_row][player_column] = EMPTY;
            player->mana++;
            
            mana_case.row = player_row;
            mana_case.col = player_column;

            /* Store value in linked list */
            if(lst_case_mana != NULL)
                if(is_in_linked_list(*lst_case_mana, mana_case) == 0)
                    insert_in_head(lst_case_mana, mana_case);
        }
    }
}

void put_mana_terrain(int** terrain, List* lst_case_mana){
    /* Declaration and Initialization */
    int size_lst;
    int rand_nb;
    Case* rand_case;

    assert(terrain != NULL);
    assert(lst_case_mana != NULL);

    /* Get random case in linked list */
    if(lst_case_mana != NULL){
        size_lst = count_size_list(*lst_case_mana);
        rand_nb = rand() % size_lst;
        rand_case = get_case_from_lst(*lst_case_mana, rand_nb);
    }


    if(NULL == rand_case){
        return;
    }

    if(terrain[rand_case->row][rand_case->col] == EMPTY){
        terrain[rand_case->row][rand_case->col] = MANA;
    }

    if(lst_case_mana != NULL)
        delete_cell(lst_case_mana, *rand_case);
}

double euclidean_distance(int x1, int y1, int x2, int y2){
    return sqrt( pow(x2 - x1, 2) +  pow(y2 - y1, 2) );
}


/**
 * @brief to know if there are obstacles around a point
 * 
 * @param terrain 
 * @param pos_x 
 * @param pos_y 
 * @return int 
 */
static int has_wall_around(int** terrain, double pos_x, double pos_y){
    int x, y;
    int row, col;

    assert(terrain != NULL);

    for(x = -1; x < 2; x++){
        for(y = -1; y < 2; y++){
            row = (pos_y + TAILLE_CASE / 2 * y) / TAILLE_CASE;
            col = (pos_x + TAILLE_CASE / 2 * x) / TAILLE_CASE;

            if(x == 0 && y == 0)
                continue;

            if(row < 0 || col < 0 || terrain[row][col] == WALL || terrain[row][col] == DOOR)
                return 0;
        }
    }

    return 1;
}


/**
 * @brief to know if guardians are to close next to each other
 * 
 * @param guardians 
 * @param number_guardians 
 * @param pos_x 
 * @param pos_y 
 * @return int 
 */
static int has_guardians_around(Guardian* guardians, int number_guardians, double pos_x, double pos_y){
    int i;
    
    assert(guardians != NULL);
    assert(number_guardians >= 0);

    for(i = 0; i < number_guardians; i++){
        double distance = euclidean_distance(guardians[i].x, guardians[i].y, pos_x, pos_y);

        if(distance < DISTANCE_BETWEEN_GUARDIAN){
            return 0;
        }
    }
    return 1;
}

void place_guardians(int** terrain, int number_guardians, Guardian* guardians, double player_x, double player_y){
    double rand_x, rand_y;
    int distance_cases;
    int i;

    assert(terrain != NULL);
    assert(number_guardians >= 0);
    assert(guardians != NULL);

    for(i = 0; i < number_guardians; i++){
        do {
            rand_x = rand() % (TAILLE_CASE * NUMBER_CASE_COL);
            rand_y = rand() % (TAILLE_CASE * NUMBER_CASE_LIG);

            distance_cases = euclidean_distance(rand_x, rand_y, player_x, player_y) / TAILLE_CASE;

        } while (distance_cases < DISTANCE_CASE_FROM_PLAYER ||
                 has_wall_around(terrain, rand_x, rand_y) == 0 ||
                 has_guardians_around(guardians, number_guardians, rand_x, rand_y) == 0);
        
        guardians[i].x = rand_x;
        guardians[i].y = rand_y;
    }
}


int detection_pause(int is_paused){
    if(MLV_get_keyboard_state(MLV_KEYBOARD_ESCAPE) == MLV_PRESSED){
        MLV_wait_milliseconds(100);
        return PAUSE;
    }

    return RESUME;
}

int make_pause(){
    int x, y;
    int valeur;

    /* As long as the player does not click wait */
    do{
        MLV_wait_mouse(&x, &y);
        valeur = choose_pause(x, y);
    } while(valeur == PAUSE);

    return valeur;
}

int choose_menu(){
    int x, y;
    int pos_x_beg, pos_x_end;
    int pos_y_beg, pos_y_end;

    int height = NUMBER_CASE_LIG * TAILLE_CASE;
    int width = TAILLE_CASE * NUMBER_CASE_COL;

    MLV_wait_mouse(&x, &y);

    /* Play position */
    pos_x_beg = TAILLE_CASE;
    pos_x_end = pos_x_beg + width / 3;
    pos_y_beg = height / 1.5;
    pos_y_end = pos_y_beg + TAILLE_CASE * 3;
    
    /* Click on play */
    if(x > pos_x_beg && x < pos_x_end &&
       y > pos_y_beg && y < pos_y_end){
        return PLAY;
    }

    /* Click on parameter */
    if(x > pos_x_beg && x < pos_x_end &&
       y > pos_y_beg + TAILLE_CASE * 5 && y < pos_y_end + TAILLE_CASE * 5){
        return PARAMETER;
    }

    /* Click on quit */
    if(x > pos_x_beg && x < pos_x_end &&
       y > pos_y_beg + TAILLE_CASE * 10 && y < pos_y_end + TAILLE_CASE * 10){
        return QUIT;
    }

    return -1;
}

int choose_pause(int mouse_x, int mouse_y){
    int x = (TAILLE_CASE * NUMBER_CASE_COL) / 2;
    int y = (TAILLE_CASE * NUMBER_CASE_LIG) / 2;

    int size_width = TAILLE_CASE * 10;
    int size_height = TAILLE_CASE * 10;

    int button_x, button_y;

    /* Click on resume */
    button_x = x - size_width / 2 + TAILLE_CASE;
    button_y = y - size_height / 2 + TAILLE_CASE * 2;

    if(mouse_x > button_x && mouse_x < button_x + size_width - TAILLE_CASE * 2 &&
       mouse_y > button_y && mouse_y < button_y + TAILLE_CASE * 2){
        return RESUME;
    }

    /* Click on quit */
    button_y = y - size_height / 2 + TAILLE_CASE * 6;
    
    if(mouse_x > button_x && mouse_x < button_x + size_width - TAILLE_CASE * 2 &&
       mouse_y > button_y && mouse_y < button_y + TAILLE_CASE * 2){
        return QUIT;
    }

    MLV_actualise_window();
    return PAUSE;
}


int choose_end(){
    int mouse_x, mouse_y;
    int size_x, size_y;
    size_x = TAILLE_CASE * NUMBER_CASE_COL;
    size_y = TAILLE_CASE * NUMBER_CASE_LIG;

    MLV_wait_mouse(&mouse_x, &mouse_y);

    if(mouse_x > TAILLE_CASE && mouse_x < TAILLE_CASE + TAILLE_CASE * 10 &&
       mouse_y > size_y - TAILLE_CASE && mouse_y < size_y - TAILLE_CASE + TAILLE_CASE * 2){
        return RETRY;
    }

    if(mouse_x > size_x - TAILLE_CASE * 11 && mouse_x < size_x - TAILLE_CASE * 11 + TAILLE_CASE * 10 &&
       mouse_y > size_y - TAILLE_CASE && mouse_y < size_y - TAILLE_CASE + TAILLE_CASE * 2){
        return QUIT;
    }
    
    return -1;
}



char* enter_nickname(char* nickname, int i){
    MLV_Keyboard_button sym;
    MLV_Keyboard_modifier mod;

    int unicode;
    char* text;

    assert(nickname != NULL);

    do {
        MLV_wait_keyboard(&sym, &mod, &unicode);
        text = MLV_convert_unicode_to_string(unicode);
    } while( (text[0] < 97 || text[0] > 122) && (text[0] < 65 || text[0] > 90));
    /* Between 97 and 122 is between 'a' and 'z' */
    /* Between 65 and 90 is between 'A' and 'Z' */

    nickname[i] = text[0];
    free(text);

    return nickname;
}



void enter_panic_mode(Guardian* guardians, int size){
    int i;

    for(i = 0; i < size; i++){
        guardians[i].case_detection = PANIC_CASE_DETECTION;
        guardians[i].is_in_panic = 1;
    }
}

/**
 * @brief to return in the normal mode after the panic mode
 * 
 * @param guardians 
 * @param size 
 */
static void quit_panic_mode(Guardian* guardians, int size){
    int i;

    for(i = 0; i < size; i++){
        guardians[i].case_detection = INITIAL_CASE_DETECTION;
        guardians[i].is_in_panic = 0;
    }
}


Point* create_points(double pos_x, double pos_y, int number_points, double radius, int* nb_points_in_angle) {
    int i;
    double x, y;
    double angle, slice;

    Point* list_points = malloc(sizeof(Point) * (number_points + 1));

    /* Angle between each points */
    slice  = (PI * 2) / number_points;

    for (i = 0; i < number_points; i++) {
        angle = slice * i;

        x = radius * cos(angle) + pos_x;
        y = radius * sin(angle) + pos_y;

        /* Add points to a list */
        list_points[*nb_points_in_angle].x = x;
        list_points[*nb_points_in_angle].y = y;
        (*nb_points_in_angle)++;
    }

    return list_points;
}


int choose_parameter(int* nb_guardians, int* nb_relics, int* minside, int* panic_duration, int* sound, int* can_power){
    int mouse_x, mouse_y;
    int deb_x;
    int deb_y;
    int i;

    int shift = TAILLE_CASE * 5;
    int x = (TAILLE_CASE * NUMBER_CASE_COL) / 2;
    int y = (TAILLE_CASE * NUMBER_CASE_LIG) / 2 - shift * 2.5;
    
    int nb_parameter = 6;

    MLV_wait_mouse(&mouse_x, &mouse_y);

    if(mouse_x > 0 && mouse_x < TAILLE_CASE * 4 &&
       mouse_y > 0 && mouse_y < TAILLE_CASE * 4){
        return BACK;
    }

    for(i = 0; i < nb_parameter; i++){
        deb_x = x - shift - TAILLE_CASE * 2;
        deb_y = y + i * shift + TAILLE_CASE / 2;

        /* Sub button */
        if(mouse_x > deb_x && mouse_x < deb_x + TAILLE_CASE &&
           mouse_y > deb_y && mouse_y < deb_y + TAILLE_CASE){

            switch(i){
                case 0:
                    (*nb_guardians)--;
                    break;
                
                case 1:
                    (*nb_relics)--;
                    break;
                
                case 2:
                    (*minside)--;
                    break;
                
                case 3:
                    (*panic_duration)--;
                    break;
                
                default:
                    break;
            }
        }

        deb_x = x - shift + TAILLE_CASE * 2;

        /* Add button */
        if(mouse_x > deb_x && mouse_x < deb_x + TAILLE_CASE &&
           mouse_y > deb_y && mouse_y < deb_y + TAILLE_CASE){
            switch(i){
                case 0:
                    (*nb_guardians)++;
                    break;
                
                case 1:
                    (*nb_relics)++;
                    break;
                
                case 2:
                    (*minside)++;
                    break;
                
                case 3:
                    (*panic_duration)++;
                    break;
                
                default:
                    break;
            }
        }
        
        deb_x = x - shift;
        
        /* Cross button */
        if(mouse_x > deb_x && mouse_x < deb_x + TAILLE_CASE &&
           mouse_y > deb_y && mouse_y < deb_y + TAILLE_CASE){
            switch(i){
                case 4:
                    (*sound) = !(*sound);
                    break;
                
                case 5:
                    (*can_power) = !(*can_power);
                    break;
                
                default:
                    break;
            }
        }
        
    }
    MLV_actualise_window();

    return 0;
}


void clamp_settings(int* nb_guardians, int* nb_relics, int* minside, int* panic_duration){
    assert(nb_guardians != NULL);
    assert(nb_relics != NULL);
    assert(minside != NULL);
    assert(panic_duration != NULL);

    clamp(nb_guardians, 0, 15);
    clamp(nb_relics, 1, 20);
    clamp(minside, 4, 15);
    clamp(panic_duration, 3, 60);
}




double panic_mode_times_up(struct timeval debut, Guardian* guardians_list, int size, int seconde){
    struct timeval fin;
    double total_total = 0;
    
    assert(guardians_list != NULL);

    if(guardians_list[0].is_in_panic){
        gettimeofday(&fin, NULL);
        total_total = (double)(fin.tv_usec - debut.tv_usec) / 1000000 + (double)(fin.tv_sec - debut.tv_sec);
        
        if(total_total > seconde){
            /* Put the guards in normal mode */
            quit_panic_mode(guardians_list, size);
        }
    }

    return total_total;
}



int player_enters_base(Player player, int player_has_relics){
    if(player_has_relics && player.x <= POS_ORIGIN_PLAYER_X + TAILLE_CASE &&
       player.y <= POS_ORIGIN_PLAYER_Y + TAILLE_CASE){
        return 1;
    }
    return 0;
}

void clamp(int* value, int min, int max){
    if(*value < min)
        (*value) = min;
        
    if(*value > max)
        (*value) = max;
}

void end_user_choice(int choice, int sound){
    while(1){
        choice = choose_end();
        if(choice == RETRY){
            play_sound(RETRY, sound);
            break;
        }

        if(choice == QUIT){
            play_sound(QUIT, sound);
            exit(EXIT_SUCCESS);
        }
    }
}