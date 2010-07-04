/*	initx.c : routines that set up the X resources and colors.
*/

#include "copyright.h"
#include "defs.h"

Display			*display;
Colormap		colormap;
Window			rootwindow;
int				screen, depth;
unsigned long	blackpixel, whitepixel;
XFontStruct		*fontinfo, *bigfontinfo;

/*	initialize the colortable with the names of the colors (from rgb.txt)
*/
struct colors	ctable[] = {
	{ "black"		},
	{ "red"			},
	{ "orange"		},
	{ "yellow"		},
	{ "RoyalBlue2"	},
	{ "grey50"		},
	{ "green"		},
	{ "white"		}
};

/*	Depending on your system, the fontnames listed here may not be
	complete enough to pick the right font. The system I developed on
	has 475 fonts, so these should be specific enough. But just in
	case, here are the full fontnames:

	small font:
	-misc-fixed-medium-r-normal--10-70-100-100-c-60-iso8859-1

	big font:
	-adobe-courier-bold-r-normal--25-180-100-100-m-150-iso8859-1

	It also might be that you won't have these fonts. If not, pick two
	fonts that look good. The program adjusts things on the screen for
	whatever fonts the program uses, so almost anything should do.
	Just pick a pretty small font for the small font, and a reasonably
	large font for the big font. Use xfontsel to make this easier
	since it makes it really easy to pick fonts, and you can paste the
	resulting fontname right into the code.
*/

static char	*fontname = "-*-fixed-medium-r-normal--*-70-*-*-c-*-*-*";
static char	*bigfontname = "-*-courier-bold-r-normal--*-180-*-*-m-150-*-*";

/*	init_X opens the display and sets up all the color stuff
*/
init_X()
{
    display = XOpenDisplay(NULL);
    if (display == NULL) {
       fprintf(stderr, "Jetpack : Cannot connect to X Server %s\n",
               XDisplayName(NULL));
       exit(1);
    }
	rootwindow = DefaultRootWindow(display);
    screen = DefaultScreen(display);
	colormap = DefaultColormap(display, screen);
    depth = DefaultDepth(display, screen);
   	blackpixel = BlackPixel(display, screen);
   	whitepixel = WhitePixel(display, screen);
	set_up_colors();
	set_up_contexts();
}

/*	set_up_contexts creates the graphics smallgcs used in the game, and
	stores them in the ctable array. The fonts are created here too.
*/
set_up_contexts()
{
	int			i;
	XGCValues	values;
	Font		small, big;

	fontinfo = XLoadQueryFont(display, fontname);
	if(fontinfo == NULL) {
		fprintf(stderr, "Jetpack : Couldn't load %s font.\n",fontname);
		exit(1);
	}
	bigfontinfo = XLoadQueryFont(display, bigfontname);
	if(bigfontinfo == NULL) {
		fprintf(stderr, "Jetpack : Couldn't load %s font.\n",bigfontname);
		exit(1);
	}
	big = bigfontinfo->fid;
	small = fontinfo->fid;
	values.font = small;
	values.function = GXcopy;
	ctable[CBLACK].smallgc = XCreateGC(display, rootwindow,
										GCFunction|GCFont, &values);
	values.font = big;
	ctable[CBLACK].biggc = XCreateGC(display, rootwindow,
										GCFunction|GCFont, &values);
	values.function = GXor;
	for(i=1; i<NCOLORS; i++) {
		values.font = small;
		ctable[i].smallgc = XCreateGC(display, rootwindow,
										GCFunction|GCFont, &values);
		values.font = big;
		ctable[i].biggc = XCreateGC(display, rootwindow,
									GCFunction|GCFont, &values);
	}
	for(i=0; i<NCOLORS; i++) {
		XSetForeground(display, ctable[i].smallgc,
						ctable[i].pixelvalue);
		XSetBackground(display, ctable[i].smallgc,
						ctable[CBLACK].pixelvalue);
		XSetForeground(display, ctable[i].biggc,
						ctable[i].pixelvalue);
		XSetBackground(display, ctable[i].biggc,
						ctable[CBLACK].pixelvalue);
	}
}

/*	set_up_colors looks up the pixel values of the colors. */
set_up_colors()
{
	int		i;
	XColor	screencolor, exactcolor;

	for(i=0; i<NCOLORS; i++) {
		XAllocNamedColor(display, colormap, ctable[i].name, &screencolor, &exactcolor);
		ctable[i].pixelvalue = exactcolor.pixel;
	}
}
