/*
 * Portions of this file are copyright Rebirth contributors and licensed as
 * described in COPYING.txt.
 * Portions of this file are copyright Parallax Software and licensed
 * according to the Parallax license below.
 * See COPYING.txt for license details.

THE COMPUTER CODE CONTAINED HEREIN IS THE SOLE PROPERTY OF PARALLAX
SOFTWARE CORPORATION ("PARALLAX").  PARALLAX, IN DISTRIBUTING THE CODE TO
END-USERS, AND SUBJECT TO ALL OF THE TERMS AND CONDITIONS HEREIN, GRANTS A
ROYALTY-FREE, PERPETUAL LICENSE TO SUCH END-USERS FOR USE BY SUCH END-USERS
IN USING, DISPLAYING,  AND CREATING DERIVATIVE WORKS THEREOF, SO LONG AS
SUCH USE, DISPLAY OR CREATION IS FOR NON-COMMERCIAL, ROYALTY OR REVENUE
FREE PURPOSES.  IN NO EVENT SHALL THE END-USER USE THE COMPUTER CODE
CONTAINED HEREIN FOR REVENUE-BEARING PURPOSES.  THE END-USER UNDERSTANDS
AND AGREES TO THE TERMS HEREIN AND ACCEPTS THE SAME BY USE OF THIS FILE.
COPYRIGHT 1993-1999 PARALLAX SOFTWARE CORPORATION.  ALL RIGHTS RESERVED.
*/

/*
 *
 * prototype definitions for descent.cfg reading/writing
 *
 */

#pragma once

#if defined(DXX_BUILD_DESCENT_I) || defined(DXX_BUILD_DESCENT_II)
#include "player-callsign.h"
#include "mission.h"
#endif

#ifdef __cplusplus
#include "pack.h"
#include "compiler-array.h"
#include "ntstring.h"

struct CCfg : prohibit_void_ptr<CCfg>
{
	bool VSync;
	bool Grabinput;
};

#if defined(DXX_BUILD_DESCENT_I) || defined(DXX_BUILD_DESCENT_II)
struct Cfg : prohibit_void_ptr<Cfg>
{
	int MusicType;
	int CMLevelMusicPlayOrder;
	int GammaLevel;
	int ResolutionX;
	int ResolutionY;
	int AspectX;
	int AspectY;
	int TexFilt;
	uint8_t DigiVolume;
	uint8_t MusicVolume;
	bool WindowMode;
	bool Multisample;
	bool FPSIndicator;
	bool ReverseStereo;
	bool OrigTrackOrder;
#ifdef DXX_BUILD_DESCENT_II
	bool MovieSubtitles;
	int MovieTexFilt;
#endif
	callsign_t LastPlayer;
	array<int, 2> CMLevelMusicTrack;
	ntstring<MISSION_NAME_LEN> LastMission;
	ntstring<PATH_MAX - 1> CMLevelMusicPath;
	array<ntstring<PATH_MAX - 1>, 5> CMMiscMusic;
};
extern struct Cfg GameCfg;
#endif

extern struct CCfg CGameCfg;

//#ifdef USE_SDLMIXER
//#define EXT_MUSIC_ON (GameCfg.SndEnableRedbook || GameCfg.JukeboxOn)
//#else
//#define EXT_MUSIC_ON (GameCfg.SndEnableRedbook)		// JukeboxOn shouldn't do anything if it's not supported
//#endif

extern int ReadConfigFile(void);
extern int WriteConfigFile(void);

#endif
