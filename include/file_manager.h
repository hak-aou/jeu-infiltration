#ifndef __FILE_MANAGER__
#define __FILE_MANAGER__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define SIZE_NAME 5
#define MAX_SCORE 10

typedef struct {
    int mana;
    double time;
    char name[SIZE_NAME + 1];
} Score;


/**
 * @brief Writes player score information to a binary file.
 * 
 * @param mana The total mana used
 * @param time The duration of the game time
 * @param nickname The player's nickname
 * @param file_name The name of the file
 */
void write_score_to_file(int mana, double time, const char* nickname, const char* file_name);


/**
 * @brief Get the total number of score in the file.
 * 
 * @param file_name The name of the file.
 * @return int Return the number of score in the file.
 */
int get_size_number_score(const char* file_name);


/**
 * @brief Return the list containing all the scores of the file.
 * 
 * @param file_name The name of the file
 * @param size The number of score in file
 * @return Score* Returns a pointer to a score list
 */
Score* get_score_from_file(const char* file_name, int size);


/**
 * @brief Function used by the qsort() function to compare two
          Score struct according to their mana.
 * 
 * @param first First pointer to void used to compare.
 * @param second  Second pointer to void used to compare.
 * @return int 1 : first < second
              -1 : first > second
               0 : first == second
 */
int comparator_score_mana(const void * first, const void * second);


/**
 * @brief Function used by the qsort() function to compare two
          Score struct according to their time.
 * 
 * @param first First pointer to void used to compare.
 * @param second  Second pointer to void used to compare.
 * @return int 1 : first < second
              -1 : first > second
               0 : first == second
 */
int comparator_score_time(const void * first, const void * second);

#endif
