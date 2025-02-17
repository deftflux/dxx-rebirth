/*
 * This file is part of the DXX-Rebirth project <http://www.dxx-rebirth.com/>.
 * It is copyright by its individual contributors, as recorded in the
 * project's Git history.  See COPYING.txt at the top level for license
 * terms and a link to the Git history.
 */
/*
 * Digital audio support
 * Library-independent stub for dynamic selection of sound system
 */

#include "dxxerror.h"
#include "vecmat.h"
#include "piggy.h"
#include "config.h"
#include "digi.h"
#include "sounds.h"
#include "console.h"
#include "rbaudio.h"
#include "jukebox.h"
#include "args.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <digi.h>
#include <digi_audio.h>

#ifdef USE_SDLMIXER
#include <digi_mixer.h>
#endif
#ifdef _WIN32
#include "hmp.h"
#endif

#ifndef __PIE__
/* PIE -> paranoid checks
 * No PIE -> prefer speed
 */
#define DXX_COPY_SOUND_TABLE
#endif

/* Sound system function pointers */

namespace {

struct sound_function_table_t
{
	int  (*init)();
	void (*close)();
#ifndef RELEASE
	void (*reset)();
#endif
	void (*set_channel_volume)(int, int);
	void (*set_channel_pan)(int, int);
	int  (*start_sound)(short, fix, int, int, int, int, sound_object *);
	void (*stop_sound)(int);
	void (*end_sound)(int);
	int  (*is_channel_playing)(int);
	void (*stop_all_channels)();
	void (*set_digi_volume)(int);
};

#ifdef USE_SDLMIXER
static const sound_function_table_t digi_mixer_table{
	&digi_mixer_init,
	&digi_mixer_close,
#ifndef RELEASE
	&digi_mixer_reset,
#endif
	&digi_mixer_set_channel_volume,
	&digi_mixer_set_channel_pan,
	&digi_mixer_start_sound,
	&digi_mixer_stop_sound,
	&digi_mixer_end_sound,
	&digi_mixer_is_channel_playing,
	&digi_mixer_stop_all_channels,
	&digi_mixer_set_digi_volume,
};
#endif

static const sound_function_table_t digi_audio_table{
	&digi_audio_init,
	&digi_audio_close,
#ifndef RELEASE
	&digi_audio_reset,
#endif
	&digi_audio_set_channel_volume,
	&digi_audio_set_channel_pan,
	&digi_audio_start_sound,
	&digi_audio_stop_sound,
	&digi_audio_end_sound,
	&digi_audio_is_channel_playing,
	&digi_audio_stop_all_channels,
	&digi_audio_set_digi_volume,
};

class sound_function_pointers_t
{
#ifdef USE_SDLMIXER
#ifdef DXX_COPY_SOUND_TABLE
	sound_function_table_t table;
#else
	const sound_function_table_t *table;
#endif
#endif
public:
	__attribute_cold
	void report_invalid_table() __noreturn;
	inline const sound_function_table_t *operator->();
	inline sound_function_pointers_t &operator=(const sound_function_table_t &t);
};

#ifdef USE_SDLMIXER
#ifdef DXX_COPY_SOUND_TABLE
const sound_function_table_t *sound_function_pointers_t::operator->()
{
	return &table;
}

sound_function_pointers_t &sound_function_pointers_t::operator=(const sound_function_table_t &t)
{
	table = t;
	return *this;
}
#else
void sound_function_pointers_t::report_invalid_table()
{
	/* Out of line due to length of generated code */
	throw std::logic_error("invalid table");
}

const sound_function_table_t *sound_function_pointers_t::operator->()
{
	if (table != &digi_audio_table && table != &digi_mixer_table)
		report_invalid_table();
	return table;
}

sound_function_pointers_t &sound_function_pointers_t::operator=(const sound_function_table_t &t)
{
	table = &t;
	/* Trap bad initial assignment */
	operator->();
	return *this;
}
#endif
#else
const sound_function_table_t *sound_function_pointers_t::operator->()
{
	return &digi_audio_table;
}

sound_function_pointers_t &sound_function_pointers_t::operator=(const sound_function_table_t &)
{
	return *this;
}
#endif

}

static sound_function_pointers_t fptr;

void digi_select_system(int n) {
	switch (n) {
#ifdef USE_SDLMIXER
	case SDLMIXER_SYSTEM:
	con_printf(CON_NORMAL,"Using SDL_mixer library");
		fptr = digi_mixer_table;
	break;
#endif
	case SDLAUDIO_SYSTEM:
	default:
	con_printf(CON_NORMAL,"Using plain old SDL audio");
		fptr = digi_audio_table;
 	break;
	}
}

/* Common digi functions */
#ifndef NDEBUG
static int digi_initialised = 0;
#endif
#if defined(DXX_BUILD_DESCENT_I)
int digi_sample_rate = SAMPLE_RATE_11K;
#endif
int digi_volume = SOUND_MAX_VOLUME;

/* Stub functions */

int  digi_init()
{
	digi_init_sounds();
	return fptr->init();
}

void digi_close() { fptr->close(); }
#ifndef RELEASE
void digi_reset() { fptr->reset(); }
#endif

void digi_set_channel_volume(int channel, int volume) { fptr->set_channel_volume(channel, volume); }
void digi_set_channel_pan(int channel, int pan) { fptr->set_channel_pan(channel, pan); }

int  digi_start_sound(short soundnum, fix volume, int pan, int looping, int loop_start, int loop_end, sound_object *soundobj)
{
	return fptr->start_sound(soundnum, volume, pan, looping, loop_start, loop_end, soundobj);
}

void digi_stop_sound(int channel) { fptr->stop_sound(channel); }
void digi_end_sound(int channel) { fptr->end_sound(channel); }

int  digi_is_channel_playing(int channel) { return fptr->is_channel_playing(channel); }
void digi_stop_all_channels() { fptr->stop_all_channels(); }
void digi_set_digi_volume(int dvolume) { fptr->set_digi_volume(dvolume); }

#ifndef NDEBUG
void digi_debug()
{
	int n_voices = 0;

	if (!digi_initialised) return;

	for (int i = 0; i < digi_max_channels; i++)
	{
		if (digi_is_channel_playing(i))
			n_voices++;
        }
}
#endif

#ifdef _WIN32
// Windows native-MIDI stuff.
int digi_win32_midi_song_playing=0;
static std::unique_ptr<hmp_file> cur_hmp;
static int firstplay = 1;

void digi_win32_set_midi_volume( int mvolume )
{
	hmp_setvolume(cur_hmp.get(), mvolume*MIDI_VOLUME_SCALE/8);
}

int digi_win32_play_midi_song( const char * filename, int loop )
{
	if (firstplay)
	{
		hmp_reset();
		firstplay = 0;
	}
	digi_win32_stop_midi_song();

	if (filename == NULL)
		return 0;

	if ((cur_hmp = hmp_open(filename)))
	{
		/* 
		 * FIXME: to be implemented as soon as we have some kind or checksum function - replacement for ugly hack in hmp.c for descent.hmp
		 * if (***filesize check*** && ***CRC32 or MD5 check***)
		 *	(((*cur_hmp).trks)[1]).data[6] = 0x6C;
		 */
		if (hmp_play(cur_hmp.get(),loop) != 0)
			return 0;	// error
		digi_win32_midi_song_playing = 1;
		digi_win32_set_midi_volume(GameCfg.MusicVolume);
		return 1;
	}

	return 0;
}

void digi_win32_pause_midi_song()
{
	hmp_pause(cur_hmp.get());
}

void digi_win32_resume_midi_song()
{
	hmp_resume(cur_hmp.get());
}

void digi_win32_stop_midi_song()
{
	if (!digi_win32_midi_song_playing)
		return;
	cur_hmp.reset();
	digi_win32_midi_song_playing = 0;
	hmp_reset();
}
#endif
