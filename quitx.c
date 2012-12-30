/*	quitx.c : routine that destroys resources and quits.
*/

#include "copyright.h"
#include "defs.h"

/*	quit_game destroys all resources, closes the score file, and quits.
*/

void	quit_game()
{
	register int	i, j;

	for(i=0; i<PLAYERPHASES; i++) {
		XFreePixmap(display, playerpix[i]);
	}
	for(i=0; i<BURNDIRECTIONS; i++) {
		XFreePixmap(display, burnpix[i]);
	}
	for(i=0; i<FIREPHASES; i++) {
		XFreePixmap(display, firepix[i]);
	}
	for(i=0; i<SWEEPERPHASES; i++) {
		XFreePixmap(display, sweeperpix[i]);
	}
	for(i=0; i<EXPLODEFRAMES; i++) {
		XFreePixmap(display, explodepix[i]);
	}
	for(i=0; i<GUARDDIRECTIONS; i++) {
		for(j=0; j<GUARDPHASES; j++) {
			XFreePixmap(display, guardpix[i][j]);
		}
	}
	for(i=0; i<DOORFRAMES; i++) {
		XFreePixmap(display, doorpix[i]);
	}
	XFreePixmap(display, fuelpix);
	XFreePixmap(display, keypix);
	XFreePixmap(display, extramanpix);
	XFreePixmap(display, scorepixmap);

#ifdef	BLIT
	XFreePixmap(display, drawpixmap);
#endif

	XDestroySubwindows(display, gamewindow);
	XDestroyWindow(display, gamewindow);
    XCloseDisplay(display);
	close_scores();
	exit(0);
}
