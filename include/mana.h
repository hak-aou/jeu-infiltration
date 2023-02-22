#ifndef __LINKED_LIST__
#define __LINKED_LIST__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct {
    int row;
    int col;
} Case;

typedef struct cel {
    Case value;
    struct cel* next;
} Cell, *List;


/**
 * @brief Allocation of a cell with a value and
 *        a next cell equal to null
 * 
 * @param value 
 * @return List 
 */
List allocate_cell(Case value);

/**
 * @brief Insert a cell at the top of the list
 * 
 * @param lst
 * @param value 
 * @return List 
 */
List insert_in_head(List *lst, Case value);

/**
 * @brief Checks if two case have the same row and column coordinates
 * 
 * @param first 
 * @param second 
 * @return int 
 */
int case_is_equal(Case first, Case second);

/**
 * @brief delete a cell in the linked list
 * 
 * @param lst 
 * @param value 
 */
void delete_cell(List* lst, Case value);

/**
 * @brief Count the number of cells in the linked list
 * 
 * @param lst 
 * @return int 
 */
int count_size_list(List lst);

/**
 * @brief Get the case from linked list
 * 
 * @param lst 
 * @param cmpt_value 
 * @return Case* 
 */
Case* get_case_from_lst(List lst, int cmpt_value);

/**
 * @brief Check if an element is in the linked list
 * 
 * @param lst 
 * @param new_case 
 * @return int 
 */
int is_in_linked_list(List lst, Case new_case);

#endif