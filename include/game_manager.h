#ifndef __GAME_MANAGER__
#define __GAME_MANAGER__

#include "terrain.h"
#include "player.h"
#include "guardian.h"
#include "sound.h"
#include "mana.h"
#include <sys/time.h> 

typedef enum {
  PLAY,
  PARAMETER,
  QUIT,
  RESUME,
  PAUSE,
  RETRY,
  BACK,
  DEAD,
  WIN,
  LOSE
} Status;

#define PI 3.14159265359
#define DEG_TO_RAD (PI / 180)

#define DISTANCE_BETWEEN_GUARDIAN (TAILLE_CASE * 10)
#define NUMBER_GUARDIANS 5
#define NUMBER_RELICS 3

#define POINT_BORDER_PLAYER 100


/**
 * @brief add mana in the mana linked list of the player
 * 
 * @param player 
 * @param terrain 
 * @param lst_case_mana 
 */
void player_get_mana(Player* player, int** terrain, List* lst_case_mana);

/**
 * @brief to calculate the euclidean distance between two points
 * 
 * @param x1 
 * @param y1 
 * @param x2 
 * @param y2 
 * @return double 
 */
double euclidean_distance(int x1, int y1, int x2, int y2);

/**
 * @brief to replace mana on the steps of the player
 * 
 * @param terrain 
 * @param lst_case_mana 
 */
void put_mana_terrain(int** terrain, List* lst_case_mana);


/**
 * @brief to place guardians randomly but far enough of the main door
 * 
 * @param terrain 
 * @param number_guardians 
 * @param guardians 
 * @param player_x 
 * @param player_y 
 */
void place_guardians(int** terrain, int number_guardians, Guardian* guardians, double player_x, double player_y);


/**
 * @brief to know if the game is in pause
 * 
 * @param is_paused 
 * @return int 
 */
int detection_pause(int is_paused);

/**
 * @brief In the main window it permits to know what 
 *        choice the user make between play, change
 *        parameter or quit the game
 * 
 * @return int 
 */
int choose_menu();

/**
 * @brief take the entries in the keyboard to have 
 *        the nickname of the player in the scoreboard
 * 
 * @param nickname 
 * @param i 
 * @return char*
 */
char* enter_nickname();

/**
 * @brief Begin the panic mode
 * 
 * @param guardians 
 * @param size 
 */
void enter_panic_mode(Guardian* guardians, int size);

/**
 * @brief to know if the user wants to resume the game or end it
 * 
 * @param mouse_x 
 * @param mouse_y 
 * @return int 
 */
int choose_pause(int mouse_x, int mouse_y);

/**
 * @brief to choose pause button
 * 
 * @return int 
 */
int make_pause();


/**
 * @brief Create the list of point around the player or the guardians
 * 
 * @param pos_x 
 * @param pos_y 
 * @param number_points 
 * @param radius 
 * @return Point* 
 */
Point* create_points(double pos_x, double pos_y, int number_points, double radius, int* nb_points_in_angle);

/**
 * @brief to know if the user chooses to retry the game or end it
 * 
 * @return int 
 */
int choose_end();

/**
 * @brief to change parameter of the game like the difficulty
 * 
 * @param nb_guardians 
 * @param nb_relics 
 * @param minside 
 * @param panic_duration 
 * @param sound 
 * @param can_power 
 * @return int 
 */
int choose_parameter(int* nb_guardians, int* nb_relics, int* minside, int* panic_duration, int* sound, int* can_power);

/**
 * @brief to know if the panic mode is finish by compare the duration of the panic mode
 * and the time elapsed
 * 
 * @param debut 
 * @param guardians_list 
 * @param size 
 * @param seconde 
 * @return double 
 */
double panic_mode_times_up(struct timeval debut, Guardian* guardians_list, int number, int seconde);

/**
 * @brief to know if the player is back on the main door
 * 
 * @param player 
 * @param player_has_relics 
 * @return int 
 */
int player_enters_base(Player player, int player_has_relics);

/**
 * @brief Allows us to restrict a value between a minimum and a maximum
 * 
 * @param value 
 * @param min 
 * @param max 
 * @return int 
 */
void clamp(int* value, int min, int max);

/**
 * @brief Restrict parameter values between minimum and
 *        maximum for each of them
 * 
 * @param nb_guardians 
 * @param nb_relics 
 * @param minside 
 * @param panic_duration 
 */
void clamp_settings(int* nb_guardians, int* nb_relics, int* minside, int* panic_duration);

/**
 * @brief The user can choose between quitting or restarting the game
 * 
 * @param choice 
 * @param sound 
 */
void end_user_choice(int choice, int sound);

#endif