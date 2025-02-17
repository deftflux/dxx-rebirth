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
 * Structure information for the player
 *
 */

#pragma once

#include "fwd-player.h"

#include <physfs.h>
#include "vecmat.h"
#include "weapon.h"

#ifdef __cplusplus
#include <algorithm>
#include "pack.h"
#include "dxxsconf.h"
#include "compiler-array.h"
#include "compiler-static_assert.h"
#include "objnum.h"
#include "player-callsign.h"

#if defined(DXX_BUILD_DESCENT_I) || defined(DXX_BUILD_DESCENT_II)
// When this structure changes, increment the constant
// SAVE_FILE_VERSION in playsave.c
struct player : public prohibit_void_ptr<player>
{
	// Who am I data
	callsign_t callsign;   // The callsign of this player, for net purposes.
	sbyte   connected;              // Is the player connected or not?
	objnum_t     objnum;                 // What object number this player is. (made an int by mk because it's very often referenced)

	//  -- make sure you're 4 byte aligned now!

	// Game data
	uint    flags;                  // Powerup flags, see below...
	fix     energy;                 // Amount of energy remaining.
	fix     shields;                // shields remaining (protection)
	ubyte   lives;                  // Lives remaining, 0 = game over.
	sbyte   level;                  // Current level player is playing. (must be signed for secret levels)
	stored_laser_level   laser_level;            // Current level of the laser.
	sbyte   starting_level;         // What level the player started on.
	objnum_t   killer_objnum;          // Who killed me.... (-1 if no one)
#if defined(DXX_BUILD_DESCENT_I)
	ubyte		primary_weapon_flags;					//	bit set indicates the player has this weapon.
	ubyte		secondary_weapon_flags;					//	bit set indicates the player has this weapon.
#elif defined(DXX_BUILD_DESCENT_II)
	ushort  primary_weapon_flags;   // bit set indicates the player has this weapon.
	ushort  secondary_weapon_flags; // bit set indicates the player has this weapon.
#endif
	ushort  vulcan_ammo;
	array<uint8_t, MAX_SECONDARY_WEAPONS>  secondary_ammo; // How much ammo of each type.

	// Statistics...
	int     last_score;             // Score at beginning of current level.
	int     score;                  // Current score.
	fix     time_level;             // Level time played
	fix     time_total;             // Game time played (high word = seconds)

	fix64   cloak_time;             // Time cloaked
	fix64   invulnerable_time;      // Time invulnerable

	short   KillGoalCount;          // Num of players killed this level
	short   net_killed_total;       // Number of times killed total
	short   net_kills_total;        // Number of net kills total
	short   num_kills_level;        // Number of kills this level
	short   num_kills_total;        // Number of kills total
	short   num_robots_level;       // Number of initial robots this level
	short   num_robots_total;       // Number of robots total
	ushort  hostages_rescued_total; // Total number of hostages rescued.
	ushort  hostages_total;         // Total number of hostages.
	ubyte   hostages_on_board;      // Number of hostages on ship.
	ubyte   hostages_level;         // Number of hostages on this level.
	fix     homing_object_dist;     // Distance of nearest homing object.
	sbyte   hours_level;            // Hours played (since time_total can only go up to 9 hours)
	sbyte   hours_total;            // Hours played (since time_total can only go up to 9 hours)
};

// Same as above but structure how Savegames expect
struct player_rw
{
	// Who am I data
	callsign_t callsign;   // The callsign of this player, for net purposes.
	ubyte   net_address[6];         // The network address of the player.
	sbyte   connected;              // Is the player connected or not?
	int     objnum;                 // What object number this player is. (made an int by mk because it's very often referenced)
	int     n_packets_got;          // How many packets we got from them
	int     n_packets_sent;         // How many packets we sent to them

	//  -- make sure you're 4 byte aligned now!

	// Game data
	uint    flags;                  // Powerup flags, see below...
	fix     energy;                 // Amount of energy remaining.
	fix     shields;                // shields remaining (protection)
	ubyte   lives;                  // Lives remaining, 0 = game over.
	sbyte   level;                  // Current level player is playing. (must be signed for secret levels)
	ubyte   laser_level;            // Current level of the laser.
	sbyte   starting_level;         // What level the player started on.
	short   killer_objnum;          // Who killed me.... (-1 if no one)
#if defined(DXX_BUILD_DESCENT_I)
	ubyte		primary_weapon_flags;					//	bit set indicates the player has this weapon.
	ubyte		secondary_weapon_flags;					//	bit set indicates the player has this weapon.
#elif defined(DXX_BUILD_DESCENT_II)
	ushort  primary_weapon_flags;   // bit set indicates the player has this weapon.
	ushort  secondary_weapon_flags; // bit set indicates the player has this weapon.
#endif
	ushort	laser_ammo;
	ushort  vulcan_ammo;
	ushort  spreadfire_ammo, plasma_ammo, fusion_ammo;
#if defined(DXX_BUILD_DESCENT_II)
	ushort  obsolete_primary_ammo[MAX_PRIMARY_WEAPONS - 5];
#endif
	ushort  secondary_ammo[MAX_SECONDARY_WEAPONS]; // How much ammo of each type.

#if defined(DXX_BUILD_DESCENT_II)
	ushort  pad; // Pad because increased weapon_flags from byte to short -YW 3/22/95
#endif
	//  -- make sure you're 4 byte aligned now

	// Statistics...
	int     last_score;             // Score at beginning of current level.
	int     score;                  // Current score.
	fix     time_level;             // Level time played
	fix     time_total;             // Game time played (high word = seconds)

	fix     cloak_time;             // Time cloaked
	fix     invulnerable_time;      // Time invulnerable

#if defined(DXX_BUILD_DESCENT_II)
	short   KillGoalCount;          // Num of players killed this level
#endif
	short   net_killed_total;       // Number of times killed total
	short   net_kills_total;        // Number of net kills total
	short   num_kills_level;        // Number of kills this level
	short   num_kills_total;        // Number of kills total
	short   num_robots_level;       // Number of initial robots this level
	short   num_robots_total;       // Number of robots total
	ushort  hostages_rescued_total; // Total number of hostages rescued.
	ushort  hostages_total;         // Total number of hostages.
	ubyte   hostages_on_board;      // Number of hostages on ship.
	ubyte   hostages_level;         // Number of hostages on this level.
	fix     homing_object_dist;     // Distance of nearest homing object.
	sbyte   hours_level;            // Hours played (since time_total can only go up to 9 hours)
	sbyte   hours_total;            // Hours played (since time_total can only go up to 9 hours)
} __pack__;
#if defined(DXX_BUILD_DESCENT_I)
static_assert(sizeof(player_rw) == 116, "wrong size player_rw");
#elif defined(DXX_BUILD_DESCENT_II)
static_assert(sizeof(player_rw) == 142, "wrong size player_rw");
#endif

#define get_local_player()	(Players[Player_num])
#define get_local_plrobj()	(*vobjptr(get_local_player().objnum))
#endif

struct player_ship
{
	int     model_num;
	int     expl_vclip_num;
	fix     mass,drag;
	fix     max_thrust,reverse_thrust,brakes;       //low_thrust
	fix     wiggle;
	fix     max_rotthrust;
	array<vms_vector, N_PLAYER_GUNS> gun_points;
}
#if defined(DXX_BUILD_DESCENT_I)
__pack__
#endif
;

#endif
