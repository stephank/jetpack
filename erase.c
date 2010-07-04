/*	erase.c : routines used in non-blitting version that erase-and-draw
	objects and reduce flicker.
*/

#include "copyright.h"
#include "defs.h"

/*	Note: the functions and variables in this file are not used in the
	blitting version.
*/

#ifndef BLIT

#define MAXOBJECTS 6+MAXLINES+MAXFIREBALLS+MAXSWEEPERS+MAXFUELPODS+MAXGUARDS+MAXHIGHSCORES/5

/*	Structure for eraseable zones that minimize flicker
*/
struct zone {
	int	type, num;
	int	x, y, w, h;
	int	numtouch;
	int	touch[MAXOBJECTS];
	int	erased, drawn;
};

int			numzones;
struct zone	zones[MAXOBJECTS];

/*	init_zones initializes the eraseable zones
*/
init_zones()
{
	numzones = 0;
}

/*	set_zone creates an eraseable zone. x and y are the upper left hand
	corner, w and h are the width and height of the rectangle, type is the
	kind of object it is (player, fireball, etc,) and num is the index to
	the object's information arrays. If type == ZLINE, w and h are used as
	the second endpoint of a line.
*/
set_zone(x, y, w, h, type, num)
int	x, y, w, h, type, num;
{
	register int	j;

	zones[numzones].x = x;
	zones[numzones].y = y;
	zones[numzones].w = w;
	zones[numzones].h = h;
	zones[numzones].type = type;
	zones[numzones].num = num;
	zones[numzones].erased = 0;
	zones[numzones].drawn = 0;
	zones[numzones].numtouch = 0;
	for(j=0; j<numzones; j++) {
		if(zone_intersect(numzones,j)) {
			zones[j].touch[zones[j].numtouch] = numzones;
			zones[j].numtouch++;
			break;
		}
	}
	numzones++;
}

/*	zone_intersect checks if two zones with indexes i and j touch. There
	is also a built-in error EDGE, since the player moving around can add
	some uncertainty to what is covering what.
*/
int	zone_intersect(i, j)
int	i, j;
{
	if(zones[i].type == ZLINE) {
		if(zones[j].type == ZLINE) {
			return((zones[i].w >= zones[j].x - EDGE) && (zones[i].x <=
					zones[j].w + EDGE) && (zones[i].h >= zones[j].y -
					EDGE) && (zones[i].y <= zones[j].h + EDGE));
		} else {
			return((zones[i].w >= zones[j].x - EDGE) && (zones[i].x <=
					zones[j].x + zones[j].w + EDGE) && (zones[i].h >=
					zones[j].y - EDGE) && (zones[i].y <= zones[j].y +
					zones[j].h + EDGE));
		}
	} else {
		if(zones[j].type == ZLINE) {
			return((zones[j].w >= zones[i].x - EDGE) && (zones[j].x <=
					zones[i].x + zones[i].w + EDGE) && (zones[j].h >=
					zones[i].y - EDGE) && (zones[j].y <= zones[i].y +
					zones[i].h + EDGE));
		} else {
			return((zones[i].x > zones[j].x - zones[i].w - EDGE) &&
					(zones[i].x < zones[j].x + zones[j].w + EDGE) &&
					(zones[i].y > zones[j].y - zones[i].h - EDGE) &&
					(zones[i].y < zones[j].y + zones[j].h + EDGE));
		}
	}
}

/*	erase_draw_zones erases all the zones and draws the associated objects
	in their new positions in such a way as to minimize flicker. The way
	this works is to erase all zones that touch each other in a group,
	draw their asscociated objects, then go to the next group. This way
	the time between erase and draw is minimized for each object while
	preventing nasty overwriting.
*/
erase_draw_zones()
{
	register int	i;

	for(i=0; i<numzones; i++) {
		erase_recur(i);
		draw_recur(i);
	}
}

/*	erase_recur erases zone i and all the zones that touch it.
*/
erase_recur(i)
int	i;
{
	register int	j;

	if(zones[i].erased) return;
	for(j=0; j<zones[i].numtouch; j++) {
		erase_recur(zones[i].touch[j]);
	}
	if(zones[i].type == ZLINE) {
		XDrawLine(display, gamewindow, ctable[CBLACK].smallgc, zones[i].x,
					zones[i].y, zones[i].w, zones[i].h);
	} else {
		XFillRectangle(display, gamewindow, ctable[CBLACK].smallgc,
						zones[i].x, zones[i].y, zones[i].w, zones[i].h);
	}
	zones[i].erased = 1;
}

/*	draw_recur draws the object associated with zone i and the objects
	associated with all the zones that touch it.
*/
draw_recur(i)
int	i;
{
	register int	j, tmp;

	if(zones[i].drawn) return;
	for(j=0; j<zones[i].numtouch; j++) {
		draw_recur(zones[i].touch[j]);
	}
	switch(zones[i].type) {
		case ZLINE:
			XDrawLine(display, gamewindow, ctable[CWHITE].smallgc,
						mazelines[zones[i].num].x1 - plx + WINDOWWIDTH / 2,
						mazelines[zones[i].num].y1 - ply + WINDOWHEIGHT / 2,
						mazelines[zones[i].num].x2 - plx + WINDOWWIDTH / 2,
						mazelines[zones[i].num].y2 - ply + WINDOWHEIGHT / 2);
			break;
		case ZPLAYER:
			if(!dead && (exploded == -1)) {
				XCopyArea(display, playerpix[playerphase], gamewindow,
							ctable[CWHITE].smallgc, 0, 0, PLAYERWIDTH,
							PLAYERHEIGHT, WINDOWWIDTH / 2 - PLAYERWIDTH / 2,
							WINDOWHEIGHT / 2 - PLAYERHEIGHT / 2);
			}
			break;
		case ZBURN:
			if(!dead && (exploded == -1) && burn) {
				XCopyArea(display, burnpix[playerphase / FACEFRONT],
							gamewindow, ctable[CWHITE].smallgc, 0, 0,
							PLAYERWIDTH, PLAYERHEIGHT, WINDOWWIDTH / 2 -
							PLAYERWIDTH / 2, WINDOWHEIGHT / 2 - PLAYERHEIGHT
							/ 2);
			}
			break;
		case ZEXPLODE:
			if(!dead && (exploded > -1)) {
				XCopyArea(display, explodepix[exploded], gamewindow,
							ctable[CWHITE].smallgc, 0, 0, EXPLODEWIDTH,
							EXPLODEHEIGHT, WINDOWWIDTH / 2 - EXPLODEWIDTH / 2,
							WINDOWHEIGHT / 2 - EXPLODEHEIGHT / 2);
			}
			break;
		case ZFIRE:
			XCopyArea(display, firepix[firephase[zones[i].num]],
						gamewindow, ctable[CWHITE].smallgc, 0, 0,
						FIREWIDTH, FIREHEIGHT, transx(firex[zones[i].num],
						FIREWIDTH), transy(firey[zones[i].num],
						FIREHEIGHT));
			break;
		case ZGUARD:
			XCopyArea(display,
				guardpix[guarddir[zones[i].num]][guardphase[zones[i].num]],
						gamewindow, ctable[CWHITE].smallgc, 0, 0,
						GUARDWIDTH, GUARDHEIGHT, transx(guardx[zones[i].num],
						GUARDWIDTH), transy(guardy[zones[i].num],
						GUARDHEIGHT));
			break;
		case ZSWEEP:
			XCopyArea(display, sweeperpix[sweepphase[zones[i].num]],
						gamewindow, ctable[CWHITE].smallgc, 0, 0,
						SWEEPERWIDTH, SWEEPERHEIGHT,
						transx(sweepx[zones[i].num], SWEEPERWIDTH),
						transy(sweepy[zones[i].num], SWEEPERHEIGHT));
			break;
		case ZFUEL:
			if(fuelalive[zones[i].num]) {
				XCopyArea(display, fuelpix, gamewindow,
							ctable[CWHITE].smallgc, 0, 0, FUELWIDTH,
							FUELHEIGHT, transx(fuelx[zones[i].num],
							FUELWIDTH), transy(fuely[zones[i].num],
							FUELHEIGHT));
			} else if(fueltimer[zones[i].num]) {
				tmp = XTextWidth(fontinfo, "100", 3) / 2;
				XDrawString(display, gamewindow, ctable[CGREEN].smallgc,
							fuelx[zones[i].num] - plx + WINDOWWIDTH / 2 -
							tmp, fuely[zones[i].num] - ply +
							WINDOWHEIGHT / 2, "100", 3);
			}
			break;
		case ZKEY:
			if(keyalive) {
				XCopyArea(display, keypix, gamewindow,
							ctable[CWHITE].smallgc, 0, 0, KEYWIDTH,
							KEYHEIGHT, transx(keyx, KEYWIDTH),
							transy(keyy, KEYHEIGHT));
			} else if(keytimer) {
				tmp = XTextWidth(fontinfo, "500", 3) / 2;
				XDrawString(display, gamewindow, ctable[CGREEN].smallgc,
							keyx - plx + WINDOWWIDTH / 2 - tmp, keyy -
							ply + WINDOWHEIGHT / 2, "500", 3);
			}
			break;
		case ZDOOR:
			tmp = indoor;
			if(indoor && (exploded > -1))  tmp = 0;
			XCopyArea(display, doorpix[tmp], gamewindow,
						ctable[CWHITE].smallgc, 0, 0, DOORWIDTH,
						DOORHEIGHT, transx(doorx, DOORWIDTH),
						transy(doory, DOORHEIGHT) - (DOORHEIGHT -
						PLAYERHEIGHT) / 2);
			break;
		case ZSMESS:
			if(messagetime != 0) {
				if(messagecolor == CBLACK) tmp = random() % (NCOLORS-1) + 1;
				else tmp = messagecolor;
				XDrawString(display, gamewindow, ctable[tmp].smallgc,
							messagex, messagey, messagestring, messagelength);
			}
			break;
		case ZBMESS:
			if(bigmessagetime != 0) {
				if(bigmessagecolor == CBLACK) tmp = random() % (NCOLORS-1) + 1;
				else tmp = bigmessagecolor;
				XDrawString(display, gamewindow, ctable[tmp].biggc,
							bigmessagex, bigmessagey, bigmessagestring, 
							bigmessagelength);
			}
			break;
	}
	zones[i].drawn = 1;
}

/*	refresh draws all the objects
*/
refresh()
{
	register int	i;

	for(i=0; i<numzones; i++) {
		draw_recur(i);
	}
	for(i=0; i<numzones; i++) {
		zones[i].drawn = 0;
	}
}

#endif
