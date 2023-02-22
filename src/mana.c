#include "../include/mana.h"

List allocate_cell(Case value){
    List tmp;
    tmp = (Cell *) malloc(sizeof(Cell));
    if(tmp == NULL) return NULL;
    
    tmp->value = value;
    tmp->next = NULL;

    return tmp;
}

List insert_in_head(List *lst, Case value){
    List tmp;

    assert(lst != NULL);

    tmp = allocate_cell(value);
    if(tmp == NULL) return NULL;

    tmp->next = *lst;
    *lst = tmp;

    return tmp;
}

int case_is_equal(Case first, Case second){
    return first.row == second.row &&
           first.col == second.col;
}

void delete_cell(List* lst, Case value){
    List tmp = *lst;
    List tmp2;
    
    assert(lst != NULL);

    if(!*lst)
        return;

    /* Delete in the head */
    if(case_is_equal(tmp->value, value)){
        (*lst) = (*lst)->next;
        free(tmp);
        return;
    }
    /* Delete in the body */ 
    while(tmp){
        if(case_is_equal(tmp->value, value)){
            tmp2->next = tmp->next;
            free(tmp);
            return;
        }
        tmp2 = tmp; /* save the cel */
        tmp = tmp->next; /* save the next cel */
    }
}

int count_size_list(List lst){
    List index = lst;
    int compteur = 0;

    while(index != NULL) {
        compteur++;
        index = index->next;
    }
    
    return compteur;
}


Case* get_case_from_lst(List lst, int cmpt_value){
    List index = lst;
    int compteur = 0;

    while(index != NULL) {
        if(compteur == cmpt_value){
            return &(index->value);
        }
        index = index->next;
        compteur++;
    }
    return NULL;
}


int is_in_linked_list(List lst, Case new_case){
    List index = lst;

    while(index != NULL) {
        if(case_is_equal(index->value, new_case)){
            return 1;
        }
        index = index->next;
    }

    return 0;
}