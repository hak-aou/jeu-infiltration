#include "../include/guardian.h"


/**
 * @brief random number in a range, gives a double
 * 
 * @param min 
 * @param max 
 * @return double 
 */
static double random_range_double(double min, double max){
    double diff = (max - min);
    return ( (double) rand() / RAND_MAX ) * diff + min;
}

void initialize_guardians(Guardian* guardians, int number_guardians){
    double new_speed = random_range_double(0.3, 0.8) * SPEED;
    int i;

    assert(guardians != NULL);
    assert(number_guardians >= 0);

    for(i = 0; i < number_guardians; i++){
        guardians[i].direction = rand() % 4;
        guardians[i].speed = new_speed;
        guardians[i].case_detection = INITIAL_CASE_DETECTION;
        guardians[i].is_in_panic = 0;
        guardians[i].view_angle = random_range(80, 200);
        guardians[i].view_direction = 0;
    }
}

void change_direction(Guardian* guardians, int** terrain){
    int is_touched = 0;
    int shift;
    double new_speed;
    int new_direction;
    int get_number;
    
    assert(guardians != NULL);
    assert(terrain != NULL);

    new_speed = SPEED;
    shift = 2;
    if(!guardians->is_in_panic){
        new_speed = random_range_double(0.3, 0.8) * SPEED;
        shift = 1;
    }

    is_touched = collision_simple(&guardians->x, &guardians->y, terrain, shift) ||
                 collision_diagonal(&guardians->x, &guardians->y, terrain, shift);
    is_touched = collision_simple(&guardians->x, &guardians->y, terrain, 1) ||
                 collision_diagonal(&guardians->x, &guardians->y, terrain, 1);
                 
    get_number = rand() % CHANGE_DIRECTION_PROBA;
    if(is_touched == 1 || get_number == 1){
        new_direction = rand() % 4;

        guardians->direction = new_direction;
        guardians->speed = new_speed;
        guardians->view_direction = (guardians->view_direction + 90) % 360;
    }
}

void update_position_guardian(Guardian* guardians, int number_guardians, int** terrain){
    int i;

    assert(guardians != NULL);
    assert(number_guardians >= 0);
    assert(terrain != NULL);

    for(i = 0; i < number_guardians; i++){
        if(guardians[i].direction == NORTH_G)
            guardians[i].y -= guardians[i].speed;
        
        if(guardians[i].direction == SOUTH_G)
            guardians[i].y += guardians[i].speed;
        
        if(guardians[i].direction == EAST_G)
            guardians[i].x += guardians[i].speed;
        
        if(guardians[i].direction == WEST_G)
            guardians[i].x -= guardians[i].speed;

        change_direction(&guardians[i], terrain);
    }
}


/**
 * @brief  Function that lets you know if the coordinates are a
 *         valid coordinate of the terrain
 * 
 * @param x
 * @param y 
 * @return int  1 : true
 *              0 : false
 */
static int in_terrain(int x, int y){
    if(0 < x && x < NUMBER_CASE_COL * TAILLE_CASE &&
       0 < y && y < NUMBER_CASE_LIG * TAILLE_CASE )
        return 1;
    return 0;
}


/**
 * @brief to know if there is a wall between two points
 * 
 * @param x0 
 * @param y0 
 * @param x1 
 * @param y1 
 * @param terrain 
 * @param point 
 * @return int 
 */
static int is_wall_between(double x0, double y0, double x1, double y1, int ** terrain, Point* point){
    int mx = 1, my = 1, wall = 0;
    double a, b, ya, xb, pa, pb;
    int ax, ay, bx, by;
    
    assert(terrain != NULL);
    assert(point != NULL);

    if(x0 > x1)
        mx = -1;
    if(y0 > y1)
        my = -1;


    /*We start at point (x0, y0)*/
    a = x0 - (int) x0 % TAILLE_CASE;
    b = y0 - (int) y0 % TAILLE_CASE;
    
    /*We travel the segment wich separates the two point until we go outside of it*/
    while(1){
        ya = y1 + (y0 - y1) / (x0 - x1) * (a - x1);
        xb = x1 + (x0 - x1) / (y0 - y1) * (b - y1);
        ax = a;
        by = b;
        ay = ya - (int) ya % TAILLE_CASE;
        bx = xb - (int) xb % TAILLE_CASE;
        

        if(pa >= 0 && pa <= 1){ /*Are we in the segment wich separates the two points ?*/
            /*We check if the point is in terrain and if there is wall*/
            if(in_terrain(ax, ay) && terrain[ay / TAILLE_CASE][ax / TAILLE_CASE] == WALL){
                wall = 1;
                if((x0 < a && a < x1) || (x1 < a && a < x0)){
                    point->x = a;
                    point->y = ya;
                }
            }
            /*if the point (x0, y0) is at the top right of the point (x1, y1) we check the case on the left and see if its a wall*/
            if(x1 < x0 && y1 > y0){
                if(in_terrain(ax - TAILLE_CASE, ay) && terrain[ay / TAILLE_CASE][ax / TAILLE_CASE  - 1] == WALL){
                    wall = 1;
                    if((x0 < a && a < x1) || (x1 < a && a < x0)){
                        point->x = a;
                        point->y = ya;
                    }
                }
            }

        }
        if(pb >= 0 && pb <= 1){ /*Are we in the segment wich separates the two points ?*/
            /*We check if the point is in terrain and if there is wall*/
            if(in_terrain(bx, by) && terrain[by / TAILLE_CASE][bx / TAILLE_CASE] == WALL){
                wall = 1;
                if((y0 < b && b < y1) || (y1 < b && b < y0)){
                    point->x = xb;
                    point->y = b;
                }
            }
            /*if the point (x0, y0) is at the below left of the point (x1, y1) we check the case on the top and see if its a wall*/
            if(x1 > x0 && y1 < y0){
                if(in_terrain(bx, by - TAILLE_CASE) && terrain[by / TAILLE_CASE - 1][bx / TAILLE_CASE] == WALL){
                    wall = 1;
                    if((y0 < b && b < y1) || (y1 < b && b < y0)){
                        point->x = xb;
                        point->y = b;
                    }
                }
            }

        }
        /*We move in abscissa and in ordinate on the segment wich separates the two points*/
        a = a + TAILLE_CASE * mx;
        b = b + TAILLE_CASE * my;

        /*if our abscissa and our ordinate are outside the segment we break the loop*/
        if(((mx == 1 && a > x1) || (mx == -1 && a < x1)) && 
           ((my == 1 && b > y1) || (my == -1 && b < y1)))
            break;

        /*We recalculate the p to know if we are still on the segment*/
        pa = (a - x1) / (x0 - x1);

        pb = (b - y1) / (y0 - y1);
        
    }

    return wall;
}


int zone_detection_guardian(double x, double y, Guardian gardien, int ** terrain){
    double x0 = gardien.x;
    double y0 = gardien.y;
    double x1 = x;
    double y1 = y;

    int distance_perso_gardien = sqrt(pow(x0 - x1, 2) + pow(y0 - y1, 2));

    assert(terrain != NULL);

    if(distance_perso_gardien <= TAILLE_CASE * gardien.case_detection){ /* On detecte si le joueur est dans le champs de vision d'un gardien */
        Point p_null;

        /* On regarde si un mur separe le joueur du gardien */
        if(is_wall_between(x0, y0, x1, y1, terrain, &p_null) == 1)
            return 0;
        else{
            return 1;
        }
    }
    return 2;
}

int panic_mode(Guardian guardian, int** terrain){
    int line, column;
    int row = guardian.y / TAILLE_CASE;
    int col = guardian.x / TAILLE_CASE;

    assert(terrain != NULL);

    for(line = row - guardian.case_detection ; line < row + guardian.case_detection ; ++line){
        for(column = col - guardian.case_detection ; column < col + guardian.case_detection ; ++column){  
            if(in_terrain(column * TAILLE_CASE, line * TAILLE_CASE) && terrain[line][column] == NO_RELIC){
                if(zone_detection_guardian(column * TAILLE_CASE, line * TAILLE_CASE, guardian, terrain) == 1){
                    terrain[line][column] = DETECTED_NO_RELIC;
                    return 1;
                }
            }
        }
    }
    
    return 0;
}

void dragon_view(Point* lst_point, Guardian guardian, int** terrain, int number_points){
    double x0, y0, x1, y1;
    int i;

    assert(lst_point != NULL);
    assert(terrain != NULL);
    
    x0 = guardian.x;
    y0 = guardian.y;

    for(i = 0 ; i < number_points ; ++i){
        x1 = lst_point[i].x;
        y1 = lst_point[i].y;
        is_wall_between(x1, y1, x0, y0, terrain, &lst_point[i]);
    }
}