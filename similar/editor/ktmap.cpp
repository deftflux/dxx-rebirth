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
 * Texture map key bindings.
 *
 */

#include <string.h>
#include "inferno.h"
#include "editor.h"
#include "editor/esegment.h"
#include "kdefs.h"

//	Assign CurrentTexture to Curside in *Cursegp
int AssignTexture(void)
{
   autosave_mine( mine_filename );
	undo_status[Autosave_count] = "Assign Texture UNDONE.";

	Cursegp->sides[Curside].tmap_num = CurrentTexture;

	New_segment.sides[Curside].tmap_num = CurrentTexture;

//	propagate_light_intensity(Cursegp, Curside, CurrentTexture, 0); 
																					 
	Update_flags |= UF_WORLD_CHANGED;

	return 1;
}

//	Assign CurrentTexture to Curside in *Cursegp
int AssignTexture2(void)
{
	int texnum, orient, ctexnum, newtexnum;

   autosave_mine( mine_filename );
	undo_status[Autosave_count] = "Assign Texture 2 UNDONE.";

	texnum = Cursegp->sides[Curside].tmap_num2 & 0x3FFF;
	orient = ((Cursegp->sides[Curside].tmap_num2 & 0xC000) >> 14) & 3;
	ctexnum = CurrentTexture;
	
	if ( ctexnum == texnum )	{
		orient = (orient+1) & 3;
		newtexnum = (orient<<14) | texnum;
	} else {
		newtexnum = ctexnum;
	}

	Cursegp->sides[Curside].tmap_num2 = newtexnum;
	New_segment.sides[Curside].tmap_num2 = newtexnum;

	Update_flags |= UF_WORLD_CHANGED;

	return 1;
}

int ClearTexture2(void)
{
   autosave_mine( mine_filename );
	undo_status[Autosave_count] = "Clear Texture 2 UNDONE.";

	Cursegp->sides[Curside].tmap_num2 = 0;

	New_segment.sides[Curside].tmap_num2 = 0;

	Update_flags |= UF_WORLD_CHANGED;

	return 1;
}


//	--------------------------------------------------------------------------------------------------
//	Propagate textures from Cursegp through Curside.
//	If uv_flag !0, then only propagate uv coordinates (if 0, then propagate textures as well)
//	If move_flag !0, then move forward to new segment after propagation, else don't
static int propagate_textures_common(int uv_flag, int move_flag)
{
   autosave_mine( mine_filename );
	undo_status[Autosave_count] = "Propagate Textures UNDONE.";
	const auto c = Cursegp->children[Curside];
	if (IS_CHILD(c))
		med_propagate_tmaps_to_segments(Cursegp, vsegptridx(c), uv_flag);

	if (move_flag)
		SelectCurrentSegForward();

	Update_flags |= UF_WORLD_CHANGED;

	return 1;
}

//	Propagate texture maps from current segment, through current side
int PropagateTextures(void)
{
	return propagate_textures_common(0, 0);
}

//	Propagate texture maps from current segment, through current side
int PropagateTexturesUVs(void)
{
	return propagate_textures_common(-1, 0);
}

//	Propagate texture maps from current segment, through current side
// And move to that segment.
int PropagateTexturesMove(void)
{
	return propagate_textures_common(0, 1);
}

//	Propagate uv coordinate from current segment, through current side
// And move to that segment.
int PropagateTexturesMoveUVs(void)
{
	return propagate_textures_common(-1, 1);
}


//	-------------------------------------------------------------------------------------
static int is_selected_segment(segnum_t segnum)
{
	return Selected_segs.contains(segnum);
}

//	-------------------------------------------------------------------------------------
//	Auxiliary function for PropagateTexturesSelected.
//	Recursive parse.
static void pts_aux(const vsegptridx_t sp, visited_segment_bitarray_t &visited)
{
	visited[sp] = true;

	for (int side=0; side<MAX_SIDES_PER_SEGMENT; side++) {
		const auto c = sp->children[side];
		if (IS_CHILD(c))
		{
			const auto &&csegp = vsegptridx(c);
			while (!visited[c] && is_selected_segment(c))
			{
				med_propagate_tmaps_to_segments(sp, csegp, 0);
				pts_aux(csegp, visited);
			}
		}
	}
}

//	-------------------------------------------------------------------------------------
//	Propagate texture maps from current segment recursively exploring all children, to all segments in Selected_list
//	until a segment not in Selected_list is reached.
int PropagateTexturesSelected(void)
{
   autosave_mine( mine_filename );
	undo_status[Autosave_count] = "Propogate Textures Selected UNDONE.";

	visited_segment_bitarray_t visited;
	visited[Cursegp] = true;

	pts_aux(Cursegp, visited);

	Update_flags |= UF_WORLD_CHANGED;

	return 1;
}

