#include "SDL.h"
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <string>

void LoadMedia(std::string path) {
	Mix_Music* gMusic = NULL;
	//Load music
	gMusic = Mix_LoadMUS("21_sound_effects_and_music/beat.wav");
	if (gMusic == NULL)
	{
		printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}

	//Load sound effects
	gScratch = Mix_LoadWAV("21_sound_effects_and_music/scratch.wav");
	if (gScratch == NULL)
	{
		printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}
}