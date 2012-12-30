/*	windowx.c : routines that set up the windows and pixmaps in the game.
*/

#include "copyright.h"
#include "defs.h"

static XWMHints wm_hint = {
	InputHint, 1,
};

/*	open_window is a primitive general function to open a window inside
	another window
*/

Window open_window(x, y, width, height, parent)
int		x, y;
int		width, height;
Window	parent;
{
    XSetWindowAttributes	windowattr;
    XSizeHints				sizehints;
	XClassHint				classhint;
    unsigned long			windowmask;
    Window					newwindow;

    windowattr.background_pixel = ctable[CBLACK].pixelvalue;
    windowattr.border_pixel = ctable[CWHITE].pixelvalue;
	windowattr.event_mask =	KeyPressMask | ButtonPressMask |
							ButtonReleaseMask | ExposureMask;
	windowattr.do_not_propagate_mask =	KeyPressMask |
										ButtonPressMask |
										ButtonReleaseMask |
										ExposureMask;
	windowattr.override_redirect = False;
    windowmask =	CWBorderPixel | CWBackPixel | CWEventMask |
					CWOverrideRedirect;
    newwindow = XCreateWindow(display, parent, x, y, width, height,
									1, depth, InputOutput, CopyFromParent,
									windowmask, &windowattr);
	classhint.res_name = "jetpack";
	classhint.res_class = "game";
	XSetClassHint(display, newwindow, &classhint);
    XSetWMHints(display, newwindow, &wm_hint);
	XStoreName(display,newwindow,"Jetpack");
    sizehints.flags = PPosition | PSize | PMaxSize | PMinSize;
    sizehints.x = x;
    sizehints.y = y;
    sizehints.width = width;
    sizehints.height = height;
	sizehints.min_width = width;
	sizehints.max_width = width;
	sizehints.min_height = height;
	sizehints.max_height = height;
    XSetNormalHints(display, newwindow, &sizehints);
    return(newwindow);
}

/*	set_up_windows sets up the major windows and pixmaps used in the game.
*/

void	set_up_windows()
{
	gamewindow = open_window(0, 0, WINDOWWIDTH, WINDOWHEIGHT+INFOHEIGHT,
								rootwindow);
	infowindow = open_window(0, WINDOWHEIGHT, WINDOWWIDTH-1, INFOHEIGHT-1,
								gamewindow);
	dialogwidth = XTextWidth(bigfontinfo,"MMMMMMMMMMMMMMMMMMMMMMMMM",25) +
								20;
    dialogheight = 4 * (bigfontinfo->ascent + bigfontinfo->descent) + 45;
    dialogwindow = open_window(WINDOWWIDTH / 2 - dialogwidth / 2,
                                WINDOWHEIGHT / 2 - dialogheight - 2,
                                dialogwidth, dialogheight, gamewindow);
    dialogstringwindow = open_window(10, 2 * (bigfontinfo->ascent +
										bigfontinfo->descent) + 15,
										dialogwidth - 20,
										(bigfontinfo->ascent +
										bigfontinfo->descent) + 10,
										dialogwindow);
    dialogbuttonwindow = open_window(10, 3 * (bigfontinfo->ascent +
										bigfontinfo->descent) + 30,
										dialogwidth / 5,
										(bigfontinfo->ascent +
										bigfontinfo->descent) + 10,
										dialogwindow);

#ifdef	BLIT
	drawpixmap = XCreatePixmap(display, gamewindow, WINDOWWIDTH,
							 	WINDOWHEIGHT, depth);
	XFillRectangle(display, drawpixmap, ctable[CBLACK].smallgc, 0, 0,
					WINDOWWIDTH, WINDOWHEIGHT);
#endif

	scorepixmap = XCreatePixmap(display, gamewindow, SCOREWIDTH,
								SCOREHEIGHT, depth);
	XFillRectangle(display, scorepixmap, ctable[CBLACK].smallgc, 0, 0,
					SCOREWIDTH, SCOREHEIGHT);
    XMapWindow(display, gamewindow);
    XMapWindow(display, infowindow);
    XFlush(display);
}
