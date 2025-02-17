/*
 * This file is part of the DXX-Rebirth project <http://www.dxx-rebirth.com/>.
 * It is copyright by its individual contributors, as recorded in the
 * project's Git history.  See COPYING.txt at the top level for license
 * terms and a link to the Git history.
 */
/*
 * 
 * Setup for 3d library
 * 
 */


#include <stdlib.h>

#include "dxxerror.h"

#include "3d.h"
#include "globvars.h"
#include "clipper.h"

#ifdef OGL
#include "ogl_init.h"
#else
#include "texmap.h"  // for init_interface_vars_to_assembler()
#endif
#include "gr.h"

//start the frame
void g3_start_frame(void)
{
	fix s;

	//set int w,h & fixed-point w,h/2
	Canv_w2 = (Canvas_width  = grd_curcanv->cv_bitmap.bm_w)<<15;
	Canv_h2 = (Canvas_height = grd_curcanv->cv_bitmap.bm_h)<<15;
#ifdef __powerc
	fCanv_w2 = f2fl((Canvas_width  = grd_curcanv->cv_bitmap.bm_w)<<15);
	fCanv_h2 = f2fl((Canvas_height = grd_curcanv->cv_bitmap.bm_h)<<15);
#endif

	//compute aspect ratio for this canvas

	s = fixmuldiv(grd_curscreen->sc_aspect,Canvas_height,Canvas_width);

	if (s <= f1_0) {	   //scale x
		Window_scale.x = s;
		Window_scale.y = f1_0;
	}
	else {
		Window_scale.y = fixdiv(f1_0,s);
		Window_scale.x = f1_0;
	}
	
	Window_scale.z = f1_0;		//always 1

#ifdef OGL
	ogl_start_frame();
#else
	init_interface_vars_to_assembler();		//for the texture-mapper
#endif
}
