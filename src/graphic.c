#include "../include/graphic.h"

/**
 * @brief We draw all the elements that make up the terrain,
 *        the tiles, the mana, the relics
 * 
 * @param terrain 
 */
static void draw_terrain(int** terrain){
    int i, j;
    int size_cirlce;
    MLV_Image* chest;

    assert(terrain != NULL);
    chest = MLV_load_image("img/chest.png");

    if(chest != NULL)
        MLV_resize_image(chest, TAILLE_CASE,
                                TAILLE_CASE);

    size_cirlce = TAILLE_CASE / 4;

    for(i = 0; i < NUMBER_CASE_LIG; i++){
        for(j = 0; j < NUMBER_CASE_COL; j++){
            MLV_draw_filled_rectangle(j * TAILLE_CASE, i * TAILLE_CASE,
                                      TAILLE_CASE, TAILLE_CASE, MLV_COLOR_BEIGE);
            switch(terrain[i][j]){
                case EMPTY:
                    break;   

                case MANA:
                    MLV_draw_filled_circle(j * TAILLE_CASE + TAILLE_CASE / 2, i * TAILLE_CASE + TAILLE_CASE / 2,
                                           size_cirlce, MLV_COLOR_LIGHT_SKY_BLUE);
                    break;

                case DOOR:
                    MLV_draw_filled_circle(j * TAILLE_CASE + TAILLE_CASE / 2, i * TAILLE_CASE + TAILLE_CASE / 2,
                                          size_cirlce, MLV_COLOR_LIGHT_SKY_BLUE);
                    break;

                case RELIC:
                    if(chest != NULL)
                        MLV_draw_image(chest, j * TAILLE_CASE, i * TAILLE_CASE);
                    else
                        MLV_draw_filled_rectangle(j * TAILLE_CASE, i * TAILLE_CASE,
                                      TAILLE_CASE, TAILLE_CASE, MLV_COLOR_RED);
                    break;

                case NO_RELIC:
                    MLV_draw_filled_rectangle(j * TAILLE_CASE, i * TAILLE_CASE,
                                TAILLE_CASE, TAILLE_CASE, MLV_COLOR_RED);
                    if(chest == NULL)
                        MLV_draw_filled_rectangle(j * TAILLE_CASE, i * TAILLE_CASE,
                                      TAILLE_CASE, TAILLE_CASE, MLV_COLOR_BEIGE);
                    break;

                case DETECTED_NO_RELIC:
                    MLV_draw_filled_rectangle(j * TAILLE_CASE, i * TAILLE_CASE,
                                TAILLE_CASE, TAILLE_CASE, MLV_COLOR_RED);
                    break;
            }

            MLV_draw_circle(j * TAILLE_CASE + TAILLE_CASE / 2, i * TAILLE_CASE + TAILLE_CASE / 2,
                                size_cirlce, MLV_COLOR_BLACK);

        }
    }

    MLV_free_image(chest);
}

/**
 * @brief We draw all the wall of the terrain
 * 
 * @param terrain 
 */
void display_wall(int** terrain){

    int i, j;
    MLV_Image* wall;

    wall = MLV_load_image("img/tile.png");
    assert(terrain != NULL);

    if(wall != NULL)
        MLV_resize_image(wall, TAILLE_CASE,
                               TAILLE_CASE);

    for(i = 0; i < NUMBER_CASE_LIG; i++){
        for(j = 0; j < NUMBER_CASE_COL; j++){
            if(terrain[i][j] == WALL){
                /* If there is Wall draw wall image */
                if(wall != NULL)
                    MLV_draw_image(wall, j * TAILLE_CASE, i * TAILLE_CASE);
                else
                    MLV_draw_filled_rectangle(j * TAILLE_CASE, i * TAILLE_CASE,
                                              TAILLE_CASE, TAILLE_CASE, MLV_COLOR_BLACK);
            }
        }
    }

    MLV_free_image(wall);
}

/**
 * @brief Draw the entry or exit area of the room
 * 
 * @param has_relics Parameter to know if the player has collected all the relics
 */
static void entrance_exit_area(int has_relics){
    if(has_relics)
        MLV_draw_filled_rectangle(TAILLE_CASE, TAILLE_CASE,
                                  POS_ORIGIN_PLAYER_X, POS_ORIGIN_PLAYER_Y, MLV_COLOR_GREEN);
    else 
        MLV_draw_filled_rectangle(TAILLE_CASE, TAILLE_CASE, POS_ORIGIN_PLAYER_X,
                                  POS_ORIGIN_PLAYER_Y, MLV_COLOR_RED);
}


/**
 * @brief Draw the player with a different skin because
 *        he has his acceleration power
 * 
 * @param player 
 */
static void draw_acceleration_power(const Player* player){
    assert(player != NULL);

    /* Draw border of circle */
    MLV_draw_filled_circle(player->x, player->y,
                           TAILLE_CASE / 2, MLV_COLOR_BLACK);

    MLV_draw_filled_circle(player->x, player->y,
                            TAILLE_CASE / 2 - 2, MLV_COLOR_YELLOW);

}

/**
 * @brief Draw the player with a different skin because
 *        he has his invisibility power
 * 
 * @param player 
 */
static void draw_invisibility_power(const Player* player){
    MLV_Color color = MLV_rgba(0, 0, 255, 90);
    
    assert(player != NULL);
    
    MLV_draw_filled_circle(player->x, player->y,
                            TAILLE_CASE / 2, color);
}


/**
 * @brief Draw the player surrounded by a black border
 * 
 * @param player 
 */
static void draw_player(const Player* player){
    assert(player != NULL);

    /* Changes color depending on whether he uses a power */
    if(player->power.is_accelerated == 1){
        draw_acceleration_power(player);
        return;
    }

    if(player->power.is_invisible){
        draw_invisibility_power(player);
        return;
    }

    /* Draw border of circle */
    MLV_draw_filled_circle(player->x, player->y,
                           TAILLE_CASE / 2, MLV_COLOR_BLACK);
    MLV_draw_filled_circle(player->x, player->y,
                            TAILLE_CASE / 2 - 2, MLV_COLOR_BLUE);
}


/**
 * @brief Draw all the guardians and their detection zone
 * 
 * @param list_guardians 
 * @param number_guardians 
 */
static void draw_all_guardians(const Guardian* list_guardians, int number_guardians){

    int i;
    MLV_Color color;

    assert(list_guardians != NULL);
    assert(number_guardians >= 0);

    color = MLV_COLOR_DARKBLUE;

    for(i = 0; i < number_guardians; i++){
        MLV_draw_filled_circle(list_guardians[i].x, list_guardians[i].y,
                               TAILLE_CASE / 2, MLV_COLOR_BLACK);

        /* If the guardian is in panic mode then we
        change the color of the guardian to red */
        if(list_guardians[i].is_in_panic == 1)
            color = MLV_COLOR_RED;

        MLV_draw_filled_circle(list_guardians[i].x, list_guardians[i].y,
                               TAILLE_CASE / 2 - 2, color);
        
      
    }
}


/**
 * @brief Draws the player's mana bar at the bottom of the screen
 * 
 * @param player 
 */
static void draw_mana_bar(const Player player){
    int hauteur = (NUMBER_CASE_LIG + 3.5) * TAILLE_CASE;
    int new_width;

    /* Write some texts */
    MLV_draw_text(TAILLE_CASE + 10, hauteur - 7 - TAILLE_CASE * 3,
                  "Barre de mana : %d / 100", MLV_COLOR_RED, player.mana);

    new_width = (player.mana * TAILLE_CASE * 15) / 100;
    MLV_draw_filled_rectangle(TAILLE_CASE + 10, hauteur - 10 - TAILLE_CASE * 2,
                              new_width, TAILLE_CASE, MLV_COLOR_BLUE2); 

    /* Draw the total rectangle rectangle */
    MLV_draw_rectangle(TAILLE_CASE + 10, hauteur - 10 - TAILLE_CASE * 2,
                       TAILLE_CASE * 15, TAILLE_CASE, MLV_COLOR_BLACK);

 
}

/**
 * @brief Draws information about relics at the bottom of the screen
 * 
 * @param counter_relics 
 * @param max_relics 
 */
static void draw_relics_info(int counter_relics, int max_relics){
    int hauteur = (NUMBER_CASE_LIG + 3.5) * TAILLE_CASE;
    
    /* Draw informations */
    MLV_draw_text(TAILLE_CASE * 18, hauteur - TAILLE_CASE * 2.5, "Reliques %d / %d", MLV_COLOR_BLACK, counter_relics, max_relics);
}

/**
 * @brief Draws information about mana at the bottom of the screen
 * 
 * @param total_mana_used 
 */
static void draw_mana_totale(int total_mana_used){
    int hauteur = (NUMBER_CASE_LIG + 3.5) * TAILLE_CASE;
    
    /* Draw informations */
    MLV_draw_text(TAILLE_CASE * 25, hauteur - TAILLE_CASE * 2.5, "Total Mana : %d", MLV_COLOR_BLACK, total_mana_used);
}

/**
 * @brief Draws information about times at the bottom of the screen
 * 
 * @param time 
 */
static void draw_time_game(double time){
        int hauteur = (NUMBER_CASE_LIG + 3.5) * TAILLE_CASE;
    
    /* Draw informations */
    MLV_draw_text(TAILLE_CASE * 34, hauteur - TAILLE_CASE * 2.5, "Temps : %.2f s", MLV_COLOR_BLACK, time);
}

void draw_menu(){
    MLV_Image *menu;
    int height = NUMBER_CASE_LIG * TAILLE_CASE;
    int width = TAILLE_CASE * NUMBER_CASE_COL;
    

    MLV_clear_window(MLV_COLOR_GREY);

    menu = MLV_load_image("img/menu.png");



    if(menu != NULL){
        MLV_resize_image(menu, width,
                               height + TAILLE_CASE * 2);
        MLV_draw_image(menu, 0, 0);
    }

    /* Play button */
    MLV_draw_filled_rectangle(TAILLE_CASE, height / 1.5,
                              width / 3, TAILLE_CASE * 3, MLV_COLOR_WHITE);
    MLV_draw_text(TAILLE_CASE + width / 7, height / 1.5 + TAILLE_CASE,
                  "Jouer", MLV_COLOR_BLACK);


    /* Settings button */
    MLV_draw_filled_rectangle(TAILLE_CASE, height / 1.5 + TAILLE_CASE * 5,
                             width / 3, TAILLE_CASE * 3, MLV_COLOR_WHITE);
    MLV_draw_text(TAILLE_CASE + width / 7, height / 1.5 + TAILLE_CASE * 6,
                 "Paramètres", MLV_COLOR_BLACK);

    /* Quit button */
    MLV_draw_filled_rectangle(TAILLE_CASE, height / 1.5 + TAILLE_CASE * 10,
                              width / 3, TAILLE_CASE * 3, MLV_COLOR_WHITE);
    MLV_draw_text(TAILLE_CASE + width / 7, height / 1.5 + TAILLE_CASE * 11,
                  "Quitter", MLV_COLOR_BLACK);

    MLV_free_image(menu);
}

void draw_parameters(int nb_guardians, int nb_relics, int minside, int panic_duration, int sound, int can_power){
   
    MLV_Image *parameter;
   
    int shift = TAILLE_CASE * 5;
    int x = (TAILLE_CASE * NUMBER_CASE_COL) / 2;
    int y = (TAILLE_CASE * NUMBER_CASE_LIG) / 2 - shift * 2.5;
    
    int nb_parameter = 6;
    int i;

    MLV_Image *back;
    MLV_Image *sub;
    MLV_Image *add;
    MLV_Image *yes_no;
    
    MLV_clear_window(MLV_COLOR_GREY);

    parameter = MLV_load_image("img/parametre_menu.png");

    if(parameter != NULL){
        MLV_resize_image(parameter, TAILLE_CASE * NUMBER_CASE_COL, TAILLE_CASE * NUMBER_CASE_LIG + TAILLE_CASE * 2);
        MLV_draw_image(parameter, 0, 0);
    }

    back = MLV_load_image("img/back.png");
    sub = MLV_load_image("img/sub.png");
    add = MLV_load_image("img/add.png");
    yes_no = MLV_load_image("img/yes_no.png");

    /* Back button image */
    if(back != NULL){
        MLV_resize_image(back, TAILLE_CASE * 4, TAILLE_CASE * 4);
        MLV_draw_image(back, 0, 0);
    } else {
        MLV_draw_filled_rectangle(0, 0,
                                  TAILLE_CASE * 4, TAILLE_CASE * 4, MLV_COLOR_BLACK);
        MLV_draw_text(TAILLE_CASE / 2, TAILLE_CASE * 1.5,
                      "Retour", MLV_COLOR_WHITE);
    }


    /* Sub button */
    if(sub != NULL)
        MLV_resize_image(sub, TAILLE_CASE, TAILLE_CASE);

    /* Add button */
    if(add != NULL)
        MLV_resize_image(add, TAILLE_CASE, TAILLE_CASE);
    
    /* Cross button*/
    if(yes_no != NULL)
        MLV_resize_image(yes_no, TAILLE_CASE, TAILLE_CASE);

    for(i = 0; i < nb_parameter; i++){
        switch(i){
            case 0:
                MLV_draw_text(x, y + i * shift, "Nombre de gardiens", MLV_COLOR_WHITE);
                MLV_draw_text(x - shift + TAILLE_CASE / 2, y + i * shift + TAILLE_CASE / 2,
                              "%d", MLV_COLOR_WHITE, nb_guardians);
                break;
            case 1:
                MLV_draw_text(x, y + i * shift, "Nombre de reliques", MLV_COLOR_WHITE);
                MLV_draw_text(x - shift + TAILLE_CASE / 2, y + i * shift + TAILLE_CASE / 2,
                              "%d", MLV_COLOR_WHITE, nb_relics);
                break;
            case 2:
                MLV_draw_text(x, y + i * shift, "Taille mur minimal", MLV_COLOR_WHITE);
                MLV_draw_text(x - shift + TAILLE_CASE / 2, y + i * shift + TAILLE_CASE / 2,
                              "%d", MLV_COLOR_WHITE, minside);
                break;
            case 3:
                MLV_draw_text(x, y + i * shift, "Durée du mode panique", MLV_COLOR_WHITE);
                MLV_draw_text(x - shift + TAILLE_CASE / 2, y + i * shift + TAILLE_CASE / 2,
                              "%d", MLV_COLOR_WHITE, panic_duration);
                break;
            case 4:
                MLV_draw_text(x, y + i * shift, "Son", MLV_COLOR_WHITE);
                MLV_draw_rectangle(x - shift, y + i * shift + TAILLE_CASE / 2,
                                   TAILLE_CASE, TAILLE_CASE, MLV_COLOR_WHITE);
                if(sound && yes_no != NULL)
                    MLV_draw_image(yes_no, x - shift, y + i * shift + TAILLE_CASE / 2);
                break;
            case 5:
                MLV_draw_text(x, y + i * shift, "Pouvoirs du joueur", MLV_COLOR_WHITE);
                MLV_draw_rectangle(x - shift, y + i * shift + TAILLE_CASE / 2,
                                   TAILLE_CASE, TAILLE_CASE, MLV_COLOR_WHITE);
                if(can_power && yes_no != NULL)
                    MLV_draw_image(yes_no, x - shift, y + i * shift + TAILLE_CASE / 2);
                break;
            default:
                break;
        }

        if(i < 4){
            /* Sub */
            MLV_draw_rectangle(x - shift - TAILLE_CASE * 2, y + i * shift + TAILLE_CASE / 2,
                               TAILLE_CASE, TAILLE_CASE, MLV_COLOR_WHITE);
            if(sub != NULL)
                MLV_draw_image(sub, x - shift - TAILLE_CASE * 2, y + i * shift + TAILLE_CASE / 2);
            
            /* Add */
            MLV_draw_rectangle(x - shift + TAILLE_CASE * 2, y + i * shift + TAILLE_CASE / 2,
                               TAILLE_CASE, TAILLE_CASE, MLV_COLOR_WHITE);
            if(add != NULL)
                MLV_draw_image(add, x - shift + TAILLE_CASE * 2, y + i * shift + TAILLE_CASE / 2);
        }
    }

    MLV_free_image(back);
    MLV_free_image(sub);
    MLV_free_image(add);
    MLV_free_image(yes_no);
    MLV_free_image(parameter);
}

void draw_game(const Player player, Guardian* guardians_list, int number_guardians, int** terrain, int has_relics){
    draw_terrain(terrain);
    entrance_exit_area(has_relics);
    draw_player(&player);
    draw_all_guardians(guardians_list, number_guardians);
}

void draw_informations(const Player player, int counter_relics, int max_relics, int total_mana_used, double time){
    draw_mana_bar(player);
    draw_relics_info(counter_relics, max_relics);
    draw_mana_totale(total_mana_used);
    draw_time_game(time);
}

void draw_pause(){
    int button_x, button_y;
    int x = (TAILLE_CASE * NUMBER_CASE_COL) / 2;
    int y = (TAILLE_CASE * NUMBER_CASE_LIG) / 2;

    int size_width = TAILLE_CASE * 10;
    int size_height = TAILLE_CASE * 10;

    MLV_Color color = MLV_rgba(230, 230, 230, 240);

    MLV_draw_filled_rectangle(x - size_width / 2, y - size_height / 2,
                             size_width, size_height, color);

    button_x = x - size_width / 2 + TAILLE_CASE;
    button_y = y - size_height / 2 + TAILLE_CASE * 2;

    MLV_draw_filled_rectangle(button_x, button_y,
                             size_width - TAILLE_CASE * 2, TAILLE_CASE * 2, MLV_COLOR_DARKGRAY);
    MLV_draw_text(button_x + TAILLE_CASE * 2.5, button_y + TAILLE_CASE / 2, "RESUME", MLV_COLOR_BLACK);
    

    button_x = x - size_width / 2 + TAILLE_CASE;
    button_y = y - size_height / 2 + TAILLE_CASE * 6;

    MLV_draw_filled_rectangle(button_x, button_y,
                             size_width - TAILLE_CASE * 2, TAILLE_CASE * 2, MLV_COLOR_DARKGRAY);
    
    MLV_draw_text(button_x + TAILLE_CASE * 3, button_y + TAILLE_CASE / 2, "QUIT", MLV_COLOR_BLACK);
}

void draw_line_under_letter(int size){
    int i;
    int x = (TAILLE_CASE * NUMBER_CASE_COL) / 2;
    int y = (TAILLE_CASE * NUMBER_CASE_LIG) / 2;
    int space = TAILLE_CASE / 0.8;

    for(i = 0; i < size; i++){
        MLV_draw_filled_rectangle(x - TAILLE_CASE * 2 + space * i, y + 20, 7, 2, MLV_COLOR_WHITE);
    }
}

/**
 * @brief Draw two continue and exit buttons at the bottom of the screen in the end menu
 * 
 */
static void draw_button_end(){
    int size_x, size_y;
    size_x = TAILLE_CASE * NUMBER_CASE_COL;
    size_y = TAILLE_CASE * NUMBER_CASE_LIG;

    /* Button Menu */
    MLV_draw_filled_rectangle(TAILLE_CASE, size_y - TAILLE_CASE,
                             TAILLE_CASE * 10, TAILLE_CASE * 2, MLV_COLOR_WHITE);
    MLV_draw_text(TAILLE_CASE * 5 ,  size_y - TAILLE_CASE / 2, "Menu", MLV_COLOR_BLACK);


    /* Button Quit */
    MLV_draw_filled_rectangle(size_x - TAILLE_CASE * 11, size_y - TAILLE_CASE,
                             TAILLE_CASE * 10, TAILLE_CASE * 2, MLV_COLOR_WHITE);
    MLV_draw_text(size_x - TAILLE_CASE * 7,  size_y - TAILLE_CASE / 2, "Quitter", MLV_COLOR_BLACK);

}

void draw_end(int mana, double time, char* nickname, int size, int resultat){
    int x = (TAILLE_CASE * NUMBER_CASE_COL) / 2;
    int y = (TAILLE_CASE * NUMBER_CASE_LIG) / 2;
    int space = TAILLE_CASE / 0.8;

    if(resultat == 0){
        MLV_draw_text(x - TAILLE_CASE * 4, y - TAILLE_CASE * 13, "VOUS AVEZ GAGNÉ !", MLV_COLOR_RED);
        MLV_draw_text(x - TAILLE_CASE * 5, y - TAILLE_CASE, "ENTREZ VOTRE PSEUDO :", MLV_COLOR_WHITE);
        MLV_draw_text(x - TAILLE_CASE * 2 + space * size, y, "%c", MLV_COLOR_WHITE, nickname[size]);
    } else
        MLV_draw_text(x - TAILLE_CASE * 3, y - TAILLE_CASE * 13, "VOUS AVEZ PERDU !", MLV_COLOR_RED);
    
    MLV_draw_text(x - TAILLE_CASE * 9, y - TAILLE_CASE * 3, "Mana utilisés : %d  -  Temps total: %.2f secondes", MLV_COLOR_WHITE, mana, time);

    draw_button_end();
}


void display_file_score(Score* lst_scores, int size){
    int i;
    int x = (TAILLE_CASE * NUMBER_CASE_COL) / 2;
    int y = (TAILLE_CASE * NUMBER_CASE_LIG) / 2 + (TAILLE_CASE * 5);
    int separator = (TAILLE_CASE * 2);

    assert(lst_scores != NULL);
    assert(size >= 0);

    /* Sort by mana */
    qsort(lst_scores, size, sizeof(Score), comparator_score_mana);
    MLV_draw_text(x - (separator * 10), y - TAILLE_CASE, "Trié par mana :", MLV_COLOR_RED);
    for(i = 0; i < size; i++){
        MLV_draw_text(x - (separator * 13), y + (i * 15), "Mana : %d", MLV_COLOR_WHITE, lst_scores[i].mana);
        MLV_draw_text(x - (separator * 10), y + (i * 15), "Temps : %.2f s", MLV_COLOR_WHITE, lst_scores[i].time);
        MLV_draw_text(x - (separator * 6), y + (i * 15), "Pseudo : %s", MLV_COLOR_WHITE, lst_scores[i].name);
        
        if(i >= MAX_SCORE - 1) break;
    }

    /* Sort by time */
    qsort(lst_scores, size, sizeof(Score), comparator_score_time);
    MLV_draw_text(x + (separator * 6), y - TAILLE_CASE, "Trié par temps :", MLV_COLOR_RED, lst_scores[i].time);
    for(i = 0; i < size; i++){
        MLV_draw_text(x + (separator * 3), y + (i * 15), "Mana : %d", MLV_COLOR_WHITE, lst_scores[i].mana);
        MLV_draw_text(x + (separator * 6), y + (i * 15), "Temps : %.2f s", MLV_COLOR_WHITE, lst_scores[i].time);
        MLV_draw_text(x + (separator * 10), y + (i * 15), "Pseudo : %s", MLV_COLOR_WHITE, lst_scores[i].name);
        
        if(i >= MAX_SCORE - 1) break;
    }
}

void draw_informations_panic_mode_time(double time_panic, double panic_duration){
    int x = TAILLE_CASE * NUMBER_CASE_COL;
    int hauteur = (NUMBER_CASE_LIG + 3.5) * TAILLE_CASE;

    MLV_draw_text(x - TAILLE_CASE * 17, hauteur - TAILLE_CASE * 2.5, "Mode panique temps : %.2f s / %.2f s", MLV_COLOR_BLACK, time_panic, panic_duration);
}

void draw_view_area_detection_guardian(Guardian guardian, Point* list_points, int number_points) {
    MLV_Color color = MLV_rgba(255, 0, 0, 100); 

    int* list_x;
    int* list_y;
    
    int i;
    
    list_x = malloc(sizeof(int) * (number_points) );
    list_y = malloc(sizeof(int) * (number_points) );

    for(i = 0; i < number_points; i++) {
        list_x[i] = list_points[i].x;
        list_y[i] = list_points[i].y;
    }

    MLV_draw_filled_polygon(list_x, list_y, number_points, color);
    MLV_draw_polygon(list_x, list_y, number_points, MLV_COLOR_BLACK);

    free(list_x);
    free(list_y);
}