#include <SDL.h>
#include <SDL_mixer.h>
#include <stdlib.h>
#include <stdio.h>

#include "audio.h"
#include "common.h"
#include "conf.h"
#include "random.h"

Audio* audio;

Audio::Audio()
	: audioSupported(true)
{
	audio = this;
	conf = new Conf("conf/audio.conf");

	// Load a couple variables.
	soundEnabled = conf->getBool("Settings", "Sound enabled", true);
	musicEnabled = conf->getBool("Settings", "Music enabled", true);
	soundVolume  = conf->getInt ("Settings", "Sound volume" , 100);
	musicVolume  = conf->getInt ("Settings", "Music volume" , 100);

	// The rest of this is just init'ing SDL_mixer.
	int rate     = conf->getInt("Playback", "Rate"    , 44100);
	int channels = conf->getInt("Playback", "Channels", 2);

	unsigned short format = AUDIO_S16SYS; // Signed 16-bit samples in
					      // system's natural byte order.
	int buffers = 4096;


	if (SDL_Init(SDL_INIT_AUDIO)) {
		fprintf(stderr, "Unable to inialize audio: %s\n",
				SDL_GetError());
		audioSupported = false;
	}

	if (Mix_OpenAudio(rate, format, channels, buffers) < 0) {
		fprintf(stderr, "Warning: Unable to initialize audio: %s\n",
			       	Mix_GetError());
		audioSupported = false;
	}
}

Audio::~Audio()
{
	std::map<uint32_t, Mix_Chunk*>::iterator it;
	
	for (it = data.begin(); it != data.end(); it++)
		Mix_FreeChunk(it->second);
	data.clear();
	delete conf;
	
	Mix_HaltMusic();
	Mix_FreeMusic(music);
	
	Mix_CloseAudio();
}

bool Audio::play(const char* sound)
{
	if (!audioSupported || !soundEnabled || !soundVolume)
		return true;

	if (sound == NULL)
		return true;

	// Generate hash
	char* lower = strdup(sound);
	for (unsigned int i = 0; i < strlen(lower); i++)
		lower[i] = tolower(lower[i]);

	uint32_t h = hash(lower);
	free(lower);

	// Do we have the sound file cached?
	std::map<uint32_t, Mix_Chunk*>::iterator i = data.find(h);
	Mix_Chunk* chunk = NULL;

	// If not found, load it and save it in cache.
	if (i == data.end()) {
		chunk = Mix_LoadWAV(conf->getString("Sounds", sound, NULL));
		if (chunk == NULL) {
			fprintf(stderr, "Unable to load audio file '%s': %s\n",
					sound, Mix_GetError());
			return false;
		}
		data[h] = chunk;
	}

	// Else we have it.
	else
		chunk = i->second;


	// Play!
	int channel = Mix_PlayChannel(-1, chunk, 0);
	if (channel == -1) {
		fprintf(stderr, "Unable to play audio file: %s\n", 
				Mix_GetError());
		return false;
	}

	Mix_Volume(channel, soundVolume - rand()%(soundVolume/2));

	return true;
}

bool Audio::startMusic()
{
	if (!audioSupported || !musicEnabled || !musicVolume)
		return true;
	
	const char* musicFile = randInt(0, 1) ? "Music 1" : "Music 2";

	music = Mix_LoadMUS(conf->getString("Music", musicFile));
	if (music == NULL) {
		fprintf(stderr, "Unable to load Ogg file: %s\n",
				Mix_GetError());
		return false;
	}

	musicChannel = Mix_PlayMusic(music, -1);
	if (musicChannel == -1) {
		fprintf(stderr, "Unable to play Ogg file: %s\n",
				Mix_GetError());
		return false;
	}

#if 0
	musicPlaying = true;
	Mix_HookMusicFinished(musicFinished);

	while (musicPlaying) {
		SDL_Delay(100);
	}

	Mix_HaltMusic();
	Mix_FreeMusic(music);
	Mix_CloseAudio();
#endif

	return true;
}



#if 0
bool musicPlaying;
void musicFinished()
{
	musicPlaying = false;
}
#endif

Audio* getAudio()
{
	return audio;
}

