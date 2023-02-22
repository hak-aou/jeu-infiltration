#ifndef __GUARDIAN__
#define __GUARDIAN__

#include "terrain.h"
#include "collision.h"
#include "player.h"

#define CHANGE_DIRECTION_PROBA 50
#define NUMBER_POINT_AROUND 200

#define INITIAL_CASE_DETECTION 4
#define PANIC_CASE_DETECTION 6

typedef enum {
    NORTH_G,
    SOUTH_G,
    WEST_G,
    EAST_G
} Direction;

typedef struct {
    double x;
    double y;
    double speed;
    int case_detection;
    int is_in_panic;
    int view_angle;
    int view_direction;
    Direction direction;
} Guardian;

typedef struct {
  double x;
  double y;
} Point;


/**
 * @brief initialize the fields of a guardian
 * 
 * @param guardians 
 * @param number_guardians 
 */
void initialize_guardians(Guardian* guardians, int number_guardians);

/**
 * @brief change the position of a guardian by checking
 * if guardian is in panic and if he will meet a wall 
 * 
 * @param guardians 
 * @param terrain 
 */
void change_direction(Guardian* guardians, int** terrain);

/**
 * @brief manage the movement of guardians
 * 
 * @param guardians 
 * @param number_guardians 
 * @param terrain 
 */
void update_position_guardian(Guardian* guardians, int number_guardians, int** terrain);


/**
 * @brief to know if the player or a relic is visible by a guardian
 * return : 0 if the object is behind a wall
 *          1 if the object is detected
 *          2 if the object is to far away to be detected
 * @param x 
 * @param y 
 * @param gardien 
 * @param terrain 
 * @return int 
 */
int zone_detection_guardian(double x, double y, Guardian gardien, int ** terrain);


/**
 * @brief to know if a guardian has detected is a relic is missing
 * 
 * @param guardian 
 * @param terrain 
 * @return int 
 */
int panic_mode(Guardian guardian, int** terrain);

/**
 * @brief change the poistions of the points wich represent the field of view of the guardian to match when
 * the points meet a wall it permits to deform the field of view
 * 
 * @param lst_point 
 * @param guardian 
 * @param terrain 
 * @param number_points 
 */
void dragon_view(Point* lst_point, Guardian guardian, int** terrain, int number_points);

#endif