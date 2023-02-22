#ifndef __GRAPHIC__
#define __GRAPHIC__

#include "player.h"
#include "guardian.h"
#include "terrain.h"
#include "file_manager.h"
#include <MLV/MLV_all.h>


/**
 * @brief Draws play, settings, exit buttons and displays a background image
 * 
 */
void draw_menu();

/**
 * @brief Draw subtraction, addittion, back button and all editable parameters
 * 
 */
void draw_parameters(int sound, int nb_guardians, int nb_relics, int minside, int panic_duration, int can_power);

/**
 * @brief Function that brings together other functions drawing the player,
 *        the guardians, the entrance to the terrain, the wall and floor
 * 
 * @param player 
 * @param guardians_list 
 * @param number_guardians 
 * @param terrain 
 * @param has_relics 
 */
void draw_game(const Player player, Guardian* guardians_list, int number_guardians, int** terrain, int has_relics);



void display_wall(int** terrain);

/**
 * @brief Writes information at the bottom of the screen such as mana bar,
 *        time, total mana used and number of relics
 * 
 * @param player 
 * @param counter_relics 
 * @param max_relics 
 * @param total_mana_used 
 * @param time 
 */
void draw_informations(const Player player, int counter_relics, int max_relics, int total_mana_used, double time);

/**
 * @brief Draw pause mode on screen with continue button and exit button
 * 
 */
void draw_pause();

/**
 * @brief Draw on the screen the end menu, with the message of victory or defeat,
 *        the total time and the total mana. As well as two obutons continue and exit.
 *        If the player won displays his nickname.
 * 
 * @param mana 
 * @param time 
 * @param nickname 
 * @param size 
 * @param resultat 
 */
void draw_end(int mana, double time, char* nickname, int size, int resultat);

/**
 * @brief Displays the rectangles below the letters when the player writes his nickname
 * 
 * @param size 
 */
void draw_line_under_letter(int size);

/**
 * @brief Displays the scoreboard on the screen. A table on the left for the score sorted
 *        by mana and a score table sorted by time. The scoreboard displays the pseudo mana
 *        used and the total time
 * 
 * @param lst_scores 
 * @param size 
 */
void display_file_score(Score* lst_scores, int size);

/**
 * @brief Displays text at the bottom of the screen, the time of the panic mod
 * 
 * @param time_panic 
 * @param panic_duration 
 */
void draw_informations_panic_mode_time(double time_panic, double panic_duration);

/**
 * @brief Draw the guard detection area with a polygon
 * 
 * @param guardian 
 * @param list_points 
 * @param number_points 
 */
void draw_view_area_detection_guardian(Guardian guardian, Point* list_points, int number_points);

#endif