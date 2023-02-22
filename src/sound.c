#include "../include/sound.h"

void play_sound(int sound_status, int can_sounds){
    MLV_Sound* sound;

    if(!can_sounds)
        return;

    switch(sound_status){
        case PLAY:
            sound = MLV_load_sound("sounds/jouer.ogg" );
            if(sound != NULL){
                MLV_play_sound(sound, 0.3);
                MLV_wait_milliseconds(400);
            }
            break;
        
        case PARAMETER:
            sound = MLV_load_sound( "sounds/parametre.ogg" );
            if(sound != NULL){
                MLV_play_sound(sound, 0.3);
                MLV_wait_milliseconds(460);
            }
            break;
        
        case QUIT:
            sound = MLV_load_sound( "sounds/quitter.ogg" );
            if(sound != NULL){
                MLV_play_sound(sound, 0.3);
                MLV_wait_milliseconds(470);
            }
            break;
        
        case PAUSE:
            sound = MLV_load_sound( "sounds/pause.ogg" );
            if(sound != NULL){
                MLV_play_sound(sound, 0.3);
                MLV_wait_milliseconds(470);
            }
            break;
        
        case LOSE:
            sound = MLV_load_sound( "sounds/perdre.ogg" );
            if(sound != NULL){
                MLV_play_sound(sound, 0.3);
                MLV_wait_milliseconds(1200);
            }
            break;
        
        case WIN:
            sound = MLV_load_sound( "sounds/gagner.ogg" );
            if(sound != NULL){
                MLV_play_sound(sound, 0.3);
              MLV_wait_milliseconds(1100);
            }
            break;
        
        case RETRY:
        case RESUME:
            sound = MLV_load_sound( "sounds/continuer.ogg" );
            if(sound != NULL){
                MLV_play_sound(sound, 0.3);
                MLV_wait_milliseconds(900);
            }
            break;
        
        case DEAD:
            sound = MLV_load_sound( "sounds/mort.ogg" );
            if(sound != NULL){
                MLV_play_sound(sound, 0.3);
                MLV_wait_milliseconds(150);
            }
            break;
        
        default:
            break;
    }

    MLV_free_sound(sound);
}

void play_music(MLV_Music* music, int can_music){
    assert(music != NULL);

    if(can_music && music != NULL)
        MLV_play_music(music, 0.2, -1);
    else if(music != NULL)
        MLV_play_music(music, 0, -1);
}