#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h> 
#include <math.h>

#include "../include/graphic.h"
#include "../include/game_manager.h"
#include "../include/collision.h"
#include "../include/player.h"
#include "../include/guardian.h"
#include "../include/file_manager.h"
#include "../include/mana.h"
#include "../include/relic.h"

int main(){

    /* Initialization and declaration of variables */
    Player player;

    Guardian* guardians_list;

    int** terrain;

    int number_guardians, number_relics;
    int quit, pause, sound;
    int counter_relics, total_mana_used, detecte_player;

    int panic_duration;
    int is_in_panic;
    int can_power;
    int minside;

    /* Music */
    MLV_Music* music;
    MLV_Music* music_panic_mode;

    /* Time */
    struct timespec end_time, new_time;
    double frametime, extratime;

    struct timeval begin, actual, end, begin_panic;
    double total_times;

    /* Mana */
    List linked_list = NULL;

    /* Player */
    int point_player;
    int size_points_player;
    Score* lst_scores;
    Point* list_points;
    int resultat_parameter;
    double time_panic;
    int i;

    /* File manager */
    char nickname[SIZE_NAME + 1];
    int size_file;
    const char* file_name = "score_game";

    srand(time(NULL));

    MLV_create_window("Jeu infiltration", "Projet",
                      TAILLE_CASE * NUMBER_CASE_COL,
                      TAILLE_CASE * NUMBER_CASE_LIG + TAILLE_CASE * 2);
    
    /* Initializes the sound infrastructure of the MLV library. */
    if(MLV_init_audio()){
        fprintf(stderr, "MLV library audio framework did not initialize properly\n");
        exit(1);
    }

    music = MLV_load_music("sounds/music.mp3");
    music_panic_mode = MLV_load_music("sounds/panic.mp3");

    /* Settings variable */
    number_guardians = 5;
    number_relics = 3;
    minside = 9;
    panic_duration = 30;
    sound = 1;
    can_power = 1;
    is_in_panic = 0;
    size_file = -1; /* No file */

    /* File */
    size_file = -1; /* No file */
    file_name = "score_game";

    play_music(music, sound);

    while(1){

        /* Menu */
        int get_choice;
        MLV_actualise_window();
        while(1) {
            draw_menu();
            MLV_actualise_window();
            get_choice = choose_menu();
            
            if(get_choice == PLAY){
                play_sound(PLAY, sound);
                break;
            }

            if(get_choice == PARAMETER){
                play_sound(PARAMETER, sound);
                
                do {
                    draw_parameters(number_guardians, number_relics,
                                    minside, panic_duration, sound, can_power);
                    MLV_actualise_window();
                    resultat_parameter = choose_parameter(&number_guardians, &number_relics,
                                           &minside, &panic_duration,
                                           &sound, &can_power);
                    clamp_settings(&number_guardians, &number_relics,
                                   &minside, &panic_duration);
                } while(resultat_parameter != BACK);
                continue;
            }

            if(get_choice == QUIT){
                play_sound(QUIT, sound);
                exit(EXIT_SUCCESS);
            }
        }

        play_music(music, sound);

        /* Terrain */
        terrain = initialize_terrain();
        generation_wall(terrain, 1, 1, NUMBER_CASE_COL - 1, NUMBER_CASE_LIG - 1, minside);
        
        /* Player */
        initialize_player(&player);

        /* Guardian */
        guardians_list = malloc(sizeof(Guardian) * number_guardians);
        place_guardians(terrain, number_guardians, guardians_list, POS_ORIGIN_PLAYER_X, POS_ORIGIN_PLAYER_Y);
        initialize_guardians(guardians_list, number_guardians);

        /* Relics */
        place_relics(terrain, number_relics, POS_ORIGIN_PLAYER_X, POS_ORIGIN_PLAYER_Y);


        MLV_actualise_window();
        counter_relics = 0;
        total_mana_used = 0;
        detecte_player = 0;
        quit = 0;
        pause = RESUME; 

        /* Time */
        gettimeofday(&begin, NULL);


        while(!quit){
            /* Some declaration of variables */
            double actual_time;
            int has_relics;

            gettimeofday(&actual, NULL);
            actual_time = (double)(actual.tv_usec - begin.tv_usec) / 1000000 + (double)(actual.tv_sec - begin.tv_sec);
            
            /* Get the time in nanoscond at the start of the frame */
            clock_gettime(CLOCK_REALTIME, &end_time);

            MLV_clear_window(MLV_COLOR_WHITE);

            /* Display of the current frame, sample function */
            draw_game(player, guardians_list, number_guardians, terrain, has_relics);
            draw_informations(player, counter_relics, number_relics, total_mana_used, actual_time);

            size_points_player = 0;
            list_points = create_points(player.x, player.y,
                                               POINT_BORDER_PLAYER, TAILLE_CASE / 2,
                                               &size_points_player);

            for(i = 0; i < number_guardians; i++){
                int nb_points_angle = 0;
                Point* list_points_guardian = create_points(guardians_list[i].x, guardians_list[i].y,
                                                            NUMBER_POINT_AROUND,
                                                            guardians_list[i].case_detection * TAILLE_CASE,
                                                            &nb_points_angle);

                dragon_view(list_points_guardian, guardians_list[i],
                            terrain, nb_points_angle);
                draw_view_area_detection_guardian(guardians_list[i],
                                                  list_points_guardian, nb_points_angle);

                if(player.power.is_invisible == 0){
                    for(point_player = 0; point_player < POINT_BORDER_PLAYER; point_player++){

                        /* Player has been detected */
                        if(zone_detection_guardian(list_points[point_player].x, list_points[point_player].y,
                                                  guardians_list[i], terrain) == 1){
                            detecte_player = 1;
                        }
                    }
                }


               if(panic_mode(guardians_list[i], terrain)){
                    gettimeofday(&begin_panic, NULL);
                    enter_panic_mode(guardians_list, number_guardians);
                    is_in_panic = 1;
                    play_music(music_panic_mode, sound);
                }
                free(list_points_guardian);
            }
            free(list_points);


            /* If the panic mode ends then we cut the music and we play the other music */
            time_panic = panic_mode_times_up(begin_panic, guardians_list, number_guardians, panic_duration);
            
            if(time_panic >= panic_duration){
                is_in_panic = 0;
                play_music(music, sound);
            }
            draw_informations_panic_mode_time(time_panic, panic_duration );
            
            display_wall(terrain);
            MLV_actualise_window();

            /* Pause */
            pause = detection_pause(pause);
            if(pause == PAUSE){
                draw_pause();
                MLV_actualise_window();
                play_sound(PAUSE, sound);
            }
            while(pause == PAUSE){
                pause = make_pause();
                if(pause == RESUME)
                    play_sound(RESUME, sound);
                if(pause == QUIT){
                    play_sound(QUIT, sound);
                    detecte_player = 1;
                }
            }
            /* End of pause module */


            /* Dealing with the events */

            /* Get object on floor */
            player_get_mana(&player, terrain, &linked_list);
            player_get_relic(&player, terrain, &counter_relics);
            
            if(can_power)
                choose_skill(&player, &total_mana_used);
            if(player.power.is_accelerated || player.power.is_invisible)
                put_mana_terrain(terrain, &linked_list);

            /* Move the entities on the grid */
            movement_player(&player, terrain);

            /* Collision detection and other game mechanisms */
            clamp_player_position(&player, terrain);
            update_position_guardian(guardians_list, number_guardians, terrain);
    
            has_relics = player_collect_all_relics(counter_relics, number_relics);
            quit = player_enters_base(player, has_relics);
            if(detecte_player == 1)
                quit = 1;

            /* Get the time in nanosecond at the end of the frame */
            clock_gettime (CLOCK_REALTIME, &new_time);

            /* Compute the time spent for the current frame */
            frametime = new_time.tv_sec - end_time.tv_sec;
            frametime += (new_time.tv_sec - end_time.tv_sec) / 1.0E9;
            
            /* Force the program to spend at least 1/60 second in total */
            extratime = 1.0 / 60 - frametime;
            if(extratime > 0){
                MLV_wait_milliseconds ((int)(extratime * 1000));
            }
        }

        /* Disable panic mode music if guardian is in panic */
        if(is_in_panic){
            play_music(music, sound);
        }

        /* If the player is dead */
        if(detecte_player == 1)
            play_sound(DEAD, sound);

        gettimeofday(&end, NULL);
        total_times = (double)(end.tv_usec - begin.tv_usec) / 1000000 + (double)(end.tv_sec - begin.tv_sec);


        MLV_clear_window(MLV_COLOR_BLACK);
        draw_end(total_mana_used, total_times, nickname, i, detecte_player);
        MLV_actualise_window();
        
        /* If the player win */
        if(detecte_player == 0){
            play_sound(WIN, sound);

            /* Enter nickname */
            draw_line_under_letter(SIZE_NAME);
            for(i = 0; i < SIZE_NAME; i++){
                MLV_actualise_window();
                enter_nickname(nickname, i);
                draw_end(total_mana_used, total_times, nickname, i, detecte_player);
            }

            /* We write the scores in the file */
            write_score_to_file(total_mana_used, total_times, nickname, file_name);

            size_file = get_size_number_score(file_name);
            lst_scores = get_score_from_file(file_name, size_file);
            display_file_score(lst_scores, size_file);
            free(lst_scores);
        } else {
            play_sound(LOSE, sound);
        }

        size_file = get_size_number_score(file_name);
        if(size_file > 0){
            lst_scores = get_score_from_file(file_name, size_file);
            display_file_score(lst_scores, size_file);
            free(lst_scores);
        }
            
        MLV_actualise_window();

        /* User choice to quit or restart */
        end_user_choice(get_choice, sound);
    
        free_terrain(terrain);
    }




    MLV_stop_all_sounds();
    free_terrain(terrain);
    free(lst_scores);

    free(guardians_list);
    MLV_free_music(music);
    MLV_free_music(music_panic_mode);
    MLV_free_audio();
    MLV_free_window();

    return 0;
}