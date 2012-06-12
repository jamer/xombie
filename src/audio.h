#ifndef _AUDIO_H_
#define _AUDIO_H_

/** Global audio class
 *
 * <Write documentation here>
 */

#include <map>
#include <SDL_mixer.h>

#include "conf.h"

class Audio
{
public:
	Audio();
	~Audio();

	bool play(QString sound);
	bool startMusic();
	// TODO: Have Level select which songs to play

private:
	Conf* conf;
	Mix_Music* music;
	std::map<uint32_t, Mix_Chunk*> data;

	bool audioSupported;
	bool soundEnabled, musicEnabled;
	int soundVolume, musicVolume;
	int musicChannel;
};

Audio* getAudio();

#endif // _AUDIO_H_
