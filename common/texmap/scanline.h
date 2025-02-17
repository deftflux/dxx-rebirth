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
COPYRIGHT 1993-1998 PARALLAX SOFTWARE CORPORATION.  ALL RIGHTS RESERVED.
*/

/*
 *
 * Prototypes for C versions of texture mapped scanlines.
 *
 */

#pragma once

#ifdef __cplusplus
#include <string>

extern void c_tmap_scanline_lin();
extern void c_tmap_scanline_lin_nolight();
extern void c_tmap_scanline_flat();
extern void c_tmap_scanline_shaded();

struct tmap_scanline_function_table
{
	using per = void ();
	per *sl_per;
};

#define cur_tmap_scanline_per (tmap_scanline_functions.sl_per)
#define cur_tmap_scanline_lin (c_tmap_scanline_lin)
#define cur_tmap_scanline_lin_nolight (c_tmap_scanline_lin_nolight)
#define cur_tmap_scanline_shaded (c_tmap_scanline_shaded)
#define cur_tmap_scanline_flat (c_tmap_scanline_flat)

extern tmap_scanline_function_table tmap_scanline_functions;
void select_tmap(const std::string &type);

#endif
