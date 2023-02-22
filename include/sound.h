#ifndef __SOUND__
#define __SOUND__

#include "game_manager.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <MLV/MLV_all.h>


/**
 * @brief Plays a sound if the parameter is activated in parameter
 *        and freezes the game until the sound ends
 * 
 * @param sound_status To know which sound to play
 * @param can_sounds If the sound is activated in the game settings
 */
void play_sound(int sound_status, int can_sounds);

/**
 * @brief Loop music until game is over or sounds are disabled in setting
 * 
 * @param can_music If the music is activated in the game settings  
 */
void play_music(MLV_Music* music, int can_music);

#endif