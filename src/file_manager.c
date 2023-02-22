#include "../include/file_manager.h"


void write_score_to_file(int mana, double time, const char* nickname, const char* file_name) {
    FILE* file;
    Score score;
    
    assert(nickname != NULL);
    assert(file_name != NULL);

    /* Open file in binary write mode */
    file = fopen(file_name, "ab");
    if (file == NULL) {
        fprintf(stderr, "Error to open the file");
        exit(EXIT_FAILURE);
    }

    /* Stores values in a Score structure */
    score.mana = mana;
    score.time = time;
    strcpy(score.name, nickname);

    /* Write in file */
    fwrite(&score, sizeof(Score), 1, file);

    fclose(file);
}

int get_size_number_score(const char* file_name){
    FILE* file;
    int size = 0;
    Score score;
    size_t n;
    
    assert(file_name != NULL);

    /* Open the file in binary reading mode */
    file = fopen(file_name, "rb");
    if (file == NULL) {
        return -1;
    }
    
    /* Browse the file */
    while((n = fread(&score, sizeof(Score), 1, file)) > 0)
        size++; /* Count the number of scores */

    fclose(file);
    return size;
}

Score* get_score_from_file(const char* file_name, int size) {
    /* Declaration and Initialization */
    FILE* file;
    Score* lst_score = malloc(sizeof(Score) * size);
    Score score;
    size_t n;
    int i = 0;

    assert(file_name != NULL);
    assert(size >= 0);

    /* Open the file in binary reading mode */
    file = fopen(file_name, "rb");
    if (file == NULL) {
        fprintf(stderr, "Error to open : %s", file_name);
        exit(EXIT_FAILURE);
    }

    /* Browse the file */
    while ((n = fread(&score, sizeof(Score), 1, file)) > 0) {
        /* Stores score values in a Score list */
        lst_score[i].mana = score.mana;
        lst_score[i].time = score.time;
        strcpy(lst_score[i].name, score.name);
        i++;
    }

    fclose(file);
    return lst_score;
} 

int comparator_score_mana(const void * first, const void * second){
    /* Declaration and Initialization */
    Score* first_score = (Score *) first;
    Score* second_score = (Score *) second;
    long difference;
    
    assert(first != NULL);
    assert(second != NULL);
    
    /* Mana Difference */
    difference = first_score->mana - second_score->mana;
    
    if(difference > 0)
        return 1;
    if(difference < 0)
        return -1;
    return 0;
}

int comparator_score_time(const void * first, const void * second){
    /* Declaration and Initialization */
    Score* first_score = (Score *) first;
    Score* second_score = (Score *) second;
    long difference;

    assert(first != NULL);
    assert(second != NULL);

    /* Times Difference */
    difference = first_score->time - second_score->time;
    
    if(difference > 0)
        return 1;
    if(difference < 0)
        return -1;
    return 0;
}